#include "itoa.hh"

#include <sys/time.h>
#include <time.h>

int main()
{
      struct timeval tv;
      gettimeofday(&tv,NULL);

      std::cout << extra::itoa(tv.tv_sec,"%d") << std::endl; 
}

