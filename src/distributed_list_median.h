#ifndef DISTRIBUTED_LIST_MEDIAN_H_
#define DISTRIBUTED_LIST_MEDIAN_H_

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Class mimicking distributed across nodes array
class DistributedArray
{
  public:
    size_t size;      // total amount of elements in all nodes
    size_t size_node; // amount of elements on each node. (Assuming it's equal)

    std::vector<std::vector<int> > nodes;

    // Read nodes data from a CSV-file filename.
    // Each line is a data for a separate node
    DistributedArray(std::string filename);

    // Basic operations with elements on a Distributed Array.
    void swap(size_t from, size_t to); // Swap two element values via global indeces
    int get(size_t index);             // Getter
    void set(int value, size_t index); // Setter

    void print();
};

// Order Statistics functions
// partition array between given indeces using first index as a pivot element,
// return index of the new place for first
size_t partition(DistributedArray array, size_t first, size_t last);

// partial k-th order of a partion between given indeces
int orderStatistic(DistributedArray array, size_t k, size_t first, size_t last);

int kthSmallest(DistributedArray array, size_t k);
int kthLargest(DistributedArray array, size_t k);
float median(DistributedArray array);

#endif // DISTRIBUTED_LIST_MEDIAN_H_