/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef _BUFFER_HH_
#define _BUFFER_HH_ 

#include <iostream>
#include <static_assert.hh>

namespace more { 

    // buffer, a slightly modified version of
    // boost::array written by Nico Josuttis

    template <typename T, std::size_t N>
    class buffer
    {

    public:
        typedef T                                       value_type;
        typedef T *                                     iterator;
        typedef const T *                               const_iterator;
        typedef std::reverse_iterator<iterator>         reverse_iterator;
        typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
        typedef T &                                     reference;
        typedef const T &                               const_reference;
        typedef std::size_t                             size_type;
        typedef std::ptrdiff_t                          difference_type;

        static const size_type  static_size = N;

        buffer()
        : _M_buffer(),
          _M_begin(_M_buffer),
          _M_end(_M_buffer)
        {}

        ~buffer()
        {}

        // assignments... and copy constructors 
        //
        template <std::size_t M>
        buffer<T,N>& operator= (const buffer<T,M>& rhs) {

            static_assert( N>=M , operator_eq, incongruence, between, buffer, sizes);
        
            _M_begin = const_cast<iterator>(rhs.begin());
            _M_end = const_cast<iterator>(rhs.end());

            // copy data available from rhs
            //
            std::copy(rhs.begin(),rhs.end(), begin());
            return *this;
        }

        template <std::size_t M>
        buffer(const buffer<T,M> & rhs)
        : _M_buffer(),
          _M_begin(rhs.begin()),
          _M_end(rhs.end())
        {
            static_assert( N>=M , operator_eq, incongruence, between, buffer, sizes);

            // copy data available from rhs
            //
            std::copy(rhs.begin(),rhs.end(), begin());
        }

        // builder from other containers
        //

        template <typename V>
        buffer(const V &v)
        : _M_buffer(),
          _M_begin(_M_buffer),
          _M_end(_M_buffer)
        {
            typename V::const_iterator it = v.begin();
            for (; it != v.end(); ++it)
                this->push_back(*it);
        }

        // Iterators of data...
        //
        iterator 
        begin() 
        { return _M_begin; }

        const_iterator 
        begin() const 
        { return _M_begin; }

        iterator 
        end() 
        { return _M_end; }

        const_iterator 
        end() const 
        { return _M_end; }

        // return the number of elements in the buffer
        //
        size_type size() const 
        { return _M_end - _M_begin; }

        // return the max. size available for writing/appending data
        //
        size_type max_size() const
        { return _M_buffer + N - _M_end; } 

        // commit n-elements after a raw-write...
        //
        void commit(size_type n)
        { _M_end += std::min(this->max_size(), n); }

        // discard n-elements from the beginning...
        //
        void discard(size_type n)
        { _M_begin += std::min(this->size(), n); }

        // return true if buffer is empty
        //
        bool empty() const
        { return !(_M_end-_M_begin); }

        // erase data from buffer...
        //
        iterator
        erase(iterator _start, iterator _end)
        {
            if ( _start <  _M_begin &&
                 _end   <= _M_begin ) {
                return _M_begin;
            }
            if ( _start <= _M_begin &&
                 _end   <= _M_end ) {
                _M_begin = _end;
                return _M_begin;
            }
            if ( _start <  _M_end &&
                 _end   <  _M_end ) {
                std::copy(_end, _M_end, _start);
                _M_end -= _end - _start;
                return _end;
            }
            if ( _start <  _M_end &&
                 _end   >= _M_end ) {
                _M_end = _start;
                return _M_end;
            }

            return _M_end;
        }

        iterator
        erase(iterator _elem)
        { return this->erase(_elem, _elem+1); }

        // flush data of buffer...
        //
        void clear()
        { _M_end = _M_begin; }

        // flush the whole buffer, reset begin and end...
        //
        void reset()
        { _M_begin = _M_buffer; _M_end = _M_buffer; }

        // operator[]: element access
        //
        reference
        operator[](size_type i)
        { return _M_begin[i]; }

        const_reference
        operator[](size_type i) const
        { return _M_begin [i]; }

        // front element
        //
        reference front()
        { return _M_begin[0]; }

        const_reference front() const
        { return _M_begin[0]; }

        // back element
        //
        reference back()
        { return _M_end[-1]; }

        const_reference back() const
        { return _M_end[-1]; }

        // swap (note: linear complexity) (ala boost)
        //
        void 
        swap (buffer<T,N>& rhs) {
            std::swap_ranges(_M_buffer,_M_buffer+N,rhs._M_buffer);
            std::swap(_M_begin, rhs._M_begin);
            std::swap(_M_end, rhs._M_end);
        }
        
