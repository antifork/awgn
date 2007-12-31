/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef MEYERS_HH
#define MEYERS_HH

#define SINGLETON(x)        x : public generic::singleton<x>
#define SINGLETON_FRIEND(x) friend class generic::singleton<x>

// Singleton Meyers is thread safe when compiled with g++ 4.0 or later
//

namespace generic
{
    template <typename T>
    struct singleton
    {
        static T& instance()
        {
            static T _one_;
            return _one_;
        }
    };
}

#endif /* MEYERS_HH */
