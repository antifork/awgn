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

extra::fast_deque<int *> deck(1000);    

void *thread_a(void *x)
{
    int r;
    for(int i=1 ;i<1000;) {

        std::cout << "push: " << i << " " << (r=deck.push_front((int *)i)) << std::endl; 
        if (r >= 0)
            i++;
        usleep(1000);
    }

    exit(0);    
    return NULL;
}

void *thread_b(void *x)
{
    int *r;
    for(;;) {
        if ( deck.pop_back(r) >= 0 )
        std::cout << "pop: " << (int)r << std::endl; 
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
