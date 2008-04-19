/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include <cache.hh>

#include <iostream>
#include <stdexcept>
#include <iomanip>

struct record {
    int val;
};

generic::cache<std::string,record> cc;

record *cc_find(const std::string &key) 
{
    record * f;

    try {
        f = cc.find(key);
    }
    catch(record &e) {
        std::cout << "   record expired@" << &e << ": {" << e.val << "} (expired record has been thrown)" << std::endl;
        return f;
    }
    catch(std::runtime_error &r) {
        std::cout << "  " << r.what() << std::endl;
        return NULL;
    }

    std::cout << "   record found@" << std::hex << f << std::dec << ": {" << f->val << "}" << std::endl;
    return f;
}


int 
main(int argc, char *argv[])
{
    record r = { 1 };
    record t = { 2 };

    record *p , *q;

    std::cout << "create the record: {" << r.val << "} ...\n";
    std::cout << "create the record: {" << t.val << "} ...\n";

    p = cc.insert("ephemeral", r, 1);
    q = cc.insert("everlasting", t, 1000);

    std::cout << "   record@" << std::hex << p << std::dec << std::endl;
    std::cout << "   record@" << std::hex << q << std::dec << std::endl;

    std::cout << "search the record 'ephemeral'\n";
    p = cc_find("ephemeral");

    std::cout << "sleep 2 sec...\n";
    sleep(2);

    std::cout << "research the record 'ephemeral' ...\n";
    q = cc_find("ephemeral");

    std::cout << "update the timeout for the current record...\n";
    cc.update("ephemeral", 1);

    std::cout << "search the record again...\n";
    q = cc_find("ephemeral");

    std::cout << "search with an invalid key...\n";
    q = cc_find("unknown");

    sleep(2);

    std::cout << std::boolalpha << "dump the content of the cache...\n";
    generic::cache<std::string, record>::iterator it = cc.begin();
    for(; it != cc.end(); ++it) 
    {
        std::cout << "   key:" << (*it).first <<  
                     " value:" << static_cast<record>((*it).second).val << " expired:"  << (*it).second.expired() << std::endl;
    }

    return 0;
}
