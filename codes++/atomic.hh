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

    template <typename T = _Atomic_word>
    class atomic_word
    {
        volatile T _M_value;

    public:

        explicit atomic_word(T v=0) 
        : 
        _M_value(v)
        {}

        const T 
        operator=(T n) volatile 
        {
            return _M_value = n;
        }

        operator T() const volatile 
        {
            return _M_value;
        }

#ifdef USE_GCC_BUILTIN
        const T 
        operator++(int) volatile
        {
            return __sync_fetch_and_add(&_M_value, 1); 
        }        
        const T 
        operator--(int) volatile
        {  
            return __sync_fetch_and_sub(&_M_value, 1); 
        }
        const T
        operator++() volatile 
        {
            return  __sync_add_and_fetch(&_M_value, 1);
        }        
        const T 
        operator--() volatile
        {
            return  __sync_sub_and_fetch(&_M_value, 1);
        }
        const T 
        operator &=(T v) volatile
        {
            return  __sync_and_and_fetch(&_M_value,v); 
        }
        const T 
        operator |=(T v) volatile 
        {
            return __sync_or_and_fetch(&_M_value,v); 
        }
        const T 
        operator ^=(T v) volatile 
        {
            return  __sync_xor_and_fetch(&_M_value,v); 
        }        
        
        const T 
        val_compare_and_swap(T oldval, T newval) volatile
        {
            return  __sync_val_compare_and_swap(&_M_value, oldval, newval); 
        }
 
        bool 
        bool_compare_and_swap(T oldval, T newval) volatile
        {
            return __sync_bool_compare_and_swap(&_M_value, oldval, newval); 
        }
        static void memory_barrier()
        {
            __sync_synchronize();
        }

#else // USE__GNU_CXX
        _Atomic_word operator++(int) volatile
        {
            return __gnu_cxx::__exchange_and_add(&_M_value,1);
        }    
        _Atomic_word operator--(int) volatile
        {
            return __gnu_cxx::__exchange_and_add(&_M_value,-1);
        } 
#endif
    
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
