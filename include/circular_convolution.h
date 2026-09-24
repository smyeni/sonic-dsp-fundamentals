#pragma once
#include <vector>
namespace sonic {
  std::vector<double> circular_convolve(
    const std::vector<double> &s,
    const std::vector<double> &h,
    std::size_t N
  );
}
