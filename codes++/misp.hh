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

#include <typemap.hh>


/////////////////////////////////////////
//  metaprogrammed init-script parser 
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

        typedef typename T::type            value_type;
        typedef misp<typename T::next>      map_type;

    private:

        value_type   _M_value;
        map_type     _M_map;

        /////////////////////////// GET /////////////////////////////////

    public:

        misp()
        : _M_value(),
        _M_map()
        {}

        ~misp()
        {}

        template <char *K>
        typename mtp::TM::has_type<K, T>::type
        get() const
        { 
            return __get<K>(int2Type< mtp::TM::indexof<K, T>::value >()); 
        }

        template <char *K, int n>
        typename mtp::TM::has_type<K, T>::type
        __get(int2Type<n>) const
        { 
            return _M_map.__get<K>(int2Type<n-1>()); 
        }

        template <char *K>
        value_type
        __get(int2Type<0>) const
        { 
            return _M_value; 
        } 

        /////////////////////////// SET /////////////////////////////////

        template <char *K, typename V>
        void set(const V &value)
        {
            __set<K,V>(value, int2Type<mtp::TM::indexof<K, T>::value >());
        }

        template <char *K, typename V, int n>
        void __set(const V &value, int2Type<n>)
        {
            _M_map.__set<K>(value, int2Type<n-1>());
        }    

        template <char *K, typename V>
        void __set(const V &value, int2Type<0>)
        { _M_value = value; } 

        /////////////////////////// parse /////////////////////////////////

    public:

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
