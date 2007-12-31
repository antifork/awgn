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
#include "meyers-singleton.hh"

class SINGLETON(hello) {

    SINGLETON_FRIEND(hello);    // <- mandatory
    hello() {};                 // <- private constructor

public:


};

int 
main(int argc, char *argv[])
{
    hello &ref  = hello::instance();

    hello &mul0 = hello::instance<0>(); // multiton: slot 0
    hello &mul1 = hello::instance<1>(); // multiton: slot 1

    std::cout << &ref  << '\n';
    std::cout << &mul0 << '\n';
    std::cout << &mul1 << '\n';

    // hello a;
}
