/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include <socket.hh>

char buffer[80];

int main()
{
    // simple echo server
    //

    net::sockaddress<AF_INET> addr("127.0.0.1",31337);
    net::sockaddress<AF_INET> peer;
    net::socket<AF_INET> l(SOCK_STREAM);

    l.bind(addr);
    l.listen(1);

    for(;;) {
        std::cout << "waiting for a client... ";
        std::cout.flush();
        net::socket<AF_INET> r(SOCK_STREAM);
        l.accept(peer, r);
        std::cout << "[" << peer.host() << ":" << peer.port() << "]" << std::endl; 
        int n = r.recv(buffer, sizeof(buffer), 0);
        r.send(buffer, n, 0);    
    }
}
