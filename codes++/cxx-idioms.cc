/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype> 
#include <string>
#include <list>
#include <set>

#include <getopt.h>
#include <errno.h>

extern char *__progname;
const char usage[]=
      "%s [options]: class1 opt... -- class2 opt... -- classn opt...\n"
     "\n Class:\n"
      "   -c class            class  name\n"
      "   -s struct           struct name\n"
      "\n Inheritance:\n"                                            
      "   -p name -p name ... public    derivation\n"
      "   -r name -p name ... protected derivation\n"
      "   -v name -p name ... private   derivation\n"
      "\n Template:\n"
      "   -n elem -n elem ... template non-type\n"
      "   -t name ...         template parameter\n"
      "   -w name ...         template template parameter\n"
      "\n Idioms:\n"
      "   -A elem -A elem ... named-constructor idiom\n"
      "   -B elem -B elem ... named-parameter idiom\n"
      "   -C elem -C elem ... public overloaded non-virtuals call\n"
      "                       protect non-overloaded virtual idiom\n"
      "   -O                  virtual-friend-function idiom (printOn)\n"            
      "   -3                  big-three guideline\n"
      "   -h                  print this help.\n"
      "\n Element: id,<type>,<default> examples: \n"
      "   'test' 'home,int' 'point,char *, NULL'\n\n";


// basic element
//

class Element {

    friend class Class;

    std::string __id;
    std::string __type;
    std::string __symbol;
    std::string __default;

    Element(const std::string &i, const std::string &t = "bool", const std::string &d = "true" ) :
        __id(i), __type(t), __symbol(t), __default(d) {

            std::string::iterator it = __symbol.begin();
            std::string::iterator it_end = __symbol.end();

            replace(it, it_end, ' ', '_');
            replace(it, it_end, '*', 'P');
            replace(it, it_end, '&', 'R');
            replace(it, it_end, ':', '$');

        }

    public:

    static Element elementOpt(std::string option, const std::string &t, const std::string &d="") {
        std::string::size_type l = option.find(",");
        if (l != std::string::npos )
           option = option.substr(0,l); 

        return Element(option,t,d); 
    }

    static Element elementOpt(std::string option) {

        std::string::size_type l = option.find(",");
        if ( l == std::string::npos )
            return Element(option);

        std::string type = option.substr(l+1);
        std::string::size_type r = type.find(",");
        if ( r == std::string::npos )
            return Element(option.substr(0,l), type );

        std::string def = type.substr(r+1);
        return Element(option.substr(0,l), type.substr(0,r), def);
    }
    
    const std::string &get_id() const { return __id; }
    const std::string &get_type() const { return __type; }
    const std::string &get_symbol() const { return __symbol; }
    const std::string &get_default() const { return __default; }
};



// Base Class    ::::::::::::::::::::::::::::::::::::::::::::
//
struct BaseClass {

    std::string __name;

    bool __private ;
    bool __protected ;
    bool __public ;

    bool __template;
    bool __derivation;
    bool __include;

    // idioms
    //

    bool __bigthree ;
    bool __printOn ;
    bool __explicit ;
    bool __ponvpnov;
    bool __nci ;
    bool __npi ;

    BaseClass(const std::string &n="") : __name(n),
    __private(false), __protected(false), __public(false),
    __template(false), __derivation(false),__bigthree(false), __printOn(false), __explicit(false), 
    __ponvpnov(false),__nci(false), __npi(false) {} 

    BaseClass& _private()     { __private = true; return *this; }
    BaseClass& _protected()   { __protected = true; return *this; }
    BaseClass& _public()      { __public = true; return *this; }
    BaseClass& _template()    { __template = true; return *this; }
    BaseClass& _derivation()  { __derivation = true; return *this; }
    BaseClass& _include()     { __include = true; return *this; }
    BaseClass& _bigthree()    { __bigthree = true; return *this; }
    BaseClass& _nci()         { __nci = true; return *this; }
    BaseClass& _npi()         { __npi = true; return *this; }
    BaseClass& _printOn()     { __printOn = true; return *this; }
    BaseClass& _explicit()    { __explicit = true; return *this; }
    BaseClass& _ponvpnov()    { __ponvpnov = true; return *this; }
 
};

