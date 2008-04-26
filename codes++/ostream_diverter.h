/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef OSTREAM_DIVERTER_H
#define OSTREAM_DIVERTER_H

#include <iostream>

namespace more {

    // ostream diverter class -  
    //

    template <typename T>
    struct osd
    {
        static std::ostream *_M_out;

        static std::ostream &
        cout()
        { return *_M_out; }

        static void
        set_ostream(std::ostream &o)
        { _M_out = &o; }

    };

    template <typename T>
    std::ostream *osd<T>::_M_out = & std::cout;

} // namespace more

#endif /* OSTREAM_DIVERTER_H */
