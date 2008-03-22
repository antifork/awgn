/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <flock.hh>

int main()
{
    std::cout << "start...\n";
    {
        ScopedFlock lock("/tmp/test", LOCK_EX|LOCK_NB);
        std::cout << "locked, sleeping...\n";
        sleep(10);
    }
    std::cout << "done\n";
}
