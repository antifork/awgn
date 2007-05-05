/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef FACTORY_HH
#define FACTORY_HH

#include <stdexcept>
#include <cstdlib>
#include <map>

#include "memory-policy.hh"

// my generic factory, inspired to that of Andrei Alexandrescu (Modern C++ Design)
//

namespace generic {

    template <class T, class factoryBase, template <class> class Memory = memory::New>
        class factoryElement : public factoryBase {
            public:
                factoryBase &alloc() {
                    return *Memory<T>::alloc();
                }
        };    

    template <typename Key, class factoryBase > class factory;
    template <typename Key, class factoryBase >
        class factory<Key, factoryBase *> {
            typedef std::map<Key, factoryBase *> FactoryMap;

            FactoryMap theFactoryMap;    

            public:
            ~factory() {
                typename FactoryMap::const_iterator i;
                for ( i = theFactoryMap.begin() ; i != theFactoryMap.end() ; i++ )
                    delete i->second;
            }

            factoryBase & operator()(const Key &k) throw(std::runtime_error) {

                typename FactoryMap::const_iterator i = theFactoryMap.find(k); 

                if ( i == theFactoryMap.end() )
                    throw std::runtime_error("factory: unknown id!");    

                return i->second->alloc();
            }

            bool inset(const Key &k) const {
                typename FactoryMap::const_iterator i = theFactoryMap.find(k);
                if ( i == theFactoryMap.end() )
                    return false;
                return true;
            }

            bool regist(const Key &k, factoryBase *e) {
                return theFactoryMap.insert(typename FactoryMap::value_type(k, e)).second;
            }

            bool unregist(const Key &k) {
                return theFactoryMap.erase(k) == 1;
            }               

        };

}

#endif /* FACTORY_HH */
