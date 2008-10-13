/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <typemap.hh>

char name[]     ="name";
char nickname[] ="nickname";
char age[]      ="age";
char size[]     ="size";

int
main(int argc, char *argv[])
{
    typedef TYPEMAP(name, std::string, age, int) mymap;

    mtp::TM::has_type<name, mymap>::type me = "Nicola";
    mtp::TM::has_type<age , mymap>::type a = 36;

    typedef mtp::TM::null empty;

    typedef mtp::TM::append<size,int, empty >::type  another_map;
    typedef mtp::TM::append<age, int, another_map>::type  another_map_2;
    // typedef mtp::TM::append<nickname, int, another_map_2>::type another_map_3;

    mtp::TM::has_type<size, another_map>::type x = 1;
    mtp::TM::has_type<size, another_map_2>::type c = 2;
    mtp::TM::has_type<age,  another_map_2>::type d= 3;

    std::cout << "size<another_map>::value = " << mtp::TM::size<another_map>::value << std::endl;
    std::cout << "size<another_map_2>::value = " << mtp::TM::size<another_map_2>::value << std::endl;

    std::cout << "indexof<size, another_map_2> = " << mtp::TM::indexof<size, another_map_2>::value << std::endl;
    std::cout << "indexof<age, another_map_2> = "  << mtp::TM::indexof<age, another_map_2>::value << std::endl;
    std::cout << "indexof<name, another_map_2> = " << mtp::TM::indexof<name, another_map_2>::value << std::endl;

    return 0;
}
 
