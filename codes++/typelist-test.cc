/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include "typelist.hh"
#include <iostream>

using namespace generic;

int main()
{
    typedef TYPELIST(int) z_list; 
    std::cout << "len:" << TL::length<z_list>::value << std::endl;

    TL::at<z_list,0>::type x1 = 1;
    std::cout << "int:" << x1 << std::endl;

    // append...

    typedef null a_list;

    typedef TL::append<a_list, bool>::type b_list;

    TL::at<b_list,0>::type x2 = true;
    std::cout << "bool:" << x2 << std::endl;

    typedef TL::append<b_list,int>::type c_list;
    TL::at<c_list,1>::type x3 = 2;
    std::cout << "int:" << x3 << std::endl;

    typedef TL::append<c_list, c_list>::type d_list;

    std::cout << "len:" << TL::length<d_list>::value << std::endl;

    typedef TYPELIST(null) e_list;
    typedef TL::insert<e_list, int>::type f_list;

    TL::at<f_list,0>::type x4 = 1;
    std::cout << "int:" << x4 << std::endl;

    typedef TL::insert<f_list, std::string>::type g_list;
    TL::at<g_list, 0>::type h ("hello world");

    std::cout << "string:" << h << std::endl;

    // indexof
    std::cout << "double:" << TL::indexof<g_list, double>::value << std::endl;
    std::cout << "string:" << TL::indexof<g_list, std::string>::value << std::endl;
    std::cout << "int   :" << TL::indexof<g_list, int>::value << std::endl;

}
