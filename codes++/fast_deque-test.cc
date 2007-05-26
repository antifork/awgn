#include <pthread.h>
#include "fast_deque.hh"

extra::fast_deque<int *> deck(100);    

void *thread_a(void *x)
{
    for(int i=1 ;i<1000;) {

        try {
            std::cout << "push: " << i << " " << deck.push_front((int *)i) << std::endl; 
        } catch(...) {
            i--;
        }

        i++;
        usleep(1000);

    }
    
    return NULL;
}

void *thread_b(void *x)
{
    for(;;) {
        int k;

        try {
            std::cout << "pop: " << (k=(int)deck.pop_back()) << std::endl; 
        } catch(...) {

        }

        deck.dump();
        usleep(2000);
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
