/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef SINGLETON_HH
#define SINGLETON_HH

#include <stdexcept>
#include <cstdlib>

#include "memory-policy.hh"
#include "atomicity-policy.hh"
#include "generic.hh"

// my generic singleton
//

namespace generic {

    template <class T, template <class> class Memory, class Atomicity = atomicity::None >
    class singleton {

        singleton(const singleton &);
        singleton &operator=(const singleton &);

        protected:
        singleton() { }

        public:
        virtual ~singleton() { instance(false); }

        static T &instance(bool action=true) {
            static typename Atomicity::mutex M_Lock;
            static volatile bool destroyed = false;

            static T *__instance;

            if ( action == false ) {
                T * ret = __instance;
                destroyed = true;
                __instance = NULL; 
                return * (new(ret) T);      
            }

            if ( !__instance ) {
                typename Atomicity::scoped_lock mutex(M_Lock);
                if ( !__instance ) {
                    if (destroyed) { 
                        throw std::runtime_error("singleton: static order FIASCO problem!");    
                    }   
                    __instance = Memory<T>::alloc();
                }

            }
            return *__instance;
        }
    };
}

#endif /* SINGLETON_HH */
