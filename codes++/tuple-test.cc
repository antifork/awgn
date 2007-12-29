/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <iostream>
#include "tuple.hh"

using namespace generic;

int main(int argc, char *argv[])
{
    tuple<TLIST_3(int,int,std::string)> hello;    

    hello.get<0>() = 0;
    hello.get<1>() = 1;
    hello.get<2>() = "2 - hello world";

    std::cout << hello.get<0>() << std::endl;
    std::cout << hello.get<1>() << std::endl;
    std::cout << hello.get<2>() << std::endl;

    tuple<TLIST_1(int)> test = make_tuple(3);
    std::cout <<  test.get<0>() << std::endl;    

    tuple<TLIST_2(int,std::string)> abc = make_tuple(4,std::string("5 - std::string"));
    std::cout << abc.get<0>() << std::endl;
    std::cout << abc.get<1>() << std::endl;

    return 0;
}
