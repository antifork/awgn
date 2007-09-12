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
    
    public:

        Socket(int type, int protocol=0)
        : sockfd(::socket(FAMILY, type, protocol)) {
            if ( sockfd == -1)
                throw std::runtime_error("socket");
        }

        ~Socket() { 
            if ( ::close(sockfd) == -1) 
                ::warn("close"); 
        }

        int connect(const sockaddress<FAMILY> &addr) 
        {
            int r = ::connect(sockfd, (const struct sockaddr *)&addr, addr.len());
            if (r == -1)
                ::warn("connect");
            return r;
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
        int bind(const sockaddress<FAMILY> &my_addr) 
        {
            int r = ::bind(sockfd,(const struct sockaddr *)&my_addr, my_addr.len());
            if (r == -1)
                ::warn("bind");
            return r;
        }
        int accept(sockaddress<FAMILY> &addr) 
        {
            int r = ::accept(sockfd, &addr, &addr.len());
            if (r == -1)
                ::warn("accept");
            return r;
        }
        int listen(int backlog) 
        {
            int r = ::listen(sockfd, backlog);
            if (r == -1)
                ::warn("listen");
            return r;
        }
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


#endif /* SOCKET_HH */

