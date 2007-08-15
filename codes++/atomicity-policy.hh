/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef ATOMICITY_HH
#define ATOMICITY_HH

#include "generic.hh"

#if   __GNUC__ >= 4
#include <tr1/memory>
#endif

namespace atomicity {

    struct None {
        typedef generic::NullType mutex;
        typedef generic::NullType scoped_lock;
    };

    struct Boost {

#ifdef BOOST_HAS_THREADS
        typedef boost::mutex mutex;
        typedef boost::mutex::scoped_lock scoped_lock;
#endif

    };

    struct GNU_CXX {

        template <int, int> struct gcc;
        template <> struct gcc<4,1> {
            typedef __gnu_cxx::mutex_type mutex;
            typedef __gnu_cxx::lock scoped_lock;
        };
        template <> struct gcc<4,2> {
            typedef __gnu_cxx::__mutex mutex;
            typedef __gnu_cxx::__scoped_lock scoped_lock;
        };

        typedef gcc<__GNUC__,__GNUC_MINOR__>::mutex mutex;
        typedef gcc<__GNUC__,__GNUC_MINOR__>::scoped_lock scoped_lock;

    };

}

#endif /* ATOMICITY_HH */
