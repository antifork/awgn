/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef MINIDB_HH
#define MINIDB_HH

#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <map>

template <class KEY, class VALUE>
class minidb {

    public:

    template <typename T>
    class proxy {
        T value_;
        int timeout_;
        unsigned int timestamp_;

    public:
        proxy(const T r = T(), int t=-1) : value_(r), timeout_(t) {
            ts_update();
        }
        proxy & operator=(const T r) { value_ = r; ts_update(); return *this; }

        operator T() const { return value_; }
        
        void ts_update() {
            struct timeval now;
            gettimeofday(&now,NULL);
            timestamp_ = now.tv_sec;
        }
        bool expired() const {
            if (timeout_ == -1)
                return false;   // never expires
            struct timeval now;
            gettimeofday(&now,NULL);
            return now.tv_sec > (timestamp_ + timeout_);
        }
        void dump(std::ostream& o) const { 
            o << "data:" << value_ << " timestamp:" << timestamp_ << " timeout:" << timeout_ << std::endl;
        }

    };

    typedef std::map<KEY, proxy<VALUE> > db_T;
    typedef typename std::map<KEY,proxy<VALUE> >::iterator iterator;
    typedef typename std::map<KEY,proxy<VALUE> >::const_iterator const_iterator;

    minidb() {}
    ~minidb(){}

    void insert(KEY k, VALUE v, int t = -1) throw(std::runtime_error) {
        typename db_T::iterator it = db.find(k);
        if ( it != db.end() && !it->second.expired() ) 
            throw std::runtime_error("key already in use!");
        db[k] = proxy<VALUE>(v, t);
    }

    VALUE find(KEY k, bool ts_update = false) throw(std::runtime_error) {
        typename db_T::iterator it = db.find(k);
        if( it == db.end()) 
            throw std::runtime_error("key not present!");
        if (it->second.expired())
            throw std::runtime_error("key expired!");
        if (ts_update)
            it->second.ts_update();
        return it->second;
    }

    bool findkey(KEY k, bool ts_update = false) {
        typename db_T::iterator it = db.find(k);
        if ( it == db.end())
            return false;
        if (it->second.expired())
            return false;
        if (ts_update)
            it->second.ts_update();
        return true;
    }

    int erase(KEY k) {
        return db.erase(k);
    }

    typename db_T::iterator begin() {
        return  db.begin();
    }
    
    typename db_T::iterator end() {
        return db.end();
    }

    typename db_T::const_iterator begin() const {
        return  db.begin();
    }

    typename db_T::const_iterator end() const {
        return db.end();
    }

    private:
        db_T db;
};


#endif /* MINIDB_HH */
