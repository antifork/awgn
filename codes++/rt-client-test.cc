/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <signal.h>
#include <iostream>

#include <rt-message.hh>

#define HELLO  0      // realtime message 0
#define WORLD  1      // realtime message 1

int main(int argc, char *argv[]) {

    if (argc <2) {
        std::cout << "usage: client pid" << std::endl;
        exit(0);
    }

    int timeout = 5; // sec

    RTMessage remote(atoi(argv[1]), timeout);

    try {
        std::cout << "*** " << remote.send(HELLO, 10) << std::endl;
    }
    catch(int rerrno) {
        std::cout << "remote errno:" << rerrno << std::endl;
    }

    std::cout << "*** " << remote.send(WORLD, 2) << std::endl; 

    return 0;
}
