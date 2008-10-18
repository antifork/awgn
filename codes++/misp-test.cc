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

typedef TYPEMAP(integer, unsigned int,
                str,     std::string,
                ip,      std::string) MYSCRIPT;

struct myscript : public more::misp<MYSCRIPT>
{};

int
main(int argc, char *argv[])
{
    myscript abc;

    if ( !abc.parse("./misp-test.txt") ) {
        return -1;
    }

    std::cout << "-> " << str::value() << "    :" << '\'' << abc.get<str>()  << '\'' << std::endl;
    std::cout << "-> " << ip::value() << "     :" << '\'' << abc.get<ip>()   << '\'' << std::endl;
    std::cout << "-> " << integer::value() << ":" << abc.get<integer>() << std::endl;

    return 0;
}

