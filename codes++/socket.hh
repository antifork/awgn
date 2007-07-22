/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef SOCKET_HH
#define SOCKET_HH

#include <iostream>
#include <stdexcept>
#include <cerrno>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "generic.hh"
#include "atomicity-policy.hh"

using namespace generic;

// socket class
//

namespace posix {

    template <int FAMILY=AF_INET> class socket;
    template <int FAMILY=AF_INET, class Atomicity = atomicity::GNU_CXX >
        class sockaddress {
            friend class socket<FAMILY>;

            static const int af_family = FAMILY;
            struct sockaddr_storage addr;
            struct hostent *he;
            socklen_t len;

            hostent *dup_hostent(hostent * src);
            void del_hostent(hostent * dead); 

            hostent *gethostbyname(const std::string &name) throw(std::runtime_error) {
                static typename Atomicity::mutex M_Lock;
                typename Atomicity::scoped_lock mutex(M_Lock);

                struct hostent *ht =::gethostbyname2(name.c_str(),FAMILY);
                if (ht == NULL) {
                    throw std::runtime_error("gethostbyname");
                }
                he = dup_hostent(ht);
                return he;
            }

            void sockaddress_(const std::string &name, short port, Int2type<AF_INET>) {
                struct sockaddr_in *in = (struct sockaddr_in *)&addr;
                len = sizeof(struct sockaddr_in);

                in->sin_family = FAMILY;
                in->sin_port   = htons(port);
                if ( name.empty() ) {
                    in->sin_addr.s_addr = INADDR_ANY;
                }

                he = gethostbyname(name);
                if ( he == NULL)
                    throw std::runtime_error("gethostbyname");

                memcpy(&in->sin_addr.s_addr, he->h_addr, he->h_length);
            }

            void sockaddress_(const std::string &name, short port, Int2type<AF_INET6>) {
                struct sockaddr_in6 *in6 = (struct sockaddr_in6 *)&addr;
                struct in6_addr addr6 = IN6ADDR_ANY_INIT;
                len = sizeof(struct sockaddr_in6);

                in6->sin6_family = FAMILY;
                in6->sin6_port   = htons(port);
                if ( name.empty() ) {
                    memcpy( &in6->sin6_addr.s6_addr, &addr6, sizeof(struct in6_addr));
                    return;
                }

                he = gethostbyname(name);
                if ( he == NULL)
                    throw std::runtime_error("gethostbyname");

                memcpy( &in6->sin6_addr.s6_addr, he->h_addr, he->h_length);
            }

            public:
            sockaddress() : he(NULL), len(sizeof(sockaddr_storage)) { }  
            sockaddress(const std::string &name, short port = 0) 
                : he(NULL), len(sizeof(sockaddr_storage)) {
                    sockaddress_(name, port, Int2type<FAMILY>()); 
                }

            ~sockaddress() {
                del_hostent(he);
            }

            hostent *c_hostent() const { return he; }

            template <int T>
                friend std::ostream & operator<<(std::ostream & o, const sockaddress<T> & c);

        };

    template <int FAMILY, class Atomicity>
        hostent *sockaddress<FAMILY, Atomicity>::dup_hostent(hostent * src) {
            if (src == NULL)
                return NULL;
            hostent *dest = new hostent;
            dest->h_name = strdup(src->h_name);
            dest->h_addrtype = src->h_addrtype;
            dest->h_length = src->h_length;

            int i;
            for (i = 0; src->h_aliases[i] != NULL; i++);    // count aliases
            dest->h_aliases = new char *[i + 1];
            dest->h_aliases[i] = NULL;
            for  (; src->h_aliases[i] && i >= 0; i--)       // copy aliases over
                dest->h_aliases[i] = strdup(src->h_aliases[i]);

            for  (i = 0; src->h_addr_list[i] != NULL; i++); // count addrs
            dest->h_addr_list = new char *[i + 1];
            dest->h_addr_list[i] = NULL;
            for  (--i; i >= 0; i--) {       // copy addrs over
                dest->h_addr_list[i] = new char[src->h_length];

                memcpy(dest->h_addr_list[i], src->h_addr_list[i],
                        dest->h_length);
            } return dest;
        }

    template <int FAMILY, class Atomicity>
        void sockaddress<FAMILY, Atomicity>::del_hostent(hostent * dead) {
            int i;
            if (dead == NULL)
                return;
            for (i = 0; dead->h_aliases[i] != NULL; i++)
                delete dead->h_aliases[i];
            for (i = 0; dead->h_addr_list[i] != NULL; i++)
                delete dead->h_addr_list[i];
        }

    template <int FAMILY>
        std::ostream & operator<<(std::ostream & o, const sockaddress<FAMILY> & c)
        {
            char address[40];

            if ( c.he == NULL ) {
                o << "h_name:NULL";
            } else {
                o << "h_name:" <<  ( c.he->h_name ? : "" );
                for (int i = 0; c.he->h_aliases[i]; i++)
                    o << " h_alies:" << c.he->h_aliases[i];
                o << " h_addrtype:" << c.he->h_addrtype;
                o << " h_length:" << c.he->h_length;
            }

            switch(c.af_family) {
                case AF_INET: {
                        struct sockaddr_in *in = (struct sockaddr_in *)&c.addr;
                        inet_ntop(AF_INET, &in->sin_addr.s_addr, address, 40);
                        o << " sin_family:AF_INET";
                        o << " sin_address:" << address;
                        o << " sin_port:"    << ntohs(in->sin_port);
                    } break;

                case AF_INET6: {
                        struct sockaddr_in6 *in6 = (struct sockaddr_in6 *)&c.addr;
                        inet_ntop(AF_INET6, &in6->sin6_addr.s6_addr, address, 40);
                        o << " sin6_family:AF_INET6";
                        o << " sin6_address:" << address;
                        o << " sin6_port:" << ntohs(in6->sin6_port);
                    } break;
                default:
                    throw std::runtime_error("unknown pf class");
            }
            return o;
        }

#define ERR(x) std::string(x) + std::string(": ") + strerror(errno)

