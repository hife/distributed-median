#include "distributed_list_median.h"

#include <iostream>
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
    std::cout << median(a) << std::endl;

    return 0;
}