#ifndef _SONIC_CONVOLUTION_H_
#define _SONIC_CONVOLUTION_H_

#include <vector>

namespace sonic
{

std::vector<double>  convolve( std::vector<double> s, std::vector<double> h );

}

#endif
