/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef _MTSTREAM_HH_
#define _MTSTREAM_HH_ 

#include <iostream>
#include <sched.h>

namespace more { 

    static inline 
    void ticket_spinlock(volatile int &sp)
    {
        int spinlock = __sync_fetch_and_add(&sp, 1L << 16);
        unsigned short ticket = (spinlock >> 16) & 0xffff;    
        while ( (sp & 0xffff )  != ticket) {
            sched_yield();
        }            
    }

    static inline
    void ticket_spinunlock(volatile int &sp)
    {
        unsigned int spinlock = __sync_add_and_fetch(&sp, 1);
        if ( ! (spinlock & 0xffff) ) {
            sp -= 1<<16;
        }    
    }

   struct mtstream {

        struct unlock {};
        struct lock{};

        static int iword_index()
        {
            static int index = std::ios_base::xalloc();
            return index;
        }

        static inline std::ostream &__lock(std::ostream &out)
        {
           volatile int & sp = reinterpret_cast<int &>(out.iword( iword_index()));
           ticket_spinlock(sp);
           return out;
        }

        static inline std::ostream &__unlock(std::ostream &out)
        {
            volatile int & sp = reinterpret_cast<int &>(out.iword( iword_index())); 
            ticket_spinunlock(sp);
            return out;
        }

    };

} // namespace more

namespace std
{
    std::ostream &operator<<(std::ostream &out, more::mtstream::unlock)
    {
        more::mtstream::__unlock(out);
        return out;
    }

    std::ostream &operator<<(std::ostream &out, more::mtstream::lock)
    {
        more::mtstream::__lock(out);
        return out;
    }
}

#endif /* _MTSTREAM_HH_ */
