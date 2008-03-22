/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

/*
 * NOTE: ilxml relies on the XML library v2.33 written by 
 *       Frank Vanden Berghen and available from: 
 *       http://www.applied-mathematics.net/tools/xmlParser.html
 */

#ifndef __ILXML__HH
#define __ILXML__HH

#include <sstream>
#include <string>
#include <xmlParser.hh>

namespace ilx 
{
    static inline XMLCSTR addr(const std::string &r) {
        return r.c_str();
    }

    template <typename T>
    static inline std::string str(const T &x) {
        std::ostringstream out;
        out << x;
        return out.str();
    }

    template<> 
    inline std::string str<bool>(const bool& x)
    {
        std::ostringstream out;
        out << std::boolalpha << x;
        return out.str();
    }

    template<> 
    inline std::string str<std::string>(const std::string& x)
    {
        return x;
    } 

} // namespace ilx


#endif /* __ILXML__HH */
