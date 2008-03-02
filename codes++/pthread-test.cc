/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include "pthread++.hh"

using namespace posix;

mutex xxx;

class Hello : public posix::pthread {

public:
    void *operator()()
    {
        posix::scoped_lock<mutex> lock(xxx);

        for(int i=0;i<3;i++) {
            std::cout << pthread_self() << " hello\n";
            sleep(1);
        }
        return NULL;
    }

    ~Hello()
    {
        this->cancel();        
    }
};

class World : public posix::pthread {

public:
    void *operator()()
    {
        posix::scoped_lock<mutex> lock(xxx);

        for(int i=0;i<3;i++) {
            std::cout << pthread_self() << " world\n";
            sleep(1);
        }
        return NULL;
    }

    ~World()
    {
        this->cancel();
    }
};

rw_mutex yyy;

class Reader : public posix::pthread {

public:
    void *operator()()
    {
        scoped_lock<rw_mutex, base_lock::reader> lock(yyy);

        for(int i=0;i<3;i++) {
            std::cout << pthread_self() << " reader!\n";
            sleep(1);
        }
        return NULL;
    }

    ~Reader()
    {
        this->cancel();
    }
};

class Writer : public posix::pthread {

public:
    void *operator()()
    {
        scoped_lock<rw_mutex, base_lock::writer> lock(yyy);

        for(int i=0;i<3;i++) {
            std::cout << pthread_self() << " writer!\n";
            sleep(1);
        }
        return NULL;
    }

    ~Writer()
    {
        this->cancel();
    }

};


int main(int argc, char *argv[])
{
    std::cout << "[*] basic mutex...\n";
    Hello *hello = new Hello;
    World *world = new World;

    hello->start();
    world->start();

    hello->join();
    world->join();
    
    delete hello;
    delete world;

    std::cout << "[*] reader mutex...\n";

    Reader *one = new Reader;
    Reader *two = new Reader;
    
    one->start();
    two->start();

    one->join();
    two->join();

    delete one;
    delete two;

    std::cout << "[*] reader/writer mutex...\n";

    one = new Reader;
    Writer *three = new Writer;

    one->start();
    three->start();

    one->join();
    three->join();

    delete one;
    delete three;

    return 0;
}
