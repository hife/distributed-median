#ifndef DISTRIBUTED_LIST_MEDIAN_H_
#define DISTRIBUTED_LIST_MEDIAN_H_

#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Class mimicking distributed across nodes array
class DistributedArray
{
    size_t m_size;      // total amount of elements in all nodes
    size_t m_size_node; // amount of elements on each node. (Assuming it's equal)

    std::vector<std::vector<int64_t> > m_nodes;

  public:
    // Read nodes data from a CSV-file filename.
    // Each line is a data for a separate node
    DistributedArray(std::string);
    DistributedArray(const std::vector<std::vector<int64_t> >&);

    // Basic operations with elements on a Distributed Array.
    size_t size();
    size_t size_node();

    void swap(size_t from, size_t to); // Swap two element values via global indeces
    int64_t &operator[] (size_t);

    void print();
};

// Order Statistics functions
// partition array between given indeces using first index as a pivot element,
// return index of the new place for first
std::pair<size_t, size_t> partition(DistributedArray &a, size_t first, size_t last);
int64_t orderStatistic(DistributedArray &a, size_t k, size_t first, size_t last);
int64_t kthSmallest(DistributedArray &a, size_t k);
int64_t kthLargest(DistributedArray &a, size_t k);
double median(DistributedArray &a);

#endif // DISTRIBUTED_LIST_MEDIAN_H_