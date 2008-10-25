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
#include "mutabl.hh"

struct base : public more::mutabl
{
    int a;
    base(int n)
    : a(n)
    {}

    virtual void hello() const = 0 ;
};

struct der1 : public base
{
    der1()
    : base(1) 
    {}

    virtual void hello() const
    {
        std::cout << "-> der1: value=" << a << std::endl;
    }
};

struct der2 : public base
{
    der2()
    : base(2)
    {}

    virtual void hello() const
    {
        std::cout << "-> der2: value=" << a << std::endl;
    }
};

struct xxx1 : public der1
{};

struct xxx2 : public der2
{};

void fun(const base *p)
{
    p->hello();
}

int
main(int argc, char *argv[])
{
    base * b = new der2;

    b->turn_into(der1());

    b->hello();

    b->turn_into(der2());

    fun(b);

    base *x = new xxx1;

    x->hello();
    x->turn_into( xxx2());
    x->hello();

    return 0;
}


