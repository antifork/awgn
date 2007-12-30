#include "probe.hh"

using namespace generic;

int main()
{
    probe hello;
    probe world(0);
    probe cctor = world;
    hello = world;
}
