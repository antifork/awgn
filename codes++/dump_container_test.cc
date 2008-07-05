#include <dump_container.hh>
#include <vector>
#include <iostream>

int 
main(int argc, char *argv[])
{
    std::cout << "string      :" << std::string("hello world") << std::endl; 

    std::vector<int> v1;

    v1.push_back(0);
    v1.push_back(1);
    v1.push_back(2);

    std::cout << std::dump_container_sep(',');
    std::cout << "vector<int> :" << v1 << std::endl;

    std::vector<unsigned char> l1;

    l1.push_back('A');
    l1.push_back('B');
    l1.push_back('C');

    std::cout << std::dump_container_sep();
    std::cout << "list<char>  :" << l1 << std::endl;

    return 0;
}
