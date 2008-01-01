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

    int par;
    int par2;

 // hello(const generic::singleton<hello> &abc) : generic::singleton<hello>(abc) {}

    SINGLETON_CTOR(hello) {}
    MULTITON_CTOR(hello, int _par) , par(_par) {}
    MULTITON_CTOR(hello, int _par, int _par2) , par(_par), par2(_par2) {}
};

int 
main(int argc, char *argv[])
{
    hello &ref  = hello::instance();

    hello &mul0 = hello::instance<0>(0); // multiton: slot 0
    hello &mul1 = hello::instance<1>(1); // multiton: slot 1

    std::cout << "\nsingleton instance...\n";
    std::cout << "instance @" << &ref  << '\n';

    std::cout << "\n2 multitons: slot0 and slot1\n";
    std::cout << "slot0    @" << &mul0 << " par:" << mul0.par << '\n';
    std::cout << "slot1    @" << &mul1 << " par:" << mul1.par << '\n';

    std::cout << "\naccesing to multiton slot0...\n";
    std::cout << "slot0 param:" << hello::instance<0,int>().par << std::endl;

    std::cout << "\nmultiton with 2-parameters ctor..\n";
    hello &mulB = hello::instance<0>(1,2);

    std::cout << "instance @" << &mulB << " par1:" << mulB.par << " par2:" << mulB.par2 << '\n';
    // hello a;  <- any other instance of hello is not allowed 
}
