/* $Id$ */
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

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <err.h>

#include <sockaddress.hh>

template <int FAMILY>
class Socket {

    int sockfd;

    bool _connected;
    bool _bound;
    bool _listening;

    std::string _pathname;   // unix socket

    public:

        Socket() : sockfd(-1), _connected(false), _bound(false) {}
        Socket(int type, int protocol=0)
        : sockfd(::socket(FAMILY, type, protocol)), _connected(false), _bound(false) {
            if ( sockfd == -1)
                throw std::runtime_error("socket");
        }

        ~Socket();

        void init(int type,int protocol=0) {
            if (sockfd>2)
                throw std::runtime_error("socket already open");
            sockfd = ::socket(FAMILY, type, protocol);
            if (sockfd == -1)
                throw std::runtime_error("socket");
        }
        int send(const void *buf, size_t len, int flags) 
        {
            int r = ::send(sockfd, buf, len, flags);
            if (r == -1)
                ::warn ("send");
            return r;
        }
        int recv(void *buf, size_t len, int flags) 
        {
            int r = ::recv(sockfd, buf, len, flags);
            if (r == -1)
                ::warn("recv");
            return r;
        }
        int sendto(const void *buf, size_t len, int flags, const sockaddress<FAMILY> &to) 
        {
            int r = ::sendto(sockfd, buf, len, flags, (const struct sockaddr *)&to, to.len());
            if (r == -1)
                ::warn("sendto");
            return r;
        }
        int recvfrom(void *buf, size_t len, int flags, sockaddress<FAMILY> &from) 
        {
            int r = ::recvfrom(sockfd, buf, len, flags, &from, &from.len());
            if (r == -1)
                ::warn("recvfrom");
            return r;
        }

        int connect(const sockaddress<FAMILY> &addr); 
        const bool connected() const { return _connected; }

        int bind(const sockaddress<FAMILY> &my_addr);
        const bool bound() const { return _bound; }

        Socket accept(sockaddress<FAMILY> &addr) 
        {
            Socket ret;

            int r = ::accept(sockfd, &addr, &addr.len());
            if (r == -1) {
                ::warn("accept");
            }
            else {
                ret.sockfd = r;
                ret._connected = true;
            }
            return ret;
        }
        int listen(int backlog) 
        {
            int r = ::listen(sockfd, backlog);
            if (r == -1)
                ::warn("listen");
            else
                _listening = true;
            return r;
        }
        const bool listening() const { return _listening; }

        int getsockname(sockaddress<FAMILY> &name) 
        {
            int r = ::getsockname(sockfd, &name, &name.len());
            if (r == -1)
                ::warn("getsockname");
            return r;
        }
        int getpeername(sockaddress<FAMILY> &name) 
        {
            int r = ::getpeername(sockfd, &name, &name.len());
            if (r == -1)
                ::warn("getpeername");
            return r;
        }
        int setsockopt(int level, int optname, const void *optval, socklen_t optlen)
        {
            int r = ::setsockopt(sockfd, level, optname, optval, optlen);
            if (r == -1)
                ::warn("setsockopt");
            return r;
        }
        int getsockopt(int level, int optname, void *optval, socklen_t *optlen)
        {
            int r = ::getsockopt(sockfd, level, optname, optval, optlen);
            if (r == -1)
                ::warn("getsockopt");
            return r;
        }

        const int fd() const { return sockfd; }
};

// destructor specializations...
//
template <int FAMILY>
inline
Socket<FAMILY>::~Socket<FAMILY>() {
    if ( ::close(sockfd) == -1) 
        ::warn("close"); 
}
template <>
inline
Socket<PF_UNIX>::~Socket<PF_UNIX>() { 
    if ( ::close(sockfd) == -1) 
        ::warn("close");
    if ( !_pathname.empty()) {
        if ( ::unlink(_pathname.c_str()) == -1)
            ::warn("unlink");
    }
}

// bind specializations..
//
template <int FAMILY>
inline
int Socket<FAMILY>::bind(const sockaddress<FAMILY> &my_addr) 
{
    int r = ::bind(sockfd,(const struct sockaddr *)&my_addr, my_addr.len());
    if (r == -1)
        ::warn("bind");
    else
        _bound = true;
    return r;
}
template <>
inline
int Socket<PF_UNIX>::bind(const sockaddress<PF_UNIX> &my_addr) 
{
    int r = ::bind(sockfd,(const struct sockaddr *)&my_addr, my_addr.len());
    if (r == -1)
        ::warn("bind");
    else {
        _bound = true;
        _pathname = my_addr;
    }
    return r;
}

// connect specializations...
//
   
template <int FAMILY>
inline
int Socket<FAMILY>::connect(const sockaddress<FAMILY> &addr) 
{
    int r = ::connect(sockfd, (const struct sockaddr *)&addr, addr.len());
    if (r == -1)
        ::warn("connect");
    else {
        _connected = true;
    }
    return r;
}
template <>
inline
int Socket<PF_UNIX>::connect(const sockaddress<PF_UNIX> &addr) 
{
    int r = ::connect(sockfd, (const struct sockaddr *)&addr, addr.len());
    if (r == -1)
        ::warn("connect");
    else {
        _connected = true;
        _pathname = addr;
    }
    return r;
}
 
#endif /* SOCKET_HH */

