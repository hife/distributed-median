#include "distributed_list_median.h"

// Read nodes data from a CSV file.
// Each list if data for a separate node
DistributedArray::DistributedArray(std::string filename)
{
    std::ifstream infile(filename);
    std::string line;

    if (infile.fail()) 
    { 
        throw std::invalid_argument("File doesn't exist");
    }

    m_size = 0;
    // Each line in CSV is a single pool member data
    while (std::getline(infile, line))
    {
        if (line.empty()) return;

        std::stringstream lineStream(line);

        // Add new pool member
        std::vector<int64_t> node;
        m_nodes.push_back(node);

        // Parse pool member's data
        std::string data;
        while (std::getline(lineStream, data, ','))
        {
            // std::cout << "Data: " << data << std::endl;
            m_nodes.back().push_back(std::stoll(data));
            m_size++;
        }
    }
    // Assuming all nodes are the same size.
    // This needs to be adjusted if the nodes are not even
    m_size_node = m_nodes.back().size();
}

// Create nodes from a vector of vectors
// Assuming all the nodes are the same size
DistributedArray::DistributedArray(const std::vector<std::vector<int64_t> > &data)
{
    m_size = 0;
    m_size_node = 0;

    for(size_t i = 0; i < data.size(); i++) {
        m_nodes.push_back(data[i]);

        m_size_node = data[i].size();
        m_size += m_size_node;
    }
}

size_t DistributedArray::size() {
    return m_size;
}

size_t DistributedArray::size_node() {
    return m_size_node;
}

// Distributed operations
// Swap two elements with given indeces properly selecting nodes
void DistributedArray::swap(size_t from, size_t to)
{
    if (from >= m_size) {
        throw std::invalid_argument( "Can't swap. Out of boundries 'from' index." );
    }

    if (to >= m_size) {
        throw std::invalid_argument( "Can't swap. Out of boundries 'to' index." );
    }

    // Assumption: each node has size_node number of elements
    std::swap(
        m_nodes[from / m_size_node][from % m_size_node],
        m_nodes[to / m_size_node][to % m_size_node]);
}

// get the corresponding index properly selecting nodes
int64_t &DistributedArray::operator[](size_t index)
{
    if (index >= m_size) {
        throw std::invalid_argument( "Index is out of boundries." );    
    }

    return m_nodes[index / m_size_node][index % m_size_node];
}

void DistributedArray::print()
{
    for (size_t i = 0; i < m_nodes.size(); i++)
    {
        std::cout << "Node " << i << ":";

        for (size_t j = 0; j < m_nodes[i].size(); j++)
        {
            std::cout << " " << m_nodes[i][j];
        }
        std::cout << std::endl;
    }
}

// Order Statistics functions
// Partition function within ranges of first to last and 
// return pair of two indexes smaller 
std::pair<size_t, size_t> partition(DistributedArray &a, size_t first, size_t last) { 
    // Middle element as pivot
    size_t pivotIndex = (first + last) / 2;
    int64_t pivot = a[pivotIndex];

    size_t smaller = first; // last index of elements smaller than pivot
    size_t equal = first;   // last index of elements equal to pivot
    size_t larger = last;   // first index of elements larger than pivot
  
    while (equal <= larger) 
    {
        if (a[equal] < pivot) {
            a.swap(smaller++, equal++);
        }
        else if (a[equal] == pivot) {
            equal++;
        }
        else {
            a.swap(equal, larger);
            larger--;
        }
    }

    return std::make_pair(smaller, larger);
}

int64_t orderStatistic(DistributedArray &a, size_t k, size_t first, size_t last)
{
    if (k > a.size()) {
        throw std::invalid_argument( "Requested order k is out of boundries." );    
    }

    std::pair<size_t, size_t> pivotPosition = partition(a, first, last);

    // Check if the element is in the desired position k
    if (pivotPosition.first <= k - 1 && pivotPosition.second >= k - 1)
        return a[k - 1];

    return 
        pivotPosition.first > k - 1 ? 
            orderStatistic(a, k, first, pivotPosition.first) : 
            orderStatistic(a, k, pivotPosition.second, last);
}

int64_t kthSmallest(DistributedArray &a, size_t k)
{
    return orderStatistic(a, k, 0, a.size() - 1);
}

int64_t kthLargest(DistributedArray &a, size_t k)
{
    return orderStatistic(a, a.size() - k + 1, 0, a.size() - 1);
}

double median(DistributedArray &a)
{
    // Median of the array will be middle element 
    // or avarage of two middle elements in case of even size of array 
    size_t mid = a.size() / 2;

    // uneven size
    if (a.size() % 2)
        return kthSmallest(a, mid + 1);

    // even size
    return double(kthSmallest(a, mid) + kthLargest(a, mid)) / 2;
}