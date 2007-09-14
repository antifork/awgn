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

#include "factory.hh"
#include "singleton.hh"

class ElementBase {

    public:
        virtual ~ElementBase(){ };
        virtual ElementBase &alloc()=0; // REQUIRED

        virtual void whoami() { std::cout << __PRETTY_FUNCTION__ << std::endl;}
};


class Book : public generic::factoryElement<Book, ElementBase, memory::New> {
    public:
        Book() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        ~Book() { std::cout << __PRETTY_FUNCTION__ << std::endl;}

        virtual void whoami() { std::cout << __PRETTY_FUNCTION__ << std::endl;}
};


class Journal : public generic::factoryElement<Journal, ElementBase, memory::Static> {
    public:
        Journal() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        ~Journal() { std::cout << __PRETTY_FUNCTION__ << std::endl;}
        virtual void whoami() { std::cout << __PRETTY_FUNCTION__ << std::endl;}
};


int main()
{
    // 1a) define the factory, give it a name...
    //

    typedef generic::factory<std::string, ElementBase *> myFactory;     // GOOD factory
//  typedef generic::factory<std::string, ElementBase  > myFactory;     // BAD  factory (no polymorphism!)

    // ... and instance it.

    myFactory factory;

    // 1b) or define a singleton-factory, give it a name...
    //

    typedef generic::singleton<myFactory, memory::Static> mySingleFactory;
        
    // and get a reference.
    //

    myFactory & rfactory = mySingleFactory::instance();

    // 2) register elements to your factory.. 
    //

    factory.regist  ("book",    new Book); 
    factory.regist  ("journal", new Journal);  

    rfactory.regist ("book",    new Book);
    rfactory.regist ("journal", new Journal);

    // NOT TODO: do not delete elements after their registration, the ~factory() will do it for you...
    // ... and don't blame me because std::auto_ptr are not being used -- they are not STL compliant.

    std::cout << "*** factory ready ***\n";

    ElementBase &ref1 = factory("book");
    ElementBase &ref2 = factory("book");

    ElementBase &ref3 = rfactory("journal");
    ElementBase &ref4 = rfactory("journal");

    std::cout << std::hex << "memory::New    -> " << &ref1 << " " << &ref2 << std::endl;
    std::cout << std::hex << "memory::Static -> " << &ref3 << " " << &ref4 << std::endl;

}
