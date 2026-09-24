#ifndef _SONIC_CONVOLUTION_H_
#define _SONIC_CONVOLUTION_H_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace sonic
{

std::vector<double> load_column(const std::string& path, int col_idx);
std::vector<double> convolve( const std::vector<double> &s, const std::vector<double> &h );

}

#endif
