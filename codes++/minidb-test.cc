/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include "minidb.hh"

#include <iostream>
#include <stdexcept>

struct record {
    int val;
};


minidb<std::string,record> db;


void db_find(const std::string &key) 
{
    record * f;

    try {
        f = db.find(key);
    }
    catch(record &e) {
        std::cout << "  record expired@" << &e << ": {" << e.val << "} (note expired is thrown)" << std::endl;

        db.update(key,10);  // update the timeout for the next search...
        return;
    }
    catch(std::runtime_error &r) {
        std::cout << "  " << r.what() << std::endl;
        return;
    }

    std::cout << "  record found@" << std::hex << f << std::dec << ": {" << f->val << "}" << std::endl;
}


int 
main(int argc, char *argv[])
{
    record r = { 1 };
    record *q;

    std::cout << "create the record: {" << r.val << "} ...\n";
    q = db.insert("test", r, 1);

    std::cout << "   record@" << std::hex << q << std::dec << std::endl;

    
    std::cout << "search the record...\n";
    db_find("test");

    std::cout << "sleep 2 sec...\n";
    sleep(2);

    std::cout << "research the expired record...\n";
    db_find("test");

    std::cout << "research the record, again...\n";
    db_find("test");

    std::cout << "search with invalid key...\n";
    db_find("unknown");

    return 0;
}
