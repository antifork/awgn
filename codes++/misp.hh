/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef _MISP_HH_
#define _MISP_HH_ 

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <tr1/type_traits>

#include <typemap.hh>

///////////////////////////////////////////
//  MISP: Metaprogrammed Init-Script Parser 
//

namespace more {

    // int2Type

    template <int n>
    struct int2Type
    {
        enum { value = n };
    };

    template <typename T>
    class misp
    {
    public:
        typedef typename T::key         key_type;
        typedef typename T::type        value_type;
        typedef misp<typename T::next>  map_type;

    private:
        key_type     _M_key;
        value_type   _M_value;
        map_type     _M_map;

    public:

        misp()
        : _M_value(), _M_map()
        {}

        ~misp()
        {}

        ///////////////////
        // compile-time get

        template <typename K>
        typename std::tr1::add_reference< typename mtp::TM::get<K, T>::type>::type
        get() 
        { 
            return __get<K>(int2Type< mtp::TM::indexof<K, T>::value >()); 
        }

        template <typename K, int n>
        typename std::tr1::add_reference<typename mtp::TM::get<K, T>::type>::type
        __get(int2Type<n>) 
        { 
            return _M_map.__get<K>(int2Type<n-1>()); 
        }

        template <typename K>
        typename std::tr1::add_reference<value_type>::type
        __get(int2Type<0>) 
        { 
            return _M_value; 
        } 
        
        ////////////////////////////////

        bool parse(const std::string &f)
        {
            std::ifstream sc(f.c_str());
            if (!sc) {
                std::clog << __FUNCTION__ << ": couldn't open file " << f << " for parsing.\n";
                return false;
            }

            std::string line;
            for(int n = 1; std::getline(sc, line); n++) {

                const char * p = line.c_str();
                // skip spaces in set
                //
                p += strspn(p, " \f\n\r\t\v");

                // skip comment and empty lines..
                //
                if (p[0] == '#' || p[0] == '\0')
                    continue;

                // std::cout << "[" << p << "]" << std::endl;

                char key[80] = { 0 }, value[80]= { 0 };

                if ( sscanf(p, "%s", key) != 1 ) {
                    std::clog << __FUNCTION__ << ": error at line " << n << std::endl;
                    return false;
                }

                p += strlen(key);
                p += strspn(p, " \t");

                if ( *p++ != '=' ) {
                    std::clog << __FUNCTION__ << ": error at line " << n << std::endl;
                    return false;
                }

                p += strspn(p, " \t");

                if ( sscanf(p, "%s", value) != 1 ) {
                    std::clog << __FUNCTION__ << ": error at line " << n << std::endl;
                    return false;
                }

                if  ( value[0] == '"' && value[1] == '"' ) // empty string
                {
                    value[0] = '\0';
                }
                else
                    if ( value[0] == '"' && sscanf(p, "\"%[^\"]\"", value) != 1 ) {
                        std::clog << __FUNCTION__ << ": error at line " << n << std::endl;
                        return false;
                    }

                // std::clog << "---> key:" << key << " value:'" << value << "'\n";

                if ( !this->grammar(key,value) ) {
                    std::clog << __FUNCTION__ << ": error at line " << n << " [ key:" << key << " value:" << value << " ]\n"; 
                    return false;
                }
            }   

            return true;
        }

    protected:
        virtual bool grammar(const std::string &, const std::string &) { return false; }

    };

    template <>
    class misp<mtp::TM::null> {};
}

#endif /* _MISP_HH_ */
