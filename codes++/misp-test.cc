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

TYPEMAP_KEY(integer);
TYPEMAP_KEY(str);
TYPEMAP_KEY(ip);

typedef TYPEMAP(integer, int,
                str,     std::string,
                ip,      std::string) MYSCRIPT;

struct myscript : public more::misp<MYSCRIPT>
{
    virtual bool grammar(const std::string &key, const std::string &value)
    {
        if ( key == str::value() ) {
            this->get<str>() = value;
            return true;
        }

        if ( key == ip::value() ) {
            this->get<ip>() = value;
            return true;
        }

        if ( key == integer::value() ) {
            int num = atoi(value.c_str());
            this->get<integer>() = num;
            return true;
        }

        return false;
    }

};


int
main(int argc, char *argv[])
{
    myscript abc;

    if ( !abc.parse("./misp-test.txt") ) {
        return -1;
    }

    std::cout << "-> " << str::value()     << ":" << '"' << abc.get<str>()  << '"' << std::endl;
    std::cout << "-> " << ip::value()      << ":" << '"' << abc.get<ip>()   << '"' << std::endl;
    std::cout << "-> " << integer::value() << ":" <<     abc.get<integer>() << std::endl;

    return 0;
}

