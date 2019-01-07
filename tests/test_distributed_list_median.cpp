#include "distributed_list_median.h"

#include <iostream>
#include <string>

// A Few basic tests
// TODO: use a unit test framework (for ex.: gtest)

void TestDistributedArrayFromFile(std::string filename) {
    DistributedArray a(filename);
    std::cout << "* DistributedArrayFromFile - OK" << std::endl;
} 

void TestDistributedArrayFromVectors() {
    std::vector<int64_t> v0 = {1, 2, 3, 4};
    std::vector<int64_t> v1 = {5, 6, 7, 8};
    std::vector<int64_t> v2 = {9, 10, 11, 12};

    std::vector<std::vector<int64_t> > data{v0, v1, v2};

    DistributedArray a(data);
    assert(a[2] == 3);
    assert(a[8] == 9);
    std::cout << "* DistributedArray::[] get - OK" << std::endl;

    a[2] = 5;
    assert(a[2] == 5);
 
    a[2] = 3;
    assert(a[2] == 3);
    std::cout << "* DistributedArray::[] set - OK" << std::endl;

    a.swap(1, 9);
    assert(a[1] == 10);
    assert(a[9] == 2);

    a.swap(1, 9);
    assert(a[1] == 2);
    assert(a[9] == 10);
    
    std::cout << "* DistributedArray::swap - OK" << std::endl;
} 

void TestDistributedKthSmallest(std::string filename, size_t k, float expected) 
{
    DistributedArray a(filename);
    assert(kthSmallest(a, k) == expected);
    std::cout << "* K-th Smallest for " << filename << " - OK" << std::endl;
}

void TestDistributedKthLargest(std::string filename, size_t k, float expected) 
{
    DistributedArray a(filename);
    assert(kthLargest(a, k) == expected);
    std::cout << "* K-th Largest for " << filename << " - OK" << std::endl;
}

void TestDistributedMedian(std::string filename, float expected) 
{
    DistributedArray a(filename);
    assert(median(a) == expected);
    std::cout << "* Median for " << filename << " - OK" << std::endl;
}

int main(int argc, char const *argv[])
{
    // Basic Unit Tests
    TestDistributedArrayFromFile("example0.txt");
    TestDistributedArrayFromVectors();

    // Functional Tests
    TestDistributedKthSmallest("example0.txt", 7, 7);
    TestDistributedKthSmallest("example1.txt", 12, 12);

    TestDistributedKthLargest("example0.txt", 2, 9);
    TestDistributedKthLargest("example1.txt", 2, 14);

    TestDistributedMedian("example0.txt", 5.5);
    TestDistributedMedian("example1.txt", 8);

    return 0;
}