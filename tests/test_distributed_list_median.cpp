#include "distributed_list_median.h"

#include <iostream>
#include <string>

void TestCase(std::string filename, float expected) 
{
    DistributedArray a(filename);
    assert(median(a) == expected);
}

int main(int argc, char const *argv[])
{
    TestCase("example0.txt", 5.5);
    TestCase("example1.txt", 8);

    return 0;
}