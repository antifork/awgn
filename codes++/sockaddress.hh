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
#include <arpa/inet.h>

#include <iostream>
#include <string>
#include <stdexcept>

template <int f> class sockaddress;
template <>
    class sockaddress<AF_INET> {

        sockaddr_in _addr;
        socklen_t   _len;

        public:

        sockaddress(){
            _len = sizeof(struct sockaddr_in);
            _addr.sin_family = AF_INET;
            _addr.sin_port   = 0;
            _addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }

        sockaddress(const std::string &host, unsigned short port ) { 
            _len = sizeof(struct sockaddr_in);
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

        sockaddress(){
            _len = sizeof(struct sockaddr_in6);
            _addr.sin6_family = AF_INET6;
            _addr.sin6_port   = 0;
            memcpy(&_addr.sin6_addr.s6_addr, &in6addr_any, sizeof(struct in6_addr));
        }

        sockaddress(const std::string &host, unsigned short port ) { 
            _len = sizeof(struct sockaddr_in6);
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
