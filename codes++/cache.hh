/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef CACHE_HH
#define CACHE_HH

#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <map>

namespace generic 
{
    template <class KEY, class VALUE>
    class cache {

    public:

        template <typename T>
        class proxy {

        public:
            proxy(const T &r = T(), int t=0) 
            : _M_data(r), 
              _M_timeout(t),
              _M_timestamp()
            { ts_update(); }

            proxy & 
            operator=(const T r) 
            { 
                _M_data = r; 
                ts_update(); 
                return *this; 
            }

            operator T&() 
            { return _M_data; }

            operator T*() 
            { return &_M_data; }

            void 
            ts_update(int timeo = -1) 
            {
                struct timeval now;
                gettimeofday(&now,NULL);
                _M_timestamp = now.tv_sec;
                _M_timeout = (timeo == -1 ? _M_timeout : timeo);
            }

            bool 
            expired() const 
            {
                if (_M_timeout == 0)
                    return false;   // never expires
                struct timeval now;
                gettimeofday(&now,NULL);
                return static_cast<unsigned int>(now.tv_sec) > (_M_timestamp + _M_timeout);
            }

            void 
            dump(std::ostream& o) const 
            { 
                o << "data:" << _M_data << " timestamp:" << _M_timestamp << " timeout:" << _M_timeout << std::endl;
            }

        private:
            T _M_data;

            unsigned int _M_timeout;
            unsigned int _M_timestamp;
        };

        typedef std::map<KEY, proxy<VALUE> > db_T;
        typedef typename std::map<KEY,proxy<VALUE> >::iterator iterator;
        typedef typename std::map<KEY,proxy<VALUE> >::const_iterator const_iterator;

        cache() :
        db()
        {}

        ~cache(){}

        VALUE *
        insert(KEY k, VALUE v, int t = 0) 
        {
            typename db_T::iterator it = db.find(k);
            if ( it != db.end() && !it->second.expired() ) 
                throw std::runtime_error("key already in use!");
            db[k] = proxy<VALUE>(v, t);
            return db[k];
        }

        VALUE *
        update(KEY k, VALUE v, int t = 0) 
        {
            db[k] = proxy<VALUE>(v, t);
            db[k].ts_update();
            return db[k];
        }

        VALUE *
        update(KEY k, int t = 0) 
        {
            typename db_T::iterator it = db.find(k);
            if ( it == db.end() )
                throw std::runtime_error("key not present!");
            it->second.ts_update(t);
            return db[k]; 
        }

        VALUE *
        find(KEY k, bool ts_update = false) 
        {
            typename db_T::iterator it = db.find(k);
            if( it == db.end()) 
                throw std::runtime_error("key not present!");
            if (it->second.expired())
                throw static_cast<VALUE>(it->second);
            if (ts_update)
                it->second.ts_update();
            return it->second;
        }

        bool 
        haskey(KEY k, bool ts_update = false) 
        {
            typename db_T::iterator it = db.find(k);
            if ( it == db.end())
                return false;
            if (it->second.expired())
                return false;
            if (ts_update)
                it->second.ts_update();
            return true;
        }

        int 
        erase(KEY k) 
        { return db.erase(k); }

        typename db_T::iterator 
        begin() 
        { return  db.begin(); }

        typename db_T::iterator 
        end() 
        { return db.end(); }

        typename db_T::const_iterator 
        begin() const 
        { return  db.begin(); }

        typename db_T::const_iterator 
        end() const 
        { return db.end(); }

    private:
        db_T db;
    };

} // namespace generic

#endif /* CACHE_HH */
