/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include <iostream>
#include <pthread.h>

// #include <boost/thread.hpp>  uncommment to use lock of boost (atomicity::Boost)
#include "singleton.hh"

class animal : public generic::singleton<animal,memory::Static, atomicity::GNU_CXX > {
    public:
        animal()  { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        ~animal() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        void hello() { std::cout << "  " << __PRETTY_FUNCTION__ << std::endl; }
};

class human : public generic::singleton<human,memory::Static> {
    public:
        human()  { std::cout << __PRETTY_FUNCTION__ << std::endl; } 
        ~human() { 
            std::cout << __PRETTY_FUNCTION__ << std::endl; 
            // animal & a __attribute__((unused)) = animal::instance();   // uncomment to trigger the order-fiasco problem ..
        }
        void hello() { std::cout << "  " << __PRETTY_FUNCTION__ << std::endl; }
};

class immortal : public generic::singleton<immortal,memory::Malloc> {
    public:
        immortal()  { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        ~immortal() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        void hello() { std::cout << "  " << __PRETTY_FUNCTION__ << std::endl; }
};

human &s = human::instance();
animal &a __attribute__((unused)) = animal::instance();

void *thread(void *) {

    human &man = human::instance();
    immortal &divinity = immortal::instance();

    man.hello();
    divinity.hello();

    return 0;
}


int main()
{
    pthread_t a,b,c,d;

    // human x; // <- detect errors like this at runtime

    pthread_create(&a,NULL,thread,NULL);
    pthread_create(&b,NULL,thread,NULL);
    pthread_create(&c,NULL,thread,NULL);
    pthread_create(&d,NULL,thread,NULL);

    pthread_join(a,NULL);
    pthread_join(b,NULL);
    pthread_join(c,NULL);
    pthread_join(d,NULL);

    return 0;
}

