/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef _LOGSTREAM_HH_
#define _LOGSTREAM_HH_ 

#include <iostream>

#ifdef __LOG_FUNCTION__
#undef __LOG_FUNCTION__
#endif
#define LOGSTR(n)           #n
#define XLOGSTR(n)          LOGSTR(n)
#ifdef NDEBUG
#define __LOG_FUNCTION__    __func__
#else
#define __LOG_FUNCTION__    (__FILE__ "|" XLOGSTR(__LINE__) "|") << __func__
#endif

namespace more { 

    class logstreambuf : public std::streambuf
    {
    public:

        enum { EMERG, ALERT, CRIT, ERR, 
               WARNING, NOTICE, INFO, DEBUG };

        logstreambuf(std::streambuf *out)
        : _M_level(EMERG), _M_levelmsg(EMERG), _M_leveltmp(-1),  _M_out(out)
        {}

        void priority(int n)
        { _M_level = n; }

        void logprio_default(int n)
        { _M_levelmsg = n; }

        void logprio(int n)
        { _M_leveltmp = n; }

        static int iword_index()
        { static int index = std::ios_base::xalloc();
            return index; }

    protected:

        virtual std::streamsize
        xsputn (const char *s, std::streamsize n)
        {
            if ( _M_current() <= _M_level) {
                return _M_out->sputn(s,n);
            }
            return n; 
        }

        virtual int_type
        overflow (int_type c)
        {
            if (_M_current() <= _M_level) {
                if (c == '\n')
                    _M_leveltmp = -1;
                return _M_out->sputc(c);
            }
            return c;
        }

        int sync()
        {
            if (_M_current() <= _M_level)
                return  _M_out->pubsync();
            return 0;
        }

    private:        
        
        int _M_current() const 
        { return _M_leveltmp != -1 ? _M_leveltmp : _M_levelmsg; }

        int _M_level;
        int _M_levelmsg;
        int _M_leveltmp;
        std::streambuf *_M_out;

    };

    static inline std::ostream &logprio(std::ostream &out, int n = logstreambuf::EMERG)
    {
        logstreambuf * lb = dynamic_cast<logstreambuf *>(out.rdbuf());
        if(lb) 
            lb->logprio(n);
        return out;
    }
    static inline std::ostream &logprio_default(std::ostream &out, int n = logstreambuf::EMERG)
    {
        logstreambuf * lb = dynamic_cast<logstreambuf *>(out.rdbuf());
        if(lb) 
            lb->logprio_default(n);
        return out;
    }


} // namespace more

#endif /* _LOGSTREAM_HH_ */