    template <int FAMILY>
        class socket {

            socket &operator=(socket &);
            int _domain;
            int _type;
            int _protocol;
            int _sock;				 

            public:

            socket(const socket &ref) {
                _domain   = ref._domain;		
                _type     = ref._type;
                _protocol = ref._protocol;
                _sock     = ref._sock;
            };

            socket(int type, int protocol) 
                : _domain(FAMILY), _type(type), _protocol(protocol),  _sock(::socket(FAMILY,type,protocol)) {
                    if (_sock == -1)
                        throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                }

            ~socket() { ::close (_sock); }

            // --- libc socket wrappers

            int c_fd() const { return _sock; }

            ssize_t 
                c_recv(void *buf, size_t len, int flags, struct sockaddr *from=NULL, socklen_t *fromlen=NULL) 
                throw() {
                    return ::recvfrom(_sock, buf, len, flags, from, fromlen);
                }

            ssize_t 
                c_recvfrom(void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen) 
                throw() {
                    return ::recvfrom(_sock, buf, len, flags, from, fromlen);
                }

            ssize_t 
                c_send(const void *buf, size_t len, int flags, const struct sockaddr *to=NULL, socklen_t tolen=0) 
                throw() {
                    return ::sendto(_sock, buf, len, flags, to, tolen);
                }

            ssize_t 
                c_sendto(const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen) 
                throw() {
                    return ::sendto(_sock, buf, len, flags, to, tolen);
                }


            // --- atomic template

            template <typename T, typename C, typename L>
                ssize_t 
                atomic(T fun, void *_s, size_t n, int flags, C addr, L len) throw()
                {
                    char *s = static_cast<char *>(_s);
                    ssize_t res, pos = 0;

                    while (n > pos) {
                        res = (this->*fun)(s + pos, n - pos, flags, addr, len);
                        switch (res) {
                            case -1:
                                if (errno == EINTR || errno == EAGAIN)
                                    continue;
                                throw std::runtime_error(ERR("atomic IO:"));
                            case 0:
                                return res;
                            default:
                                pos += res;
                        }
                    }
                    return pos;
                }


            template <typename T>
                ssize_t
                atomic(T fun, void *_s, size_t n, int flags=0) throw()
                {
                    char *s = static_cast<char *>(_s);
                    ssize_t res, pos = 0;

                    while (n > pos) {
                        res = (this->*fun)(s + pos, n - pos, flags);
                        switch (res) {
                            case -1:
                                if (errno == EINTR || errno == EAGAIN)
                                    continue;
                                throw std::runtime_error(ERR("atomic IO:"));
                            case 0:
                                return res;
                            default:
                                pos += res;
                        }
                    }
                    return pos;
                }

            // IO wrappers
            //
            ssize_t recv(void *buf, size_t len, int flags=0)
            {
                int r;
                r = atomic(&socket::c_recv, buf, len, flags);
                if ( r == -1)
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                return r;
            }

            ssize_t send(void *buf, size_t len, int flags=0)
            {
                int r;
                r = atomic(&socket::c_send, buf, len, flags);
                if ( r == -1)
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                return r;
            }

            ssize_t recvfrom (void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
            {
                int r;
                r = atomic(&socket::c_recvfrom, buf, len, flags, from, fromlen);
                if ( r == -1)
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                return r;
            }

            ssize_t sendto(void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen)
            {
                int r;
                r = atomic(&socket::c_sendto, buf, len, flags, const_cast<struct sockaddr *>(to), tolen);
                if ( r == -1)
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                return r;
            }

            // connect wrappers
            //

            int connect(const sockaddress<FAMILY> &saddr) {
                int r;
                if ( (r=::connect(_sock,(struct sockaddr *)&saddr.addr,saddr.len)) == -1)
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                return r;
            }

            int bind(const sockaddress<FAMILY> &saddr) {
                int r;
                if ( (r=::bind(_sock,(struct sockaddr *)&saddr.addr,saddr.len)) == -1)
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                return r;
            }

            int listen(int backlog) {
                int r;
                if ( (r=::listen(_sock,backlog)) == -1 )
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                return r;
            }

            socket *accept(sockaddress<FAMILY> &saddr) {
                int k;
                if (( k=::accept(_sock,(struct sockaddr *)&saddr.addr, &saddr.len)) == -1 )
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));

                socket *ret = new socket<FAMILY>(*this);
                ret->_sock = k;
                return ret;
            }

            int setsockopt(int level, int optname, const void *optval, socklen_t optlen)
            {
                int r;
                if ( (r=::setsockopt(_sock, level, optname, optval, optlen)) == -1 )
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                return r;
            }

            int getsockopt(int level, int optname, void *optval, socklen_t *optlen)
            {
                int r;
                if ( (r=::getsockopt(_sock, level, optname, optval, optlen)) == -1 )
                    throw std::runtime_error(ERR(__PRETTY_FUNCTION__));
                return r;
            }

            template<int F>
                friend std::ostream & operator<<(std::ostream & o, const socket<F> & s);
        };

    template <int F>
        std::ostream & operator<<(std::ostream & o, const socket<F> & s)
        {
            o << "socket<" << ( s._domain == AF_INET ?  "AF_INET" : (s._domain == AF_INET6 ? "AF_INET6" : "" ) );
            o << "> type:" << s._type;
            o << " proto:" << s._protocol;
            o << " fd:"    << s._sock;
        }

}


#endif /* SOCKET_HH */
