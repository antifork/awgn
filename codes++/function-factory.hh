/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef FUNCTION_FACTORY_HH
#define FUNCTION_FACTORY_HH

#ifdef USE_BOOST
#include <boost/function.hpp>
namespace std { namespace tr1 = ::boost; }
#else
#include <tr1/functional>
#endif

#include <iostream>
#include <stdexcept>
#include <string>
#include <map>

namespace generic 
{
    template <typename Key, typename T>
    class functionFactory
    {
    public:
        typedef std::tr1::function<T> FactoryFunction;
        struct register_factory
        {
            functionFactory<Key,T> & _ref_;
            FactoryFunction   _elem_;
            const std::string _name_;

            template <typename F>
            register_factory(functionFactory &ref, const std::string &name, const F &elem)
            :
            _ref_(ref),
            _elem_(std::tr1::function<T>(elem)),
            _name_(name)
            {
                _ref_.regist(_name_, _elem_);
            }

            ~register_factory()
            {
                _ref_.unregist(_name_);
            }

        };

        functionFactory() 
        {}

        ~functionFactory()
        {}

        FactoryFunction 
        operator()(const Key &k) const throw(std::runtime_error) 
        {
            typename FactoryMap::const_iterator it = theFactoryMap.find(k); 
            if ( it == theFactoryMap.end() )
                throw std::runtime_error("factory: unknown producer!");    

            return it->second;
        }

        bool has_key(const Key &k) const 
        {
            typename FactoryMap::const_iterator i = theFactoryMap.find(k);
            if ( i == theFactoryMap.end() )
                return false;
            return true;
        }

        template <typename E>
        bool regist(const Key &k, E &e) 
        {
            return theFactoryMap.insert(typename FactoryMap::value_type(k, std::tr1::function<T>(e))).second;
        }

        bool unregist(const Key &k) 
        {
            return theFactoryMap.erase(k) == 1;
        }               

    private:
        typedef std::map<Key, FactoryFunction > FactoryMap;
        FactoryMap theFactoryMap;

    };

} // namespace generic

#endif /* FUNCTION_FACTORY_HH */
