/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <iostream>
#include <rt-message.hh>

#define HELLO  0      // realtime message 0
#define WORLD  1      // realtime message 1

int hello(int i) {
    std::cout << "hello(" << i << ")" << std::endl;
    sleep(1);
    return 1;
}

int world(int i) {
    std::cout << "world(" << i << ")" << std::endl;
    return 0;
}


int main(int argc, char *argv[])
{
    RTMessageSrv::instance().regist(HELLO,hello); 
    RTMessageSrv::instance().regist(WORLD,world); 

    for(;;) {
        std::cout << "boing!" << std::endl;
        sleep(1);
    }

    return 0;
}
