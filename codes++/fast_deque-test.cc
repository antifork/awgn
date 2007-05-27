/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include <pthread.h>
#include "fast_deque.hh"

extra::fast_deque<int> deck(100);    

void *thread_a(void *x)
{
    for(int i=1 ;i<1000;) {

        try {
            std::cout << "push: " << i << " " << deck.push_front((int *)i) << std::endl; 
        } catch(...) {
            std::cout << "BANG!\n";
            i--;
        }

        i++;
        usleep(10000);

    }

    exit(0);    
    return NULL;
}

void *thread_b(void *x)
{
    for(;;) {
        int k;

        try {
            std::cout << "pop: " << (k=(int)deck.pop_back()) << std::endl; 
        } catch(...) {
            std::cout << "EMPTY!\n";
        }

        usleep(100);
        deck.dump();
    }

    return NULL;
}


int main()
{
    pthread_t a;
    pthread_t b;

    pthread_create(&a, NULL, thread_a, NULL);
    pthread_create(&b, NULL, thread_b, NULL);

    pthread_join(a,NULL);
    pthread_join(b,NULL);
}
