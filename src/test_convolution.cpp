#include <print>
#include "convolution.h"

using namespace sonic;

int main()
{
    std::vector<double> s{1,2,3,4,5};     
    std::vector<double> h{6,9,3};     

    std::vector<double> y = convolve(s,h);
    std::println("Output length: {}", y.size());

    size_t n = 0;
    for (double val : y) {
        std::println("y[{}] = {}", n++, val);
    }

    return 0;
}