// include policy  ::::::::::::::::::::::::::::::::::::::::::::

struct IncludePolicy: public virtual BaseClass {
    std::set<std::string> includes;

    void push(const std::string &s) {
        includes.insert(s);
    }

    const std::string action() {
        std::string r;
        std::set<std::string>::iterator it = includes.begin();
        std::set<std::string>::iterator it_end = includes.end();
        
        for (; it != it_end; ++it) {
            r.append("#include <").append(*it).append(">\n");
        }        
        return r;
    } 

};

// template policy ::::::::::::::::::::::::::::::::::::::::::::
//

struct TemplatePolicy : public virtual BaseClass {

    std::list<Element>  list;        

    void push(const Element &e) {
        list.push_back(e);                
    }

   const std::string action()
    {
        std::string r;

        std::list<Element>::iterator it =   list.begin();
        std::list<Element>::iterator it_end = list.end();

        r.append("template < ");

        for(;;) {

            r.append(it->get_type()).append(" ").append(it->get_id());
            if (!it->get_default().empty())
                r.append(" = ").append(it->get_default());

            if (++it == it_end)
                break;

            r.append(" , ");
        }

        r.append(" >");
        return r;
    }

};


// derivation policy :::::::::::::::::::::::::::::::::::::::::::: 
//

struct DerivationPolicy : public virtual BaseClass {

    std::list<Element>  list;        
     void push(const Element &e) {
        list.push_back(e);                
    }

   const std::string action()
    {
        std::string r;

        std::list<Element>::iterator it = list.begin();
        std::list<Element>::iterator it_end = list.end();

        r.append(" : ");

        for(;;) {

            r.append((*it).get_type()).append(" ").append((*it).get_id());
            ++it;

            if (it == it_end)
                break;

            r.append(" , ");
        }

        return r;
    }

};


// NamedConstructorIdiom policy :::::::::::::::::::::::::::::::::::::::::::: 
//

struct NamedConstructorIdiom : public virtual BaseClass {

    std::list<Element>  list;        
      void push(const Element &e) {
        list.push_back(e);                
    }

  const std::string action() 
    {
        std::string r;
        std::list<Element>::iterator it = list.begin();
        std::list<Element>::iterator it_end = list.end();

        r.append("\n");
        for(; it != it_end; ++it) {
            r.append("        static ").append(__name).append(" ").append((*it).get_id());
            r.append("() { return ").append(__name).append("(); }\n");
        }

        return r;
    } 

};


// NamedParameterIdiom policy :::::::::::::::::::::::::::::::::::::::::::: 
//

struct NamedParameterIdiom : public virtual BaseClass {

    std::list<Element>  list;        
     void push(const Element &e) {
        list.push_back(e);                
    }

   const std::string action() 
    {
        std::string r;
        std::list<Element>::iterator it = list.begin();
        std::list<Element>::iterator it_end = list.end();
    
        r.append("\n");
        for(; it != it_end; ++it) {
            r.append("        ").append(__name).append("& ").append((*it).get_id());
            r.append("() { ").append((*it).get_id()).append("_ = ");
            r.append((*it).get_default()).append("; return *this; }\n");
        }
    
        return r;
    } 

   const std::string action_private() 
    {
        std::string r;
        std::list<Element>::iterator it = list.begin();
        std::list<Element>::iterator it_end = list.end();

        r.append("\n");
        for(; it != it_end; ++it) {
            r.append("        ").append((*it).get_type()).append(" ").append((*it).get_id()).append("_ ;\n");
        }

        return r;
    } 

};


// PrintOnIdiom policy :::::::::::::::::::::::::::::::::::::::::::: 
//

struct PrintOnIdiom :  public virtual BaseClass {

    const std::string action() {

        std::string r;
        r.append("\n        friend std::ostream& operator<< (std::ostream& o, const ");
        r.append(__name).append("& b) { \n");
        r.append("            b.printOn(o);\n");
        r.append("            return o;\n");
        r.append("        }\n");  
        return r;
    }

};


