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
#include <ostream_policer.h>

class Test : protected more::osp<Test>
{
    public:
    Test() 
    {
        cout() << "hello world\n";
    }

    using more::osp<Test>::set_ostream; 
};

int main()
{
    Test::set_ostream(std::cerr);
    Test x;
}
