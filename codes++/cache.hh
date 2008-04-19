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
            typedef T value_type;

            proxy(const T &r = T(), int t=0)
            : _M_data(r),
              _M_timeout(t),
              _M_timestamp()
            { this->ts_update(); }

            proxy(const proxy &rhs)
            {
                _M_data = rhs._M_data;
                _M_timeout = rhs._M_timeout;
                this->ts_update();
            }

            proxy &
            operator=(const proxy &rhs)
            {
                if ( &_M_data == &rhs )
                    return *this;
                this->_M_data = rhs._M_data;
                this->_M_timeout = rhs._M_timeout;
                this->ts_update();
                return *this;
            }

            proxy &
            operator=(const T &rhs)
            {
                _M_data = rhs;
                this->ts_update();
                return *this;
            }

            T *
            operator&()
            { return &_M_data; }

            const T *
            operator&() const
            { return &_M_data; }

            operator T &()
            { return _M_data; }

            operator const T &() const
            { return _M_data; }

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

        private:
            T _M_data;
            unsigned int _M_timeout;
            unsigned int _M_timestamp;
        };

        typedef std::map<KEY, proxy<VALUE> > db_T;
        typedef typename std::map<KEY,proxy<VALUE> >::iterator iterator;
        typedef typename std::map<KEY,proxy<VALUE> >::const_iterator const_iterator;

        cache() 
        : _M_db()
        {}

        ~cache(){}

        VALUE *
        insert(KEY k, VALUE v, int t = 0) 
        {
            typename db_T::iterator it = _M_db.find(k);
            if ( it != _M_db.end() && !it->second.expired() ) 
                throw std::runtime_error("key already in use!");
            _M_db[k] = proxy<VALUE>(v, t);
            return & _M_db[k];
        }

        VALUE *
        update(KEY k, VALUE v, int t = 0) 
        {
            _M_db[k] = proxy<VALUE>(v, t);
            _M_db[k].ts_update();
            return & _M_db[k];
        }

        VALUE *
        update(KEY k, int t = 0) 
        {
            typename db_T::iterator it = _M_db.find(k);
            if ( it == _M_db.end() )
                throw std::runtime_error("key not found!");
            it->second.ts_update(t);
            return & _M_db[k]; 
        }

        VALUE *
        find(KEY k, bool ts_update = false) 
        {
            typename db_T::iterator it = _M_db.find(k);
            if( it == _M_db.end()) 
                throw std::runtime_error("key not found!");
            if (it->second.expired())
                throw static_cast<VALUE>(it->second);
            if (ts_update)
                it->second.ts_update();
            return & it->second;
        }

        bool 
        has_key(KEY k, bool ts_update = false) 
        {
            typename db_T::iterator it = _M_db.find(k);
            if ( it == _M_db.end())
                return false;
            if (it->second.expired())
                return false;
            if (ts_update)
                it->second.ts_update();
            return true;
        }

        int 
        erase(KEY k) 
        { return _M_db.erase(k); }

        // iterators...
        //

        typename db_T::iterator 
        begin() 
        { return  _M_db.begin(); }

        typename db_T::iterator 
        end() 
        { return _M_db.end(); }

        typename db_T::const_iterator 
        begin() const 
        { return  _M_db.begin(); }

        typename db_T::const_iterator 
        end() const 
        { return _M_db.end(); }

    private:
        db_T _M_db;
    };

} // namespace generic

#endif /* CACHE_HH */
