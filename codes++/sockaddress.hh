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
        sockaddr_un _addr;

        public:
        sockaddress(const std::string &name) :
        _addr()
        {
            _addr.sun_family = AF_UNIX;
            strncpy(_addr.sun_path, name.c_str(), UNIX_PATH_MAX-1);
            _addr.sun_path[UNIX_PATH_MAX-1]='\0';
        }
        int family() const { return _addr.sun_family; }
        operator std::string() const { return _addr.sun_path; }
        sockaddr *operator &() {
            return reinterpret_cast<sockaddr *>(&_addr);
        }
        const socklen_t len() const { return sizeof(sockaddr_un); }
    };

template <>
    class sockaddress<AF_INET> {

        sockaddr_in _addr;
        socklen_t   _len;

        public:

        sockaddress() :
        _addr(),
        _len(sizeof(struct sockaddr_in))
        {
            _addr.sin_family = AF_INET;
            _addr.sin_port   = 0;
            _addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        sockaddress(const sockaddr_in *sa) :
        _addr(),
        _len()
        {
            assert(sa->sin_family == AF_INET);
            _addr.sin_family = AF_INET;
            _addr.sin_port   = sa->sin_port;
            _addr.sin_addr   = sa->sin_addr;
        }
        sockaddress(const std::string &host, unsigned short port ) :
        _addr(),
        _len(sizeof(struct sockaddr_in))
        { 
            _addr.sin_family = AF_INET;
            _addr.sin_port   = htons(port);
            if (host.empty()) {
                _addr.sin_addr.s_addr  = htonl(INADDR_ANY);
                return;
            }
            if (inet_pton( AF_INET, host.c_str(), &_addr.sin_addr) <= 0)
                throw std::runtime_error("inet_pton");
        }

        // set...
        void set_host(const std::string &host) {
            if (host.empty()) {
                _addr.sin_addr.s_addr = htonl(INADDR_ANY);
                return;
            }
            if (inet_pton( AF_INET, host.c_str(), &_addr.sin_addr) <= 0)
                throw std::runtime_error("inet_pton");
        }

        void set_port(unsigned short port) {
            _addr.sin_port = htons(port);
        }

        // get...
        std::string host() const {
            char buf[64];
            if (inet_ntop(AF_INET, &_addr.sin_addr, buf, sizeof(buf)) <= 0)
                throw std::runtime_error("inet_ntop");
            return buf;
        }
        unsigned short port() const { return ntohs(_addr.sin_port); }
        int family() const { return _addr.sin_family; }

        sockaddr *operator &() {
            return (sockaddr *)&_addr;
        }

        socklen_t &len() { return _len; }
        const socklen_t &len() const { return _len; }

    };

template <>
    class sockaddress<AF_INET6> {

        sockaddr_in6 _addr;
        socklen_t    _len;

        public:

        sockaddress():
        _addr(),
        _len(sizeof(struct sockaddr_in6))
        {
            _addr.sin6_family = AF_INET6;
            _addr.sin6_port   = 0;
            memcpy(&_addr.sin6_addr.s6_addr, &in6addr_any, sizeof(struct in6_addr));
        }
        sockaddress(const sockaddr_in6 *sa) :
        _addr(),
        _len(sizeof(struct sockaddr_in6))
        {
            assert(sa->sin6_family == AF_INET6);
            _addr.sin6_family = AF_INET6;
            _addr.sin6_port   = sa->sin6_port;
            memcpy(_addr.sin6_addr.s6_addr, sa->sin6_addr.s6_addr, sizeof(struct in6_addr));
        }
        sockaddress(const std::string &host, unsigned short port ) :
        _addr(),
        _len(sizeof(struct sockaddr_in6))
        { 
            _addr.sin6_family = AF_INET6;
            _addr.sin6_port   = htons(port);
            if (host.empty()) {
                memcpy(&_addr.sin6_addr.s6_addr, &in6addr_any, sizeof(struct in6_addr));
                return;
            }
            if (inet_pton(AF_INET6, host.c_str(), &_addr.sin6_addr.s6_addr) <= 0)
                throw std::runtime_error("inet_pton");
        }

        // set...
        void set_host(const std::string &host) {
            if (host.empty()) {
                memcpy(&_addr.sin6_addr.s6_addr, &in6addr_any, sizeof(struct in6_addr));
                return;
            }
            if (inet_pton(AF_INET6, host.c_str(), &_addr.sin6_addr.s6_addr) <= 0)
                throw std::runtime_error("inet_pton");
        }

        void set_port(unsigned short port) {
            _addr.sin6_port = htons(port);
        }

        // get...
        std::string host() const {
            char buf[64];
            if (inet_ntop(AF_INET6, &_addr.sin6_addr, buf, sizeof(buf)) <= 0)
                throw std::runtime_error("inet_ntop");
            return buf;
        }
        unsigned short port() const { return ntohs(_addr.sin6_port); }
        int family() const { return _addr.sin6_family; }

        struct sockaddr *operator &() {
            return (struct sockaddr *)&_addr;
        }

        socklen_t &len() { return _len; }
        const socklen_t &len() const { return _len; }
    };


#endif /* SOCKADDRESS_HH */
