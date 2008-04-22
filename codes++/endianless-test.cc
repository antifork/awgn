#include "endianless.hh"
#include <iostream>

using namespace more;

int main()
{
    endianless<short int> x;
    x = 1;

    const endianless<int> y(x);

    endianless<long int> z;

    z = y;

    std::cout << "[host_byte_order]\n";

    std::cout << "   x=" << x << std::endl;
    std::cout << "   y=" << y << std::endl;
    std::cout << "   z=" << z << std::endl;

    std::cout << "[network_byte_order]\n";

    std::cout << "   x=" << x.get() << std::endl;
    std::cout << "   y=" << y.get() << std::endl;
    std::cout << "   z=" << z.get() << std::endl;

}
