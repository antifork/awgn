/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include "socket.hh"

int main()
{
#ifdef IPV6
	posix::sockaddress<AF_INET6> local("::1",10000);
	posix::sockaddress<AF_INET6> remote;
#else
	posix::sockaddress<AF_INET> local("127.0.0.1",10000);
	posix::sockaddress<AF_INET> remote;
#endif

	// create socket 

#ifdef IPV6
    posix::socket<AF_INET6> s(SOCK_STREAM,0);
#else
    posix::socket<AF_INET>  s(SOCK_STREAM,0);
#endif

    std::cout << s << std::endl;

    int tru = 1;
	s.setsockopt(SOL_SOCKET, SO_REUSEADDR, &tru, sizeof(tru));

	s.bind(local);    
	s.listen(1024);

	std::cout << "accepting connections..\n";

#ifdef IPV6
    posix::socket<AF_INET6> *peer = s.accept(remote);
#else
    posix::socket<AF_INET>  *peer = s.accept(remote);
#endif

    std::cout << remote << std::endl;
    std::cout << *peer  << std::endl;

	sleep(2);

	delete peer;
	std::cout << "connection closed\n";
}
