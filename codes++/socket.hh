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

    int _M_fd;

    bool _M_connected;
    bool _M_bound;
    bool _M_listening;

    std::string _M_pathname;   // unix socket

public:

    Socket() 
    : _M_fd(-1), 
      _M_connected(false), 
      _M_bound(false), 
      _M_listening(false),
      _M_pathname()
    {}

    Socket(int type, int protocol=0)
    : _M_fd(::socket(FAMILY, type, protocol)), 
      _M_connected(false), 
      _M_bound(false), 
      _M_listening(false),
      _M_pathname()
    {
        if ( _M_fd == -1)
            throw std::runtime_error("socket");
    }

    Socket(const Socket<FAMILY> &);                         // noncopyable
    Socket<FAMILY> &operator=(const Socket<FAMILY> &);      // noncopyable

    ~Socket(); 

    void 
    init(int type,int protocol=0) 
    {
        if (_M_fd>2)
            throw std::runtime_error("socket already open");
        _M_fd = ::socket(FAMILY, type, protocol);
        if (_M_fd == -1)
            throw std::runtime_error("socket");
    }
    int 
    send(const void *buf, size_t len, int flags) 
    {
        int r = ::send(_M_fd, buf, len, flags);
        if (r == -1)
            ::warn ("send");
        return r;
    }
    int 
    recv(void *buf, size_t len, int flags) 
    {
        int r = ::recv(_M_fd, buf, len, flags);
        if (r == -1)
            ::warn("recv");
        return r;
    }
    int 
    sendto(const void *buf, size_t len, int flags, const sockaddress<FAMILY> &to) 
    {
        int r = ::sendto(_M_fd, buf, len, flags, reinterpret_cast<const struct sockaddr *>(&to), to.len());
        if (r == -1)
            ::warn("sendto");
        return r;
    }
    int 
    recvfrom(void *buf, size_t len, int flags, sockaddress<FAMILY> &from) 
    {
        int r = ::recvfrom(_M_fd, buf, len, flags, reinterpret_cast<const struct sockaddr *>(&from), &from.len());
        if (r == -1)
            ::warn("recvfrom");
        return r;
    }

    int connect(const sockaddress<FAMILY> &addr); 

    const bool 
    connected() const 
    { return _M_connected; }

    int bind(const sockaddress<FAMILY> &my_addr);

    const bool 
    bound() 
    const { return _M_bound; }

    int 
    accept(sockaddress<FAMILY> &addr, Socket<FAMILY> &ret) 
    {
        int r = ::accept(_M_fd,reinterpret_cast<const struct sockaddr *>(&addr), &addr.len());
        if (r == -1) {
            ::warn("accept");
        }
        else {
            ret._M_fd = r;
            ret._M_connected = true;
        }
        return r;
    }
    int 
    listen(int backlog) 
    {
        int r = ::listen(_M_fd, backlog);
        if (r == -1)
            ::warn("listen");
        else
            _M_listening = true;
        return r;
    }

    const bool 
    listening() const 
    { return _M_listening; }

    int 
    getsockname(sockaddress<FAMILY> &name) const
    {
        int r = ::getsockname(_M_fd, reinterpret_cast<const struct sockaddr *>(&name), &name.len());
        if (r == -1)
            ::warn("getsockname");
        return r;
    }
    int 
    getpeername(sockaddress<FAMILY> &name) const
    {
        int r = ::getpeername(_M_fd, reinterpret_cast<const struct sockaddr *>(&name), &name.len());
        if (r == -1)
            ::warn("getpeername");
        return r;
    }
    int 
    setsockopt(int level, int optname, const void *optval, socklen_t optlen)
    {
        int r = ::setsockopt(_M_fd, level, optname, optval, optlen);
        if (r == -1)
            ::warn("setsockopt");
        return r;
    }
    int 
    getsockopt(int level, int optname, void *optval, socklen_t *optlen) const
    {
        int r = ::getsockopt(_M_fd, level, optname, optval, optlen);
        if (r == -1)
            ::warn("getsockopt");
        return r;
    }

    const int 
    fd() const 
    { return _M_fd; }
};

// destructor specializations...
//
template <int FAMILY>
inline
Socket<FAMILY>::~Socket<FAMILY>() {
    if ( ::close(_M_fd) == -1) 
        ::warn("close"); 
}
template <>
inline
Socket<PF_UNIX>::~Socket<PF_UNIX>() { 
    if ( ::close(_M_fd) == -1) 
        ::warn("close");
    if ( !_M_pathname.empty()) {
        if ( ::unlink(_M_pathname.c_str()) == -1)
            ::warn("unlink");
    }
}

// bind specializations..
//
template <int FAMILY>
inline
int Socket<FAMILY>::bind(const sockaddress<FAMILY> &my_addr) 
{
    int r = ::bind(_M_fd,reinterpret_cast<const struct sockaddr *>(&my_addr), my_addr.len());
    if (r == -1)
        ::warn("bind");
    else
        _M_bound = true;
    return r;
}
template <>
inline
int Socket<PF_UNIX>::bind(const sockaddress<PF_UNIX> &my_addr) 
{
    int r = ::bind(_M_fd,reinterpret_cast<const struct sockaddr *>(&my_addr), my_addr.len());
    if (r == -1)
        ::warn("bind");
    else {
        _M_bound = true;
        _M_pathname = my_addr;
    }
    return r;
}

// connect specializations...
//

template <int FAMILY>
inline
int Socket<FAMILY>::connect(const sockaddress<FAMILY> &addr) 
{
    int r = ::connect(_M_fd, reinterpret_cast<const struct sockaddr *>(&addr), addr.len());
    if (r == -1)
        ::warn("connect");
    else {
        _M_connected = true;
    }
    return r;
}
template <>
inline
int Socket<PF_UNIX>::connect(const sockaddress<PF_UNIX> &addr) 
{
    int r = ::connect(_M_fd, reinterpret_cast<const struct sockaddr *>(&addr), addr.len());
    if (r == -1)
        ::warn("connect");
    else {
        _M_connected = true;
        _M_pathname = addr;
    }
    return r;
}

#endif /* SOCKET_HH */

