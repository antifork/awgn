/* $Id$ */
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
#include <string>
#include <typeinfo>
#include <cxxabi.h>

#include "memory-policy.hh"
#include "atomicity-policy.hh"
#include "generic.hh"

// my generic singleton, inspired to that of Alexandrescu
//

namespace generic {

    static std::string demangle(const char *name) {
        int status;
        char *realname = abi::__cxa_demangle(name, 0, 0, &status);
        std::string ret(realname);
        ::free(realname);
        return ret;
    }

    template <class T, template <class, class T = int> class Memory, class Atomicity = atomicity::NONE >
    class singleton {

        singleton(const singleton &);                   // noncopyable
        singleton &operator=(const singleton &);        // noncopyable

        enum Tristate {Uninitialized=-1,False=false,True=true};

        static Tristate _M_destroyed(Tristate action=Uninitialized) 
        {
            static Tristate ret = False;
            if (action == False)
                ret = False;
            else 
                if (action == True)
                    ret = True;
            return ret; 
        }

        protected:

        virtual ~singleton() {
            _M_destroyed(True); 
            instance(false); 
        }

        singleton() {
            if ( _M_destroyed() == False ) {
                const std::type_info  &ti = typeid(T);
                std::string cl = generic::demangle(ti.name());
                throw std::runtime_error(
                        std::string("singleton: built instance of '").
                        append(cl).
                        append("' (use ").
                        append(cl).
                        append("::instance() instead!)"));
            }
        }

        public:

        static T &instance(bool action=true) {

            static typename Atomicity::mutex mutex;
            static T *_S_instance;
            
            if ( !_S_instance || !action ) {

                typename Atomicity::scoped_lock lock(mutex);

                if (!action) {
                    T *ret = _S_instance;
                    _S_instance = NULL;
                    return *ret;
                }

                if (!_S_instance)  {

                    if (_M_destroyed() == True) {
                        const std::type_info  &ti = typeid(T);
                        throw std::runtime_error(
                        std::string("singleton: instance of '").
                             append(generic::demangle(ti.name())).
                             append("' destroyed. (FIASCO problem!)"));
                    }
                    _M_destroyed(True);
                    _S_instance = Memory<T>::alloc();
                    _M_destroyed(False);
                }
            }

            return *_S_instance;
        }

    };
}

#endif /* SINGLETON_HH */
