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

#include <getopt.h>
#include <errno.h>

extern char *__progname;
const char usage[]=
      "%s [options]: class1 opt... -- class2 opt... -- classn opt...\n"
     "\n Class:\n"
      "   -c class            class  name\n"
      "   -s struct           struct name\n"
      "\n Polymorphism:\n"                                            
      "   -p name -p name ... public    derivation\n"
      "   -r name -p name ... protected derivation\n"
      "   -v name -p name ... private   derivation\n"
      "\n Template:\n"
      "   -N elem -N elem ... template non-type\n"
      "   -T name ...         template parameter\n"
      "   -V name ...         template template parameter\n"
      "\n Idioms:\n"
      "   -n elem -n elem ... named-constructor idiom\n"
      "   -a elem -a elem ... named-parameter idiom\n"
      "   -k elem -k elem ... public overloaded non-virtuals call\n"
      "                       protect non-overloaded virtual idiom\n"
      "   -o                  virtual-friend-function idiom (printOn)\n"            
      "   -3                  big-three guideline\n"
      "   -h                  print this help.\n"
      "\n Element: id,<type>,<default> examples: \n\n"
      "   'test' 'home,int' 'point,char *, NULL'\n";


// basic element
//

class Element {

    friend class Entity;

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



// base policy     ::::::::::::::::::::::::::::::::::::::::::::
//
struct BasePolicy {

    std::list<Element>  list;        

    void push(const Element &e) {
        list.push_back(e);                
    }

};

// template policy ::::::::::::::::::::::::::::::::::::::::::::
//

struct TemplatePolicy : public BasePolicy {

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

struct DerivationPolicy : public BasePolicy {

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

struct NamedConstructorIdiom : public BasePolicy {

    const std::string action(const std::string &name) 
    {
        std::string r;
        std::list<Element>::iterator it = list.begin();
        std::list<Element>::iterator it_end = list.end();

        r.append("\n");
        for(; it != it_end; ++it) {
            r.append("        static ").append(name).append(" ").append((*it).get_id());
            r.append("() { return ").append(name).append("(); }\n");
        }

        return r;
    } 

};


// NamedParameterIdiom policy :::::::::::::::::::::::::::::::::::::::::::: 
//

struct NamedParameterIdiom : public BasePolicy {

    const std::string action(const std::string &name) 
    {
        std::string r;
        std::list<Element>::iterator it = list.begin();
        std::list<Element>::iterator it_end = list.end();
    
        r.append("\n");
        for(; it != it_end; ++it) {
            r.append("        ").append(name).append("& ").append((*it).get_id());
            r.append("() { ").append((*it).get_id()).append("_ = ");
            r.append((*it).get_default()).append("; return *this; }\n");
        }
    
        return r;
    } 

   const std::string action_private(const std::string &name) 
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

struct PrintOnIdiom {

