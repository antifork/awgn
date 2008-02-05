/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef ATOMIC_HH
#define ATOMIC_HH

#include <iostream>
#include "atomicity-policy.hh"

#if   __GNUC__ >= 4
#include <tr1/memory>
#elif (__GNUC__ == 3) && (__GNUC_MINOR__ == 4)
#include <bits/atomicity.h>
#else
#error "g++ compiler not supported"
#endif

namespace more {

    class atomic_word
    {
        _Atomic_word value;

    public:
        explicit atomic_word(_Atomic_word v=0) : value(v) {}

        operator int() volatile {
            return value;
        }

        _Atomic_word operator=(_Atomic_word n) volatile 
        {
            return value = n;
        }
        _Atomic_word operator++(int) volatile
        {
            return __gnu_cxx::__exchange_and_add(&value,1);
        }    
        _Atomic_word operator--(int) volatile
        {
            return __gnu_cxx::__exchange_and_add(&value,-1);
        } 
        _Atomic_word operator+=(int n) volatile
        {
            return __gnu_cxx::__exchange_and_add(&value,n);
        }
        _Atomic_word operator-=(int n) volatile
        {
            return __gnu_cxx::__exchange_and_add(&value,-n);
        }
    };

    // the following atomic_ptr is based on an idea of Alexandrescu, 
    // see http://www.ddj.com/cpp/184403766 for further details.

    template <typename Atomicity = atomicity::GNU_CXX>
    struct atomic_class 
    {
        typename Atomicity::mutex mutex;
    };

    template <typename T, class Atomicity = atomicity::GNU_CXX>
    class atomic_ptr {

        T* ptr;
        typename Atomicity::scoped_lock lock;

        atomic_ptr(const atomic_ptr&);              // uncopyable
        atomic_ptr& operator=(const atomic_ptr&);   // uncopyable

    public:

        explicit atomic_ptr(volatile T& obj)
        : ptr(const_cast<T*>(&obj)), lock(const_cast<T *>(&obj)->mutex) 
        { }
        atomic_ptr(volatile T& obj, typename Atomicity::mutex& m_)
        : ptr(const_cast<T*>(&obj)), lock(m_) 
        { }
 
        ~atomic_ptr() 
        { }

        T& operator*()
        {    return *ptr;    }

        T* operator->()
        {   return ptr;   }

    };

    template <typename Atomicity>
    class atomic_ptr<_Atomic_word,Atomicity>;       // use atomic_word class directly. 


} // namespace more

#endif /* ATOMIC_HH */
