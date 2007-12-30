#ifndef PROBE_HH
#define PROBE_HH

#include <iostream>

namespace generic 
{

#define probe_dump(x) std::cout << x << " :" << this << ": " << __PRETTY_FUNCTION__ << '\n'

    struct probe 
    {
        probe() 
        { 
            probe_dump("[C]"); 
        }
        probe(const probe &) 
        { 
            probe_dump("[!]"); 
        } 
        probe &operator=(const probe &) 
        { 
            probe_dump("[!]");
            return *this; 
        } 

        template <typename T1>
        probe(const T1 &) 
        { 
            probe_dump("[c]"); 
        } 
        template <typename T1, typename T2>
        probe(const T1 &, const T2 &) 
        {
            probe_dump("[c]"); 
        }
        template <typename T1, typename T2, typename T3>
        probe(const T1 &, const T2 &, const T3 &) 
        { 
            probe_dump("[c]"); 
        }
        template <typename T1, typename T2, typename T3,
                  typename T4>
        probe(const T1 &, const T2 &, const T3 &,
              const T4 &) 
        { 
            probe_dump("[c]"); 
        }
        template <typename T1, typename T2, typename T3,
                  typename T4, typename T5>
        probe(const T1 &, const T2 &, const T3 &,
              const T4 &, const T5 &) 
        { 
            probe_dump("[c]"); 
        }
        template <typename T1, typename T2, typename T3,
                  typename T4, typename T5, typename T6>
        probe(const T1 &, const T2 &, const T3 &,
              const T4 &, const T5 &, const T6 &) 
        { 
            probe_dump("[c]"); 
        }

        ~probe() 
        { 
            probe_dump("[d]"); 
        } 
    };

} // namespace generic

#endif /* PROBE_HH */

