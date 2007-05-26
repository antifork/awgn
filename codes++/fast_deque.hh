/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef FAST_DEQUE_HH
#define FAST_DEQUE_HH

#ifdef X86_FEATURE_XMM2

#define mb()  asm volatile("mfence" ::: "memory")
#define rmb() asm volatile("lfence" ::: "memory")
#define wmb() asm volatile("sfence" ::: "memory")

#else

#warning "compile with -DX86_FEATURE_XMM2 to have optimized memory barriers"
#define mb()  asm volatile("lock; addl $0,0(%%esp)" ::: "memory")
#define rmb() asm volatile("lock; addl $0,0(%%esp)" ::: "memory")
#define wmb() asm volatile("lock; addl $0,0(%%esp)" ::: "memory")

#endif

#include <iostream> 
#include <stdexcept>

namespace extra {

/* note: this lockless version of stack/deque can only be used
         by a single producer and a single consumer. 
         Multiple consumers or producers are not allowed.
 */ 
        
    template <typename T> class fast_deque;
    template <typename T>
        class fast_deque<T *> {

        private:
            typedef T * ptr_T;

            size_t _size;

            volatile int _head;
            volatile int _tail;

            ptr_T * _arena;

            int next(int p) {
                return ( p == (int)_size ? 0 : p+1 );
            }

            fast_deque(fast_deque &);
            fast_deque& operator=(const fast_deque&);

        public:
            fast_deque(int s) : _size(s), _head(0), _tail(0), _arena(new ptr_T[s+1]) { }

            ~fast_deque() {  delete[] _arena; }

            int push_front(T *ptr) {
                int nh = next(_head);
                if (nh == _tail)
                    throw std::runtime_error(NULL);

                _arena[_head] = ptr;
                wmb();

                return _head = nh;
            }

            T * pop_back() {
                if (_head == _tail)
                    throw std::runtime_error(NULL);

                T *ret = _arena[_tail];
                rmb();

                _tail = next(_tail);
                return ret;
            }

            int size() const { return _size; }

            int depth() const { 
                int ret = _head-_tail;
                return ret >= 0 ? ret : ret + _size + 1; 
            }
        
            void dump() const { 
                std::cout << " size:"  << _size   << 
                             " depth:" << depth() << 
                             " head:"  << _head   << 
                             " tail:"  << _tail   << std::endl; 
            }
        };
}

#endif /* FAST_DEQUE_HH */

