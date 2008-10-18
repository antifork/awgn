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
#include <sstream>
#include <string>
#include <tr1/type_traits>

#include <typemap.hh>

///////////////////////////////////////////
//  MISP: Metaprogrammed Init-Script Parser 

namespace more {

    template <int n>
    struct int2Type
    {
        enum { value = n };
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // overloaded misp_parse_elem functions must be provided to parse user-defined types

    template <typename E>
    static bool misp_parse_elem(std::istream &in, E &elem)
    { in >> elem; 
        return true; }

    static bool misp_parse_elem(std::istream &in, std::string &elem)
    { 
        in >> elem;
        if (in.fail()) {
            return false;
        }

        if (elem[0] == '"') {
            char c;
            in >> std::noskipws;
            while ( in >> c && c != '"' ) {
                if ( c == '\\') {
                    in >> c;
                    if (!in)
                        break;
                }
                elem.push_back(c);
            }
            in >> std::skipws;
            elem.erase(0,1);

            if (c != '"') {
                std::clog << "parse: error at string '" << elem << ": missing quotation.\n";
                return false;
            }
        }

        return true;
    }

    template <typename T>
    struct misp
    {
    public:

        typedef typename T::key         key_type;
        typedef typename T::type        value_type;
        typedef misp<typename T::next>  map_type;

        key_type     _M_key;
        value_type   _M_value;
        map_type     _M_map;

        misp()
        : _M_value(), _M_map()
        {}

        ~misp()
        {}

    public:
        //////////////////////////////////////////////////////////////////////////
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
       
    protected:
        //////////////////////////////////////////////////////////////////////////
        // run-time parser 

        bool _parse(std::istream &in, const std::string &file, const std::string &key)
        { return __parse(in, file, key, *this); }

        template <typename U>
        static bool __parse(std::istream &in, const std::string &file, const std::string &key, misp<U> &m)
        {
            if (key == U::key::value()) {
                if (!misp_parse_elem(in,m._M_value) || in.fail() ) {
                    std::clog << file << ": parse error: key[" << U::key::value() << "] unexpected argument";
                    return false;
                }
                return true;
            }
            else return __parse(in, file, key, m._M_map);
        }
        static bool __parse(std::istream &in, const std::string &file, const std::string &k, misp<mtp::TM::null> &)
        {
            std::clog << file << ": parse error: key[" << k << "] not found";
            return false;
        }
       
    public:

        bool parse(const std::string &file)
        {
            std::ifstream sc(file.c_str());
            if (!sc) {
                std::clog << file << ": parse error: no such file\n";
                return false;
            }

            std::string line;
            for(int n = 1; std::getline(sc, line); n++) {

                std::stringstream sline(line);
                std::string key;

                // parse key...
                sline >> std::noskipws;
                sline >> std::ws;

                char c('\0');
                while ( sline >> c && c != '=' && !isspace(c)) {
                    key.push_back(c);
                }

                sline >> std::skipws;
                // std::cout << "KEY{"  << key << "}\n";

                // skip comments/empty lines
                if (key.empty() || key[0] == '#')
                    continue;

                // parse '='
                if ( c != '=') {
                    char eq('\0'); sline >> eq;
                    // std::cout << "EQ{" << eq << "}\n";
                    if ( eq != '=' ) {
                        std::clog << file << ": parse error: key[" << key << "] missing '=' (line "<< n << ")\n";
                        return false;
                    }
                }

                sline >> std::ws;

                // parse value... 
                if ( !_parse(sline,file, key) ) {
                    std::clog << " (line " << n << ")\n";
                    return false;
                }

                std::string garbage; sline >> garbage;

                if (!garbage.empty() && garbage[0] != '#') {
                    std::clog << file << ": parse error: key[" << key << "] trailing garbage (line " << n << ")\n";
                    return false;
                }
            }   

            return true;
        }
    };

    template <>
    class misp<mtp::TM::null> {};
}

#endif /* _MISP_HH_ */
