/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <concept.hh>
#include <iostream>

using namespace more::basic_concepts;

struct test_class {

    bool operator==(const test_class &b)
    { return true; }

    test_class &
    operator++(int)
    {
        return *this;
    }
};

template <typename T>
bool test_template_function(T x, T y)
{
    FUNCTION_REQUIRES_CONCEPT(T, PostIncrementableConcept);
    return true;
}

template <typename T>
struct test_template_class 
{
    CLASS_REQUIRES_CONCEPT(T, EqualityComparableConcept);
    CLASS_REQUIRES_CONCEPT(T, PostIncrementableConcept);
    // CLASS_REQUIRES_CONCEPT(T, PreIncrementableConcept);

    void hello_world() 
    { std::cout << __PRETTY_FUNCTION__  << std::endl; } 
};

int main()
{
    test_class a, b;
    test_template_function(a,b);

    test_template_class<test_class>().hello_world(); 

}
