/* $Id$ */
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

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#include <iostream> 
#include <stdexcept>

namespace more 
{

// note: this decaffeinated/lockless version of deque can be only 
//       used by a single producer and a single consumer. 
//       Multiple consumers or producers are not allowed and 
//       should be avoided.
 
    template <typename T> class fast_deque;
    template <typename T>
        class fast_deque<T *> {

        public:
            typedef size_t              size_type;
            typedef T *                 value_type;

        private:
            fast_deque(fast_deque &);                   // uncopyable
            fast_deque& operator=(const fast_deque&);   // uncopyable

            value_type * _M_arena;
            size_type   _M_size;

            int _M_head;
            int _M_tail;

            int 
            next(int p) volatile 
            { return ( unlikely(p == (int)_M_size) ? 0 : p+1 ); }

        public:
            fast_deque(int s) 
            : _M_size(s), 
              _M_head(0), 
              _M_tail(0), 
              _M_arena(new value_type[s+1]) 
            {}

            ~fast_deque() 
            {  delete[] _M_arena; }

            int 
            push_front(T *ptr) volatile 
            {
                int nh = next(_M_head);
                if (unlikely(nh == _M_tail))
                    return -1;
                _M_arena[_M_head] = ptr;
                wmb();
                return _M_head = nh;
            }

            int 
            pop_back(T * &ret) volatile
            {
                if (unlikely(_M_head == _M_tail))
                    return -1;
                ret = _M_arena[_M_tail];
                rmb();
                _M_tail = next(_M_tail);
                return 0;
            }

            //  note: a lockless/thread-safe version of clear() cannot be implemented.
            //        Instead, you can use clear_push() and/or clear_pop(), that do the same
            //        of clear() and are supposed to be called from the same context as 
            //        that of the threads doing push_front()/pop_back() respectively.

            void 
            clear_push() volatile 
            { _M_head = _M_tail; }

            void 
            clear_pop() volatile 
            { _M_tail = _M_head; }

            size_type 
            max_size() const volatile 
            { return _M_size; }

            size_type 
            size() const volatile
            { 
                int ret = _M_head-_M_tail;
                return ret >= 0 ? ret : ret + _M_size + 1; 
            }

            bool
            empty() const volatile
            { return _M_head == _M_tail; }

            void 
            dump() const volatile
            { 
                std::cout << " size:"     << size()     << 
                             " max_size:" << max_size() << 
                             " head:"     << _M_head      << 
                             " tail:"     << _M_tail      << 
                             " empty:"    << empty()    << std::endl; 
            }

        };
} // namespace more

#endif /* FAST_DEQUE_HH */

