/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include "fnv.hh"
#include <iostream>

int main()
{
    hash::fnv<32> fnv_32;

    std::cout << "init:  'hello world' => fnv-32a :" << fnv_32("hello world",5) << std::endl;
    std::cout << "reset: 'hello world' => fnv-32a :" << fnv_32("hello world",5, hash::fnv<32>::INIT) << std::endl;
    std::cout << "       'hello world' => fnv-32a :" << fnv_32( std::string("hello world")) << std::endl;
    std::cout << "       'hello world' => fnv-32a :" << fnv_32( std::string("hello world")) << std::endl;
    std::cout << "       'hello world' => fnv-32a :" << fnv_32( std::string("hello world")) << std::endl;
 
    hash::fnv<64> fnv_64;

    std::cout << "init:  'hello world' => fnv-64a :" << fnv_64("hello world",5) << std::endl;
    std::cout << "reset: 'hello world' => fnv-64a :" << fnv_64("hello world",5, hash::fnv<64>::INIT) << std::endl;
    std::cout << "       'hello world' => fnv-64a :" << fnv_64( std::string("hello world")) << std::endl;
    std::cout << "       'hello world' => fnv-64a :" << fnv_64( std::string("hello world")) << std::endl;
    std::cout << "       'hello world' => fnv-64a :" << fnv_64( std::string("hello world")) << std::endl;

    return 0;
}
