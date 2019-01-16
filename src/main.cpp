#include "distributed_list_median.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

int main(int argc, char const *argv[])
{
    if(argc == 1) {
        std::cout << "Please, specify filename with CSV data for nodes." << std::endl;
        std::cout << "Usage: " << argv[0] << " file" << std::endl;

        return 1;
    }

    std::string filename(argv[1]);

    DistributedArray a(filename);
    double m = median(a); 

    // Double without trailing zeroes
    std::cout << std::fixed;
    std::cout << std::setprecision((std::abs(m - int64_t(m)) > 0) ? 1 : 0); 
    
    std::cout << m << std::endl;

    return 0;
}