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
#include <string>
#include <factory.hh>

class Base {
    public:
    virtual ~Base() {};

    virtual Base &alloc()=0;                // to call the default ctor
    virtual Base &alloc(const int &)=0;     // to call the ctor(const int &) 
    virtual void dealloc(Base &)=0;         // to free the allocated object

    virtual void whoami() { std::cout << __PRETTY_FUNCTION__ << std::endl;}

};


class One : public generic::factoryElement<One, int, Base, memory::New> {

    public:
        One() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        One(const int &) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        ~One() { std::cout << __PRETTY_FUNCTION__ << std::endl; }

        void whoami() { std::cout << __PRETTY_FUNCTION__ << std::endl;}

};

class Two : public generic::factoryElement<Two, int, Base, memory::Static> {

    public:
        Two() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        Two(const int &) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        ~Two() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        
        void whoami() { std::cout << __PRETTY_FUNCTION__ << std::endl;}
};

class Three : public generic::factoryElement<Three, int, Base, memory::Malloc> {

    public:
        Three() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        Three(const int &) { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        ~Three() { std::cout << __PRETTY_FUNCTION__ << std::endl; }

        void whoami() { std::cout << __PRETTY_FUNCTION__ << std::endl;}
};


int main()
{
    // define the factory, give it a name...

 // typedef generic::factory<std::string, Base  > myFactory; BAD! (no polymorphism!)
    typedef generic::factory<std::string, Base *> myFactory;

    // ... and instance it.
    myFactory factory;

    std::cout << "::::: registering producers...\n";

    // register elements/producers into your factory...

    factory.regist("one",   new One);
    factory.regist("two",   new Two);
    factory.regist("three", new Three);

    std::cout << "\n::::: building objects with the factory...\n";

    // construct elements by name...
    //
    Base &ref1 = factory("one",1);
    Base &ref2 = factory("two");
    Base &ref3 = factory("three");

    std::cout << "\n::::: who are you?\n";

    ref1.whoami();
    ref2.whoami();
    ref3.whoami();

    std::cout << "\n::::: freeing objects\n";

    // and delete them once out of scope 
    //
    factory.dealloc(ref1);
    factory.dealloc(ref2);
    factory.dealloc(ref3);

    std::cout << "\n::::: destroying the factory...\n";
}
