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

class hello : public generic::singleton<hello> {

public:

 // hello(const generic::singleton<hello> &abc) : generic::singleton<hello>(abc) {}

    SINGLETON_CTOR(hello) {}

};

int 
main(int argc, char *argv[])
{
    hello &ref  = hello::instance();

    hello &mul0 = hello::instance<0>(); // multiton: slot 0
    hello &mul1 = hello::instance<1>(); // multiton: slot 1

    std::cout << "instance @" << &ref  << '\n';
    std::cout << "slot0    @" << &mul0 << '\n';
    std::cout << "slot1    @" << &mul1 << '\n';

    // hello a;  <- any other instance of hello is not allowed 
}
