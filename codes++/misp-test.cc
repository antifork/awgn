/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <cstdlib>
#include <misp.hh>

char k_integer[]  ="integer";
char k_string[]   ="string";
char k_ip[]       ="ip";

typedef TYPEMAP(k_integer, int,
                k_string,  std::string,
                k_ip,      std::string) MYSCRIPT;

struct myscript : public more::misp<MYSCRIPT>
{
    virtual bool grammar(const std::string &key, const std::string &value)
    {
        if ( key == k_string ) {
            this->set<k_string>(value);
            return true;
        }

        if ( key == k_ip ) {
            this->set<k_ip>(value);
            return true;
        }

        if ( key == k_integer ) {
            int num = atoi(value.c_str());
            this->set<k_integer>(num);
            return true;
        }

        return false;
    }

};


int
main(int argc, char *argv[])
{
    myscript abc;

    if ( !abc.parse("./test") ) {
        return -1;
    }

    std::cout << k_string  << ":" << '"' << abc.get<k_string>()  << '"' << std::endl;
    std::cout << k_ip      << ":" << '"' << abc.get<k_ip>()      << '"' << std::endl;
    std::cout << k_integer << ":" <<        abc.get<k_integer>() << std::endl;

    return 0;
}

