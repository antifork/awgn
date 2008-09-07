/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef _CONCEPT_HH_
#define _CONCEPT_HH_ 

#define FUNCTION_REQUIRES_CONCEPT(t,C)    \
    void (C<t>::*ptr_ ## C)() __attribute__((unused)) = &C<t>::constraints

#define CLASS_REQUIRES_CONCEPT(t,C)       \
    template <void (C<t>::*ptr_value)()> struct class_ ## C {}; \
    typedef class_ ## C< & C<t>::constraints> C ## _type

// some basic concepts...
//

namespace more { namespace basic_concepts {

    template <class T>
    struct EqualityComparableConcept
    {
        T a, b;

        void constraints()
        {
            a == b;
        }
    };

    template <class T>
    struct PreIncrementableConcept
    {
        T a;
        void constraints()
        {
            ++a;
        }
    };

    template <class T>
    struct PostIncrementableConcept
    {
        T a;
        void constraints()
        {
            a++;
        }
    };

}}

#endif /* _CONCEPT_HH_ */
