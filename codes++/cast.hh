/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef _CAST_HH_
#define _CAST_HH_ 

#include <cassert>

namespace more { 

    // polymorphic_downcast ala boost...
    //

    template <typename R, typename T>
    inline R polymorphic_downcast(T p)
    {
        assert( dynamic_cast<R>(p) == p );
        return static_cast<R>(p);
    }

} // namespace more

#endif /* _CAST_HH_ */
