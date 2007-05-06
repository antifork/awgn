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
      "%s [options]:\n"
      "\n Class:\n"
      "   -c class            class  name\n"
      "   -s struct           struct name\n"
      "\n Polymorphism:\n"                                            
      "   -p name -p name ... public    derivation\n"
      "   -r name -p name ... protected derivation\n"
      "   -v name -p name ... private   derivation\n"
      "\n Template:\n"
      "   -N 'type,name' -N.. template non-type\n"
      "   -V name -V name ... template parameter\n"
      "   -W name -W name ... template template parameter\n"
      "\n Idioms:\n"
      "   -n name -n name ... named-constructor idiom\n"
      "   -a name -a name ... named-parameter idiom\n"
      "   -o                  virtual-friend-function idiom (printOn)\n"            
      "   -3                  big-three guideline\n"
      "   -h                  print this help.\n";

char *__classname  = NULL;
char *__keyword    = NULL;

int  __private;
int  __protected;
int  __public;

int  __bigthree;
int  __nci;             // named-ctor-idiom
int  __npi;             // named-param-idiom
int  __printOn;
int  __explicit;

struct base {
    std::string name;
    std::string relation; 
    base(const std::string &n, const std::string &r) : name(n), relation(r) { }
};

struct generic {
    std::string type;
    std::string name;
    generic(const std::string &t, const std::string &n) : type(t), name(n) { } 
};

std::list<base>        derivation_list;
std::list<generic>     template_list;
std::list<std::string> nci_list;
std::list<std::string> npi_list;

namespace freefunction {

    __attribute__((noreturn))
        void throw_nocatch(const std::string &txt) throw() {
            std::string e(txt);
            if (errno)
                throw std::runtime_error(e.append(": ").append(strerror(errno)));
            else
                throw std::runtime_error(e);
        }


    const std::string copyctor(const std::string &n)
    {
        std::string r;
        r.append(n).append("(").append(n).append(" &);");
        return r;
    }


    const std::string opequal(const std::string &n)
    {
        std::string r;
        r.append(n).append(" &operator=(const ").append(n).append(" &);");
        return r;
    }


    const std::string ctor(const std::string &n, bool expl = true )
    {
        std::string r;

        if (__explicit && expl)
            r.append("explicit ");

        r.append(n).append(" () { }");
        return r;
    }


    const std::string dtor(const std::string &n)
    {
        std::string r;
        r.append("/* virtual */ ~").append(n).append(" () { }");
        return r;
    }


    const std::string derivate()
    {
        std::string r;

        std::list<base>::iterator it = derivation_list.begin();
        std::list<base>::iterator it_end = derivation_list.end();

        r.append(" : ");

        for(;;) {

            r.append((*it).relation).append(" ").append((*it).name);
            ++it;

            if (it == it_end)
                break;

            r.append(" , ");
        }


        return r;
    }


    const std::string template_format()
    {
        std::string r;

        std::list<generic>::iterator it =   template_list.begin();
        std::list<generic>::iterator it_end = template_list.end();

        r.append("template < ");

        for(;;) {

            r.append(it->type).append(" ").append(it->name);
            ++it;

            if (it == it_end)
                break;

            r.append(" , ");
        }

        r.append(" >");

        return r;
    }

    const std::string named_ctor_idiom(const std::string &name) 
    {
        std::string r;
        std::list<std::string>::iterator it = nci_list.begin();
        std::list<std::string>::iterator it_end = nci_list.end();

        r.append("\n");
        for(; it != it_end; ++it) {
            r.append("        static ").append(name).append(" ").append(*it);
            r.append("() { return ").append(name).append("(); }\n");
        }

        return r;
    } 

    const std::string named_param_idiom(const std::string &name) 
    {
        std::string r;
        std::list<std::string>::iterator it = npi_list.begin();
        std::list<std::string>::iterator it_end = npi_list.end();

        r.append("\n");
        for(; it != it_end; ++it) {
            r.append("        ").append(name).append("& ").append(*it);
            r.append("() { ").append(*it).append("_ = true; return *this; }\n");
        }

        return r;
    } 