// PonvPnovIdiom policy :::::::::::::::::::::::::::::::::::::::::::: 
//

struct PonvPnovIdiom : public virtual BaseClass {

    std::list<Element>  list;        
    void push(const Element &e) {
        list.push_back(e);                
    }

   const std::string action() 
    {
        std::string r;
        std::list<Element>::iterator it = list.begin();
        std::list<Element>::iterator it_end = list.end();

        r.append("\n");
        for(; it != it_end; ++it) {
            r.append("        void ").append(it->get_id()).append("(").append(it->get_type());
            r.append(" x) { ").append(it->get_id()).append("_").append(it->get_symbol());
            r.append("(x); }\n");
        }

        return r;
    } 

    const std::string action_protect() 
    {
        std::string r;
        std::list<Element>::iterator it = list.begin();
        std::list<Element>::iterator it_end = list.end();

        r.append("\n");
        for(; it != it_end; ++it) {
            r.append("        virtual void ").append(it->get_id()).append("_").append(it->get_symbol());
            r.append("(").append(it->get_type()).append(" x)=0;\n"); 
        }

        return r;
    } 

};


// ENTITY Class :::::::::::::::::::::::::::::::::::::::::::: 
//


class Class : public IncludePolicy, public PrintOnIdiom, public TemplatePolicy, public DerivationPolicy,
public NamedConstructorIdiom, public NamedParameterIdiom, public PonvPnovIdiom {

    private:

        static std::string __NAME;

        std::ostream *out;

        void guardName(const std::string &n) {
            __NAME.append("__").append(n);
            std::transform(__NAME.begin(), __NAME.end(), __NAME.begin(), (int(*)(int)) toupper);
        }
        
     
        std::string copyctor()
        {
             std::string r;
             r.append(__name).append("(").append(__name).append(" &);");
             return r;
        }
    
        std::string opequal()
        {
            std::string r;
            r.append(__name).append(" &operator=(const ").append(__name).append(" &);");
            return r;
        }
    
        std::string ctor(bool expl = true )
        {
            std::string r;
    
            if (__explicit && expl)
                r.append("explicit ");
    
            r.append(__name).append(" () { }");
            return r;
        }
    
        std::string dtor()
        {
            std::string r;
            r.append("/* virtual */ ~").append(__name).append(" () { }");
            return r;
        }
 
 
        Class(Class &);
        Class &operator=(const Class &);
   
    public:

        explicit Class (const std::string &n = "") 
            : BaseClass(n) {
                guardName(n);
            } 

        static std::string openGuard() {
            std::string ret; 
            ret.append("#ifndef ").append(__NAME).append("\n#define ").append(__NAME).append("\n\n");
            return ret;
        }
     
        static std::string closeGuard() {
            std::string ret;
            ret.append("#endif /* ").append(__NAME).append(" */\n"); 
            return ret;
        }

        /* virtual */ ~Class () { }

        friend std::ostream& operator<< (std::ostream& o, Class & b);  

};


std::string Class::__NAME;

std::list<Class *> theEntities;


//////////////////////////////////////////////////////////
//
//  Dump Class
//
//////////////////////////////////////////////////////////

