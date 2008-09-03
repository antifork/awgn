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
#include <vector>
#include <buffer.hh>

#define BUFSIZE     65536

template <int n>
struct buffer : more::buffer<int,n> {};

int main()
{
    // more::buffer<int, BUFSIZE> x;

    buffer<BUFSIZE> x;

    std::cout << "size:" << x.size() << " static_size:" << x.static_size <<  " max_size:" << x.max_size() << std::endl;
    std::cout << "empty:" << x.empty() << std::endl;

    x.pop_back();
    x.pop_front();

    x.commit(4);

    std::cout << "size:" << x.size() << " static_size:" << x.static_size <<  std::endl;
    std::cout << "empty:" << x.empty() << std::endl;

    std::cout << "discarding 2 pods...\n";
    x.discard(2);

    std::cout << "size:" << x.size() << " static_size:" << x.static_size <<  std::endl;
    std::cout << "empty:" << x.empty() << std::endl;

    std::cout << "feeding...\n";

    x.commit(5);

    x[0] = 0;
    x[1] = 1;
    x[2] = 2;
    x[3] = 3;
    x[4] = 4;

    std::cout << "size:" << x.size() << " static_size:" << x.static_size <<  std::endl;
    std::cout << "empty:" << x.empty() << std::endl;

    std::cout << "x[0] = " << x[0] << std::endl;
    std::cout << "x[1] = " << x[1] << std::endl;
    std::cout << "x[2] = " << x[2] << std::endl;
    std::cout << "x[3] = " << x[3] << std::endl;
    std::cout << "x[4] = " << x[4] << std::endl;

    std::cout << "discarding 1 element...\n";

    x.discard(1);

    std::cout << "x[0] = " << x[0] << std::endl;
    std::cout << "x[1] = " << x[1] << std::endl;
    std::cout << "x[2] = " << x[2] << std::endl;
    std::cout << "x[3] = " << x[3] << std::endl;

    more::buffer<int, BUFSIZE> y; 

    std::cout << "x.size()=" << x.size() << " y.size()=" << y.size() << std::endl;
    std::cout << "swapping...\n";

    y.swap(x);

    std::cout << "x.size()=" << x.size() << " y.size()=" << y.size() << std::endl;

    std::cout << "3 push_back to x...\n";

    x.push_back(0);
    x.push_back(1);
    x.push_back(2);

    std::cout << "x.size()=" << x.size() << std::endl;

    std::cout << "3 pop_back from x...\n";

    x.pop_back();
    x.pop_back();
    x.pop_back();

    std::cout << "x.size()=" << x.size() << std::endl;

    std::cout << "1 push_back to, 1 pop_front from  x...\n";
    
    x.push_back(10);
    x.pop_front();

    std::cout << "x.size()=" << x.size() << std::endl;

    std::cout << "2 push_back, preparing x...\n";

    x.push_back(100);
    x.push_back(101);

    std::cout << "x.size()=" << x.size() << std::endl;

    more::buffer<int,BUFSIZE>::iterator it = x.begin();
    for(; it != x.end(); it++) {
        std::cout << "elem:" << *it << std::endl;
    }

    std::cout << "x == x ? " << (x == x) << std::endl;

    more::buffer<int,65538> copy;

    copy = x;

    std::cout << "x == copy ? " << (x == copy) << std::endl;

    // erase test

    more::buffer<int, 10> erase_test;

    erase_test[3] = 1;
    erase_test[4] = 1;
    erase_test[5] = 1;
    erase_test[6] = 1;

    erase_test.commit(7);
    erase_test.discard(3);

    std::cout << "erase_test.size()=" << erase_test.size() << std::endl;

    std::cout << erase_test[0] << "," << erase_test[1] << "," << erase_test[2] << "," << erase_test[3] << std::endl;

    // erase nothing 1)
    std::cout << "erase nothing:\n";
    erase_test.erase(&erase_test[0]-3, &erase_test[3]-3);
    std::cout << "erase_test.size()=" << erase_test.size() << std::endl;

    // erase the first... 2)

    std::cout << "erase the first:\n";
    erase_test.erase(&erase_test[0]-3, &erase_test[4]-3);
    std::cout << "erase_test.size()=" << erase_test.size() << std::endl;

    // push element...
    std::cout << "push element:\n";
    erase_test.push_front(1);
    std::cout << "erase_test.size()=" << erase_test.size() << std::endl;

    // erase in the middle  3)
    std::cout << "erase in the middle:\n";
    erase_test.erase(&erase_test[1], &erase_test[3]);
    std::cout << "erase_test.size()=" << erase_test.size() << std::endl;

    // erase in the end.. 4)

    std::cout << "erase in the end:\n";
    erase_test.erase( &erase_test[4]-3, &erase_test[5]-3);
    std::cout << "erase_test.size()=" << erase_test.size() << std::endl;

    // erase nothing 5)

    std::cout << "erase nothing:\n";
    erase_test.erase( &erase_test[7]-3, &erase_test[8]-3);
    std::cout << "erase_test.size()=" << erase_test.size() << std::endl;

    // erase everything...
    std::cout << "erase everything:\n";
    erase_test.erase(erase_test.begin(), erase_test.end());
    std::cout << "erase_test.size()=" << erase_test.size() << std::endl;

    // append test...

    more::buffer<int,10> a;
    more::buffer<int,10> b;

    b.push_back(1);
    b.push_back(2);
    b.push_back(3);

    a.append(b);
    std::cout << "a.size()=" << a.size() << std::endl;

    a.append(b);
    std::cout << "a.size()=" << a.size() << std::endl;

    a.append(b);
    std::cout << "a.size()=" << a.size() << std::endl;

    a.append(b);
    std::cout << "a.size()=" << a.size() << std::endl;

    a.append(b.begin(), b.end());
    std::cout << "a.size()=" << a.size() << std::endl;

    // builder test (from vector)

    std::vector<int> abc;

    abc.push_back(0);
    abc.push_back(1);
    abc.push_back(2);
    abc.push_back(3);
    abc.push_back(4);

    more::buffer<int, 4> h(abc);

    std::cout << "h.size()=" << h.size() << std::endl;

}