    const std::string printOn_idiom(const std::string &name) {

        std::string r;
        r.append("\n        friend std::ostream& operator<< (std::ostream& o, const ");
        r.append(name).append("& b) { \n");
        r.append("            b.printOn(o);\n");
        r.append("            return o;\n");
        r.append("        }\n");  
        return r;
    }    
}


class Header {

    std::string guardname;

    public:
    Header() : guardname(__classname) {

        std::transform(guardname.begin(), guardname.end(), guardname.begin(), (int(*)(int)) toupper);
        guardname.append("_HH");

        std::cout << "#ifndef " << guardname << "\n"          
            << "#define " << guardname << "\n\n";         
    }

    ~Header() {
        std::cout << "#endif /* " << guardname << " */" << std::endl;
    }    

};


class Body : public Header {

    public:
        Body() : Header() { 

            // template
            //

            if (!template_list.empty())
                std::cout << freefunction::template_format() << std::endl;

            // class
            //

            std::cout << __keyword << " " << __classname;

            // derivation
            //
            if (!derivation_list.empty())
                std::cout << freefunction::derivate();

            std::cout << " {\n";
            if (__keyword == "class" || ( __keyword == "struct" && __bigthree) )
                std::cout << "\n    private:\n";

            // the big three
            //
            if (__bigthree) {
                std::cout << "        " << freefunction::copyctor(__classname) << std::endl;    
                std::cout << "        " << freefunction::opequal (__classname) << std::endl;    
            }

            if ( __nci )  {  // named constructor idiom
                std::cout << "        " << freefunction::ctor(__classname, false) << std::endl; 
            }

            // PROTECTED: -----------------------------------------------------
            //
            if ( __protected )
                std::cout << "\n    protected:\n";

            if ( __printOn) 
                std::cout << 
                    "    virtual void printOn(std::ostream& o) const { o << __PRETTY_FUNCTION__ << std::endl; }\n";

            // PUBLIC: --------------------------------------------------------
            //
            if (__public)
                std::cout << "\n    public:\n";

            if (!__nci)
                std::cout << "        " << freefunction::ctor(__classname) << std::endl;
            else
                std::cout << freefunction::named_ctor_idiom(__classname) << std::endl;

            if (__npi) // named-parameter-idiom
                std::cout << "        " << freefunction::named_param_idiom(__classname) << std::endl;

            std::cout << "        " << freefunction::dtor(__classname) << std::endl;

            if (__printOn)
                std::cout << freefunction::printOn_idiom(__classname) << std::endl;

            std::cout << "};\n\n";          
        }        
};

int 
main(int argc, char *argv[]) {

    int i;

    while ((i = getopt(argc, argv, ":N:V:W:p:r:v:n:c:s:a:3oh")) != EOF)
        switch (i) {
            case '3': __bigthree  = 1; __public = 1; break; 
            case 's': __classname = optarg; __keyword = "struct"; break;   
            case 'c': __classname = optarg; __keyword = "class" ; __explicit = 1; __public = 1; break;

            case 'p': derivation_list.push_back(base(optarg,"public"));    break;
            case 'r': derivation_list.push_back(base(optarg,"protected")); break;
            case 'v': derivation_list.push_back(base(optarg,"private"));   break;
            case 'n': __nci = 1; nci_list.push_back(std::string(optarg)); break;
            case 'a': __npi = 1; npi_list.push_back(std::string(optarg)); break; 
            case 'o': __printOn = 1; __protected = 1 ; break;
            case 'N': { 
                          char * name = strchr(optarg,',');
                          if ( name == NULL )
                              freefunction::throw_nocatch("invalid template non-type format: 'type , name'");
                          *name++ = '\0';        
                          template_list.push_back(generic(optarg,name));
                      } break;

            case 'V': template_list.push_back(generic("typename", optarg)); break;
            case 'W': template_list.push_back(generic("template <typename> class", optarg)); break; 

            case 'h': fprintf(stderr,usage,__progname); exit(0);
        }

    argc -= optind;
    argv += optind;

    if (__classname == NULL)
        freefunction::throw_nocatch("-c/-s class/struct name missing!");

    Body generator;

    return 0;
}
