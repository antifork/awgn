/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef SOCKADDRESS_HH
#define SOCKADDRESS_HH

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>

#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX 108
#endif

template <int f> class sockaddress;
template <>
    class sockaddress<AF_UNIX> {
        sockaddr_un _M_addr;

        public:
        sockaddress(const std::string &name) 
        : _M_addr()
        {
            _M_addr.sun_family = AF_UNIX;
            strncpy(_M_addr.sun_path, name.c_str(), UNIX_PATH_MAX-1);
            _M_addr.sun_path[UNIX_PATH_MAX-1]='\0';
        }

        const int 
        family() const 
        { return _M_addr.sun_family; }

        operator std::string() const 
        { return _M_addr.sun_path; }

        sockaddr *
        operator &() 
        { return reinterpret_cast<sockaddr *>(&_M_addr); }

        const socklen_t 
        len() const 
        { return sizeof(sockaddr_un); }
    };

template <>
    class sockaddress<AF_INET> {

        sockaddr_in _M_addr;
        socklen_t   _M_len;

        public:

        sockaddress() 
        : _M_addr(),
          _M_len(sizeof(struct sockaddr_in))
        {
            _M_addr.sin_family = AF_INET;
            _M_addr.sin_port   = 0;
            _M_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }

        sockaddress(const sockaddr_in *sa) 
        : _M_addr(),
          _M_len()
        {
            assert(sa->sin_family == AF_INET);
            _M_addr.sin_family = AF_INET;
            _M_addr.sin_port   = sa->sin_port;
            _M_addr.sin_addr   = sa->sin_addr;
        }
        
        sockaddress(const std::string &host, unsigned short port ) 
        : _M_addr(),
          _M_len(sizeof(struct sockaddr_in))
        { 
            _M_addr.sin_family = AF_INET;
            _M_addr.sin_port   = htons(port);
            if (host.empty()) {
                _M_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
                return;
            }
            if (inet_pton( AF_INET, host.c_str(), &_M_addr.sin_addr) <= 0)
                throw std::runtime_error("inet_pton");
        }

        // set...
        void 
        set_host(const std::string &host) 
        {
            if (host.empty()) {
                _M_addr.sin_addr.s_addr = htonl(INADDR_ANY);
                return;
            }
            if (inet_pton( AF_INET, host.c_str(), &_M_addr.sin_addr) <= 0)
                throw std::runtime_error("inet_pton");
        }

        void 
        set_port(unsigned short port) 
        {
            _M_addr.sin_port = htons(port);
        }

        // get...
        const std::string 
        host() const 
        {
            char buf[64];
            if (inet_ntop(AF_INET, &_M_addr.sin_addr, buf, sizeof(buf)) <= 0)
                throw std::runtime_error("inet_ntop");
            return buf;
        }
        
        const unsigned short 
        port() const 
        { return ntohs(_M_addr.sin_port); }
        
        const int family() 
        const 
        { return _M_addr.sin_family; }

        sockaddr *
        operator &() 
        { return reinterpret_cast<sockaddr *>(&_M_addr); }

        const sockaddr *
        operator &() const 
        { return reinterpret_cast<const sockaddr *>(&_M_addr); }

        socklen_t &
        len() 
        { return _M_len; }

        const socklen_t &
        len() const 
        { return _M_len; }

    };

template <>
    class sockaddress<AF_INET6> {

        sockaddr_in6 _M_addr;
        socklen_t    _M_len;

        public:

        sockaddress()
        : _M_addr(),
          _M_len(sizeof(struct sockaddr_in6))
        {
            _M_addr.sin6_family = AF_INET6;
            _M_addr.sin6_port   = 0;
            memcpy(&_M_addr.sin6_addr.s6_addr, &in6addr_any, sizeof(struct in6_addr));
        }

        sockaddress(const sockaddr_in6 *sa) 
        : _M_addr(),
          _M_len(sizeof(struct sockaddr_in6))
        {
            assert(sa->sin6_family == AF_INET6);
            _M_addr.sin6_family = AF_INET6;
            _M_addr.sin6_port   = sa->sin6_port;
            memcpy(_M_addr.sin6_addr.s6_addr, sa->sin6_addr.s6_addr, sizeof(struct in6_addr));
        }

        sockaddress(const std::string &host, unsigned short port ) 
        : _M_addr(),
          _M_len(sizeof(struct sockaddr_in6))
        { 
            _M_addr.sin6_family = AF_INET6;
            _M_addr.sin6_port   = htons(port);
            if (host.empty()) {
                memcpy(&_M_addr.sin6_addr.s6_addr, &in6addr_any, sizeof(struct in6_addr));
                return;
            }
            if (inet_pton(AF_INET6, host.c_str(), &_M_addr.sin6_addr.s6_addr) <= 0)
                throw std::runtime_error("inet_pton");
        }

        // set...
        void 
        set_host(const std::string &host) 
        {
            if (host.empty()) {
                memcpy(&_M_addr.sin6_addr.s6_addr, &in6addr_any, sizeof(struct in6_addr));
                return;
            }
            if (inet_pton(AF_INET6, host.c_str(), &_M_addr.sin6_addr.s6_addr) <= 0)
                throw std::runtime_error("inet_pton");
        }

        void 
        set_port(unsigned short port) 
        { _M_addr.sin6_port = htons(port); }

        // get...
        const std::string 
        host() const 
        {
            char buf[64];
            if (inet_ntop(AF_INET6, &_M_addr.sin6_addr, buf, sizeof(buf)) <= 0)
                throw std::runtime_error("inet_ntop");
            return buf;
        }
        
        const unsigned short 
        port() const 
        { return ntohs(_M_addr.sin6_port); }

        const int 
        family() const 
        { return _M_addr.sin6_family; }

        struct sockaddr *
        operator &() 
        { return reinterpret_cast<struct sockaddr *>(&_M_addr); }

        const struct sockaddr *
        operator &() const
        { return reinterpret_cast<const struct sockaddr *>(&_M_addr); }

        socklen_t &
        len() 
        { return _M_len; }

        const socklen_t &
        len() const 
        { return _M_len; }

    };


#endif /* SOCKADDRESS_HH */
