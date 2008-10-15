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

TYPEMAP_KEY(name);
TYPEMAP_KEY(nickname);
TYPEMAP_KEY(age);
TYPEMAP_KEY(size);

int
main(int argc, char *argv[])
{
    typedef TYPEMAP(name, std::string, age, int) mymap;

    mtp::TM::has_type<name, mymap>::type me = "Nicola";
    mtp::TM::has_type<age , mymap>::type a = 36;

    typedef mtp::TM::null empty;

    typedef mtp::TM::append<size,int, empty >::type  map_1;
    typedef mtp::TM::append<age, int, map_1>::type  map_2;
    // typedef mtp::TM::append<nickname, int, map_2>::type map_3;

    mtp::TM::has_type<size, map_1>::type x = 1;
    mtp::TM::has_type<size, map_2>::type c = 2;
    mtp::TM::has_type<age,  map_2>::type d= 3;

    std::cout << "size<map_1>::value  = " << mtp::TM::size<map_1>::value << std::endl;
    std::cout << "size<map_2>::value  = " << mtp::TM::size<map_2>::value << std::endl;

    std::cout << "indexof<size,map_2> = " << mtp::TM::indexof<size, map_2>::value << std::endl;
    std::cout << "indexof<age, map_2> = "  << mtp::TM::indexof<age, map_2>::value << std::endl;
    std::cout << "indexof<name,map_2> = " << mtp::TM::indexof<name, map_2>::value << std::endl;

    return 0;
}
 
