/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <cstdlib>
#include <kv_file.hh>

TYPEMAP_KEY(number_0);
TYPEMAP_KEY(number_1);
TYPEMAP_KEY(number_2);

TYPEMAP_KEY(string_0);
TYPEMAP_KEY(string_1);

TYPEMAP_KEY(boolean_0);
TYPEMAP_KEY(boolean_1);

typedef TYPEMAP(number_0,  int,
                number_1,  unsigned int,
                number_2,  unsigned int, 
                boolean_0, bool,
                boolean_1, bool,
                string_0,  std::string,
                string_1,  std::string) MYSCRIPT;

struct myscript : public more::kv_file<MYSCRIPT>
{};

int
main(int argc, char *argv[])
{
    myscript abc;

    if ( !abc.parse("kv_test.txt", true /* strict */ ) ) {
        return -1;
    }

    std::cout << "-> " << number_0::value() << "  :" <<  abc.get<number_0>()  << std::endl;
    std::cout << "-> " << number_1::value() << "  :" <<  abc.get<number_1>()  << std::endl;
    std::cout << "-> " << number_2::value() << "  :" <<  abc.get<number_2>()  << std::endl;
    std::cout << "-> " << boolean_0::value() << " :" <<  abc.get<boolean_0>() << std::endl;
    std::cout << "-> " << boolean_1::value() << " :" <<  abc.get<boolean_1>() << std::endl;
    std::cout << "-> " << string_0::value() << "  :" <<  abc.get<string_0>()  << std::endl;
    std::cout << "-> " << string_1::value() << "  :" <<  abc.get<string_1>()  << std::endl;

    return 0;
}

