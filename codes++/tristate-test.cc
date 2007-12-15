/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <iostream>
#include <tristate.hh>

int main(int argc, char *argv[])
{
    more::tristate x(more::tristate::True);

    if (x) {
        std::cout << "true  (ok)\n";
    }

    x = false;

    if (!x) {
        std::cout << "false (ok)\n";
    }

    if ( x == false ) {
        std::cout << "false (ok)\n";
    }   

    if ( x == x ) {
        std::cout << "operator==(tristate,tristate) (ok)\n"; 
    }

    x = more::tristate::Unknown;

    if (x == more::tristate::Unknown) {
        std::cout << "operator==(more::tristate::state) ok!\n";
    }

    std::cout << "operator std::string :" << (std::string)!x << std::endl;

    x = false;
    std::cout << "operator std::string :" << (std::string) x << std::endl;
    std::cout << "operator std::string :" << (std::string)!x << std::endl;

    return 0;
}
