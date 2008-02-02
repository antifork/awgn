/* $Id$ */
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
#include <memory-policy.hh>

#if   __GNUC__ >= 4
#include <tr1/memory>
#else
#error "g++ compiler not supported"
#endif

// my generic factory, inspired to that of Andrei Alexandrescu (Modern C++ Design)
//

namespace generic {

    template <class T, typename E, class factoryBase, template <class, class> class Memory = memory::New>
        class factoryElement : public factoryBase {
            public:
                std::tr1::shared_ptr<factoryBase> alloc() {
                    return std::tr1::shared_ptr<factoryBase>( Memory<T,int>::alloc(), factoryElement::dealloc );
                }
                std::tr1::shared_ptr<factoryBase> alloc(const E &e) {
                    return std::tr1::shared_ptr<factoryBase>( Memory<T,E>::alloc(e), factoryElement::dealloc );
                }
                static void dealloc(factoryBase *obj) {
                    Memory<T,E>::dealloc(static_cast<T *>(obj));
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

            template <typename E>
            std::tr1::shared_ptr<factoryBase> operator()(const Key &k, const E &e) throw(std::runtime_error) {

                typename FactoryMap::const_iterator i = theFactoryMap.find(k); 

                if ( i == theFactoryMap.end() )
                    throw std::runtime_error("factory: unknown producer!");    

                return i->second->alloc(e);
            }

            std::tr1::shared_ptr<factoryBase> operator()(const Key &k) throw(std::runtime_error) {

                typename FactoryMap::const_iterator i = theFactoryMap.find(k); 

                if ( i == theFactoryMap.end() )
                    throw std::runtime_error("factory: unknown producer!");    

                return i->second->alloc();
            }

            bool has_key(const Key &k) const {
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
