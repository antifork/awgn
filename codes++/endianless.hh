#ifndef ENDIANLESS_HH
#define ENDIANLESS_HH

#include <arpa/inet.h>

namespace more {
        
    template <typename T> struct endianless_traits;
    template <> struct endianless_traits<short unsigned int>
    {
        typedef short unsigned int type;
    };
    template <> struct endianless_traits<short int>
    {
        typedef short int type;
    };
    template <> struct endianless_traits<unsigned int>
    {
        typedef unsigned int type;
    };
    template <> struct endianless_traits<int>
    {
        typedef int type;
    };
    template <> struct endianless_traits<long unsigned int>
    {
        typedef long unsigned int type;
    };
    template <> struct endianless_traits<long int>
    {
        typedef long int type;
    };
    template <> struct endianless_traits<long long unsigned int>
    {
        typedef long long unsigned int type;
    };
    template <> struct endianless_traits<long long int>
    {
        typedef long long int type;
    };

    template <typename T> 
    class endianless
    {
    public:

        template <int n>
        struct size2type {
            enum { value = n };
        };

        endianless() 
        : _M_data (T())
        {}

        explicit endianless(const endianless &rhs)
        : _M_data(rhs._M_data)
        {}

        explicit endianless(const T &rhs) 
        : _M_data(hton(rhs, size2type<sizeof(T)>()))
        {}

        endianless &
        operator=(const T &rhs)
        {
            _M_data = hton(rhs, size2type<sizeof(T)>());
            return *this;
        }
        endianless &
        operator=(const endianless &rhs)
        {
            if ( this != &rhs)
                _M_data = rhs._M_data; 
            return *this;
        }

        operator T() const 
        { return ntoh(_M_data, size2type<sizeof(T)>()); }

        const T get() const
        { return _M_data; }

    private:

        template <typename U>
        const U hton(const U n,size2type<2>) const
        { return htons(n); }

        template <typename U>
        const U ntoh(const U n,size2type<2>) const
        { return ntohs(n); }

        template <typename U>
        const U hton(const U n,size2type<4>) const
        { return htonl(n); }

        template <typename U>
        const U ntoh(const U n,size2type<4>) const 
        { return ntohl(n); }

        template <typename U>
        const U hton(const U n,size2type<8>) const
        { return (static_cast<U>(htonl (n)) << 32) + htonl (n >> 32); }

        template <typename U>
        const U ntoh(const U n,size2type<8>) const
        { return (static_cast<U>(ntohl (n)) << 32) + ntohl (n >> 32); }

        typename endianless_traits<T>::type _M_data;

    };

} // namespace more

#endif /* ENDIANLESS_HH */
