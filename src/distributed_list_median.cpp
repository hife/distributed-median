#include "distributed_list_median.h"

#include <iostream>

// Read nodes data from a CSV file.
// Each list if data for a separate node
DistributedArray::DistributedArray(std::string filename)
{
    std::ifstream infile(filename);
    std::string line;

    if (! infile) return;

    size = 0;
    // Each line in CSV is a single pool member data
    while (std::getline(infile, line))
    {
        std::stringstream lineStream(line);

        // Add new pool member
        std::vector<int> node;
        nodes.push_back(node);

        // Parse pool member's data
        std::string data;
        while (std::getline(lineStream, data, ','))
        {
            nodes.back().push_back(std::stoi(data));
            size++;
        }
    }
    // Assuming all nodes are the same size.
    // This needs to be adjusted if the nodes are not even
    size_node = nodes.back().size();
}

// Distributed operations
// Swap two elements with given indeces properly selecting nodes
void DistributedArray::swap(size_t from, size_t to)
{
    if (from >= size) {
        throw std::invalid_argument( "Can't swap. Out of boundries 'from' index." );
    }

    if (to >= size) {
        throw std::invalid_argument( "Can't swap. Out of boundries 'to' index." );
    }

    // Assumption: each node has size_node number of elements
    std::swap(
        nodes[from / size_node][from % size_node],
        nodes[to / size_node][to % size_node]);
}

// get the corresponding index properly selecting nodes
int DistributedArray::get(size_t index)
{
    if (index >= size || index < 0) {
        throw std::invalid_argument( "Index is out of boundries." );    
    }

    return nodes[index / size_node][index % size_node];
}

// get the corresponding element value properly selecting nodes
void DistributedArray::set(int value, size_t index)
{
    if (index >= size || index < 0) {
        throw std::invalid_argument( "Index is out of boundries." );    
    }
    
    nodes[index / size_node][index % size_node] = value;
}

void DistributedArray::print()
{
    for (size_t i = 0; i < nodes.size(); i++)
    {
        std::cout << "Node " << i << ":";

        for (size_t j = 0; j < nodes[i].size(); j++)
        {
            std::cout << " " << nodes[i][j];
        }
        std::cout << std::endl;
    }
}

// Order Statistics functions
// partition array between given indeces using first index as a pivot element,
// return index of the new place for first
size_t partition(DistributedArray array, size_t first, size_t last)
{
    int pivot = array.get(first);
    size_t pivotPosition = first;

    first++;
    while (first <= last)
    {
        // Check right
        while (first <= last && array.get(first) > pivot)
            first++;

        // Check left
        while (last >= first && array.get(last) <= pivot)
            last--;

        // Move into the proper position
        if (first > last)
        {
            array.swap(pivotPosition, last);
        }
        else
        {
            array.swap(first, last);
        }
    }
    return last;
}

int orderStatistic(DistributedArray array, size_t k, size_t first, size_t last)
{
    if (k > array.size) {
        throw std::invalid_argument( "Requested order k is out of boundries." );    
    }

    size_t pivotPosition = partition(array, first, last);

    // Check if the element is in the desired position k
    if (pivotPosition == k - 1)
        return array.get(k - 1);

    return 
        pivotPosition > k - 1 ? 
            orderStatistic(array, k, first, pivotPosition - 1) : 
            orderStatistic(array, k, pivotPosition + 1, last);
}

int kthSmallest(DistributedArray array, size_t k)
{
    return orderStatistic(array, k, 0, array.size - 1);
}

int kthLargest(DistributedArray array, size_t k)
{
    return orderStatistic(array, array.size - k + 1, 0, array.size - 1);
}

float median(DistributedArray array)
{
    // Median of the array will be middle element 
    // or avarage of two middle elements in case of even size of array 
    size_t mid = array.size / 2;

    // uneven size
    if (array.size % 2)
        return kthSmallest(array, mid + 1);

    // even size
    return float(kthSmallest(array, mid) + kthLargest(array, mid)) / 2;
}
