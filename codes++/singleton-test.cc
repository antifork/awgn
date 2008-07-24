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
#include <singleton.hh>

struct hello1: public generic::singleton<hello1> 
{
    SINGLETON_CTOR(hello1) {}
};

class hello2 : public generic::singleton<hello2> 
{
    int __par1;
    int __par2;

public:
 // hello(const VOID &abc) : generic::singleton<hello>(abc) {}
    SINGLETON_CTOR(hello2), __par1(0), __par2(1) {}

    const int par1() const volatile
    { return __par1; }

    const int par2() const volatile
    { return __par2; }
};

class hello3 : public generic::singleton<hello3> {

    int __par1;
    int __par2;

public:
    MULTITON_CTOR(hello3, int _par1, int _par2), __par1(_par1), __par2(_par2) {}

    const int par1() const volatile
    { return __par1; }

    const int par2() const volatile
    { return __par2; }
};


int 
main(int argc, char *argv[])
{
    volatile hello1 &ref1 = hello1::instance();
    volatile hello2 &ref2 = hello2::instance();

    std::cout << "\nsingleton instance (via volatile reference)...\n";
    std::cout << "\nnote: volatile is a contract. The class writer must honour it,\n";
    std::cout << "      by writing volatile methods with must be thread-safe.\n\n";

    std::cout << "instance @" << &ref1  << '\n';
    std::cout << "instance @" << &ref2  << '\n';

    volatile hello3 &mul0 = hello3::instance<0>(0,1); // multiton: slot 0
    volatile hello3 &mul1 = hello3::instance<1>(1,2); // multiton: slot 1

    std::cout << "\n2 multitons: slot0 and slot1\n";
    std::cout << "slot0    @" << &mul0 << " par:" << mul0.par1() << '\n';
    std::cout << "slot1    @" << &mul1 << " par:" << mul1.par1() << '\n';

    std::cout << "\ngetting a reference to multiton slot0...\n";
    std::cout << "slot0 param:" << hello3::instance<0,int, int>().par1() << std::endl;

    std::cout << "\nmultiton with 2-parameters ctor..\n";
    volatile hello3 &mulB = hello3::instance<0>(1,2);

    std::cout << "instance @" << &mulB << " par1:" << mulB.par1() << " par2:" << mulB.par2() << '\n';

    // hello1 a;  // <- instances of singleton are not allowed, and non copyable. 

}
