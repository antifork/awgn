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

    RTMessage remote(atoi(argv[1]));

    std::cout << "*** " << remote.msg(HELLO, 1) << std::endl; 
    std::cout << "*** " << remote.msg(WORLD, 2) << std::endl; 

    return 0;
}