std::ostream & operator<<(std::ostream& out, Class & ent) {

    ent.out = &out;

    // includes
    //
    if (ent.__include)
        out << ent.IncludePolicy::action() << std::endl;

    // template
    //

    if (ent.__template)
        out << ent.TemplatePolicy::action() << std::endl;

    // class//struct
    //

    out << ( ent.__explicit ? "class" : "struct" ) << " " << ent.__name;

    // derivation policy...
    //

    if ( ent.__derivation)
                out << ent.DerivationPolicy::action();

    out << " {\n";

    // private: -----------------------------------------------------
    //

    if (ent.__private)
        out << "\n    private:\n";

    // named parameter idiom: private element list...
    //

    if (ent.__npi) 
        out << "        " << ent.NamedParameterIdiom::action_private() << std::endl;

    // the big three
    //
    if (ent.__bigthree) {
        out << "        " << ent.copyctor() << std::endl;    
        out << "        " << ent.opequal () << std::endl;    
    }

    // named constructor idiom
    //

    if ( ent.__nci )  
        out << "        " << ent.ctor(false) << std::endl; 

    // protected: -----------------------------------------------------
    //

    if ( ent.__protected )
        out << "\n    protected:\n";


    if ( ent.__printOn) 
        out << "    virtual void printOn(std::ostream& o) "
               "const { o << __PRETTY_FUNCTION__ << std::endl; }\n";

    if ( ent.__ponvpnov)
        out << ent.PonvPnovIdiom::action_protect() << std::endl;


    // public: --------------------------------------------------------
    //
    
    if (ent.__public)
        out << "\n    public:\n";

    if (!ent.__nci)
        out << "        " << ent.ctor() << std::endl;
    else
        out << ent.NamedConstructorIdiom::action() << std::endl;

    // named parameter idiom
    //

    if (ent.__npi) 
        out << "        " << ent.NamedParameterIdiom::action() << std::endl;

    // descructor
    //

    out << "        " << ent.dtor() << std::endl;

    // printOn idiom..
    //

    if (ent.__printOn)
        out << ent.PrintOnIdiom::action() << std::endl;

    // Public Overl. Non-Virt. Call Protected Non-Overl. Virt.
    //
    if (ent.__ponvpnov)
        out << ent.PonvPnovIdiom::action() << std::endl;

    out << "\n};\n\n";   


    return out;
}


Class *notnull(Class *e) {

    if ( e == NULL )
        throw std::runtime_error("unknown class/structure name: -c/-s option must come first!");
    return e;

};


int 
main(int argc, char *argv[]) {

    int i;

    for (;argc>1;  optind = 0  ) {

        Class *x = NULL;

        while ((i = getopt(argc, argv, "+p:r:v:n:t:w:c:s:A:B:C:3Oh")) != EOF) {

          switch(i) {
            case 'c': x = new Class(optarg); x->_explicit()._public()._private();  break;
            case 's': x = new Class(optarg); break;   

            case 'n': notnull(x)->TemplatePolicy::push(Element::elementOpt(optarg)); 
                      x->_template(); break;     
            case 't': notnull(x)->TemplatePolicy::push(Element::elementOpt(optarg,"typename")); 
                      x->_template(); break;     
            case 'w': notnull(x)->TemplatePolicy::push(Element::elementOpt(optarg,"template <typename> class")); 
                      x->_template(); break; 
            case 'p': notnull(x)->DerivationPolicy::push(Element::elementOpt(optarg,"public"));    
                      x->_derivation() ;break;
            case 'r': notnull(x)->DerivationPolicy::push(Element::elementOpt(optarg,"protected")); 
                      x->_derivation(); break;
            case 'v': notnull(x)->DerivationPolicy::push(Element::elementOpt(optarg,"private"));   
                      x->_derivation(); break;

            // Idioms
            //

            case 'A': notnull(x)->NamedConstructorIdiom::push(Element::elementOpt(optarg)); x->_nci(); break;
            case 'B': notnull(x)->NamedParameterIdiom::push(Element::elementOpt(optarg));   x->_npi(); break; 
            case 'C': notnull(x)->PonvPnovIdiom::push(Element::elementOpt(optarg)); 
                      x->_ponvpnov()._protected(); break;

            case 'O': notnull(x)->IncludePolicy::push("iostream"); x->_printOn()._protected()._include() ; break;
            case '3': notnull(x)->_bigthree()._public()._private(); break; 

            case 'h': fprintf(stderr,usage,__progname); exit(0);
            case '?': throw std::runtime_error("unknown option");

                                 
            }

        }

        argc -= (optind-1);
        argv += (optind-1);

        theEntities.push_back(x);

    }

    std::list<Class *>::iterator it = theEntities.begin();
    std::list<Class *>::iterator it_end = theEntities.end();

    if (theEntities.empty())
        return 0;

    std::cout << Class::openGuard();

    for(; it != it_end ; ++it)
        std::cout << *(*it);

    std::cout << Class::closeGuard();

    return 0;
}
