# Median of Distributed Array

You have N nodes where N > 2. Each node has an array of many integers; 
each array is of the same length. Assume that the arrays are so large, 
that they cannot all be copied in their entirety to one node.

Simulate the scenario above, and write a solution that will return 
the single median value of all of the integers across all of the arrays. 

Notes:
- The median of an even quantity of numbers is the average of the two center values. 
- Your program should accept a filename as its first argument. 
- The file will contain one line per node. 
- Each line is a simple comma-separated list of ints. 
- Your program should print the median to stdout. 

# Solution 
Using order statistics (https://en.wikipedia.org/wiki/Order_statistic) 
k-th smallest, where k is pointing to the middle element(s)

# Build:
    make

# Run
    dmedian FILE

# Test
    cd tests
    make
    ./run_tests

# Clean:
    make clean