    const std::string action(const std::string &name) {

        std::string r;
        r.append("\n        friend std::ostream& operator<< (std::ostream& o, const ");
        r.append(name).append("& b) { \n");
        r.append("            b.printOn(o);\n");
        r.append("            return o;\n");
        r.append("        }\n");  
        return r;
    }

};


// PonvPnovIdiom policy :::::::::::::::::::::::::::::::::::::::::::: 
//

struct PonvPnovIdiom : public BasePolicy {

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


//
// ENTITY Class :::::::::::::::::::::::::::::::::::::::::::: 
//


class Entity : public PrintOnIdiom, public TemplatePolicy, public DerivationPolicy,
public NamedConstructorIdiom, public NamedParameterIdiom, public PonvPnovIdiom {

    private:
        static std::string __NAME;

    private:

        std::ostream *out;
        std::string __name;

        bool __private ;
        bool __protected ;
        bool __public ;
        bool __template;
        bool __derivation;

        // idioms
        //

        bool __bigthree ;
        bool __printOn ;
        bool __explicit ;
        bool __ponvpnov;
        bool __nci ;
        bool __npi ;

        Entity(Entity &);
        Entity &operator=(const Entity &);

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
    
    public:

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

        explicit Entity (const std::string &n = "") 
            : __name(n), __private(false), __protected(false), __public(false),
            __template(false), __derivation(false),__bigthree(false), __printOn(false), __explicit(false), 
            __ponvpnov(false),__nci(false), __npi(false) { 

                guardName(n);
            } 

        Entity& _name(const std::string &n) { 
            __name = n;
            guardName(n);
            return *this;
        }

        Entity& _private()     { __private = true; return *this; }
        Entity& _protected()   { __protected = true; return *this; }
        Entity& _public()      { __public = true; return *this; }
        Entity& _template()    { __template = true; return *this; }
        Entity& _derivation()  { __derivation = true; return *this; }
        Entity& _bigthree()    { __bigthree = true; return *this; }
        Entity& _nci()         { __nci = true; return *this; }
        Entity& _npi()         { __npi = true; return *this; }
        Entity& _printOn()     { __printOn = true; return *this; }
        Entity& _explicit()    { __explicit = true; return *this; }
        Entity& _ponvpnov()    { __ponvpnov = true; return *this; }
        
        /* virtual */ ~Entity () { }

        friend std::ostream& operator<< (std::ostream& o, Entity & b);  

};


std::string Entity::__NAME;

std::list<Entity *> theEntities;


//////////////////////////////////////////////////////////
//
//  Dump Entity
//
//////////////////////////////////////////////////////////

std::ostream & operator<<(std::ostream& out, Entity & ent) {

    ent.out = &out;

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
        out << "        " << ent.NamedParameterIdiom::action_private(ent.__name) << std::endl;

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
        out << ent.NamedConstructorIdiom::action(ent.__name) << std::endl;

    // named parameter idiom
    //

    if (ent.__npi) 
        out << "        " << ent.NamedParameterIdiom::action(ent.__name) << std::endl;

    // descructor
    //

    out << "        " << ent.dtor() << std::endl;

    // printOn idiom..
    //

    if (ent.__printOn)
        out << ent.PrintOnIdiom::action(ent.__name) << std::endl;

    // Public Overl. Non-Virt. Call Protected Non-Overl. Virt.
    //
    if (ent.__ponvpnov)
        out << ent.PonvPnovIdiom::action() << std::endl;

    out << "\n};\n\n";   


    return out;
}


Entity *notnull(Entity *e) {

    if ( e == NULL )
        throw std::runtime_error("unknown class/structure name: -c/-s option must come first!");
    return e;

};


int 
main(int argc, char *argv[]) {

    int i;

    for (;argc>1;  optind = 0  ) {

        Entity *x = NULL;

        while ((i = getopt(argc, argv, "+N:T:V:p:r:v:n:c:s:k:a:3oh")) != EOF) {

          switch(i) {
            case 'c': x = new Entity(optarg); x->_explicit()._public()._private();  break;
            case 's': x = new Entity(optarg); break;   
            case '3': notnull(x)->_bigthree()._public()._private(); break; 
            case 'o': notnull(x)->_printOn()._protected() ; break;

            case 'N': notnull(x)->_template().TemplatePolicy::push(Element::elementOpt(optarg)); break;     
            case 'T': notnull(x)->_template().TemplatePolicy::push(Element::elementOpt(optarg,"typename")); break;     
            case 'V': notnull(x)->_template().TemplatePolicy::push(Element::elementOpt(optarg,"template <typename> class")); break; 

            case 'p': notnull(x)->_derivation().DerivationPolicy::push(Element::elementOpt(optarg,"public"));    break;
            case 'r': notnull(x)->_derivation().DerivationPolicy::push(Element::elementOpt(optarg,"protected")); break;
            case 'v': notnull(x)->_derivation().DerivationPolicy::push(Element::elementOpt(optarg,"private"));   break;

            case 'n': notnull(x)->_nci().NamedConstructorIdiom::push(Element::elementOpt(optarg)); break;
            case 'a': notnull(x)->_npi().NamedParameterIdiom::push(Element::elementOpt(optarg)); break; 

            case 'k': notnull(x)->_ponvpnov()._protected().PonvPnovIdiom::push(Element::elementOpt(optarg)); break;

            case 'h': fprintf(stderr,usage,__progname); exit(0);
            case '?': throw std::runtime_error("unknown option");

                                 
            }

        }

        argc -= (optind-1);
        argv += (optind-1);

        theEntities.push_back(x);

    }

    std::list<Entity *>::iterator it = theEntities.begin();
    std::list<Entity *>::iterator it_end = theEntities.end();

    if (theEntities.empty())
        return 0;

    std::cout << Entity::openGuard();

    for(; it != it_end ; ++it)
        std::cout << *(*it);

    std::cout << Entity::closeGuard();

    return 0;
}