        // move data inside the buffer
        //
        void shift_begin()
        {
            if (_M_begin == _M_buffer)
                return;

            size_type s = this->size();
            std::copy(_M_begin, _M_end, _M_buffer);

            _M_begin = _M_buffer;
            _M_end = _M_buffer + s;
        }
        void shift_end()
        {
            if (_M_end == &_M_buffer[N])
                return;

            size_type s = this->size();
            std::copy_backward(_M_begin, _M_end, _M_buffer + N );

            _M_begin = _M_buffer + N - s;
            _M_end   = _M_buffer + N;
        }
        void shift_middle()
        {
            size_type s = this->size();

            if ( _M_begin == _M_buffer + ((N-s)/2) ) {
                return;
            }
            if ( _M_begin > _M_buffer + ((N-s)/2) ) {
                std::copy(_M_begin, _M_end, _M_buffer + (N-s)/2 );
            }
            else {
                std::copy_backward(_M_begin, _M_end, _M_buffer + (N-s)/2 + s );
            }

            _M_begin = _M_buffer + (N-s)/2 ;
            _M_end   = _M_buffer + (N-s)/2 + s;
        }
      
        // direct access to data 
        //
        const T* 
        data() const 
        { return _M_begin; }

        T* 
        data() 
        { return _M_begin; }

        // use array as C array (direct read/write access to raw buffer)
        //
        T* 
        c_array() 
        { return _M_buffer; }

        const T* 
        c_array() const 
        { return _M_buffer; }

        // push/pop
        //
        int push_back(const T &e)
        {
            if ( !(_M_end < _M_buffer+N) ) {
                std::clog << "push_back: no space available in buffer!\n";
                return -1;
            }
            *(_M_end++) = e; 
            return 0;
        }

        int push_front(const T &e)
        { 
            if ( !(_M_begin > _M_buffer) ) {
                std::clog << "push_front: no space available in buffer!\n";
                return -1;
            }
            *(--_M_begin) = e;
            return 0;
        }

        int pop_back()
        { 
            if ( !(_M_end > _M_begin) ) {
                std::clog << "pop_back: empty buffer!\n";
                return -1;
            }
            _M_end--;
            return 0;
        }

        int pop_front()
        { 
            if ( !(_M_begin < _M_end) ) {
                std::clog << "pop_front: empty buffer!\n";
                return -1;
            }
            _M_begin++;
            return 0;
        }   

        // append...

        template <std::size_t M>
        int append(const more::buffer<T,M> &buf)
        {
            if ( buf.size() > this->max_size() ) {
                std::clog << "buffer::append: no space available!\n";
                return -1;
            }

            std::copy(buf.begin(), buf.end(), _M_end);
            _M_end += buf.end()-buf.begin();
            return 0;
        }

        int append(iterator _begin, iterator _end)
        {
            if ( (_end - _begin) > this->max_size()) {
                std::clog << "buffer::append: no space available!\n";
                return -1;
            }

            std::copy(_begin, _end, _M_end);
            _M_end += (_end - _begin);
            return 0;
        }

    private:
        T   _M_buffer[N];

        T * _M_begin;
        T * _M_end;
    };

    // comparisons

    template<class T, std::size_t N, std::size_t M >
    bool operator== (const buffer<T,N>& x, const buffer<T,M>& y) {
        if ( (x.end() - x.begin()) != (y.end()-y.begin()) )
             return false;
        return std::equal(x.begin(), x.end(), y.begin());
    }
    template<class T, std::size_t N, std::size_t M>
    bool operator< (const buffer<T,N>& x, const buffer<T,M>& y) {
        return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }
    template<class T, std::size_t N, std::size_t M>
    bool operator!= (const buffer<T,N>& x, const buffer<T,M>& y) {
        return !(x==y);
    }
    template<class T, std::size_t N, std::size_t M>
    bool operator> (const buffer<T,N>& x, const buffer<T,M>& y) {
        return y<x;
    }
    template<class T, std::size_t N, std::size_t M>
    bool operator<= (const buffer<T,N>& x, const buffer<T,M>& y) {
        return !(y<x);
    }
    template<class T, std::size_t N, std::size_t M>
    bool operator>= (const buffer<T,N>& x, const buffer<T,M>& y) {
        return !(x<y);
    }

    // global swap()
    template<class T, std::size_t N>
    inline void swap (buffer<T,N>& x, buffer<T,N>& y) {
        x.swap(y);
    }

} // namespace more

#endif /* _BUFFER_HH_ */
