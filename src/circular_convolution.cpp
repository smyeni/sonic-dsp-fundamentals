#include "circular_convolution.h"

namespace sonic {

std::vector<double> circular_convolve(
    const std::vector<double> &s,
    const std::vector<double> &h,
    std::size_t N)
{
    std::vector<double> y(N, 0.0);
    for (std::size_t n = 0; n < N; ++n) {
        for (std::size_t k = 0; k < h.size(); ++k) {
            if (k >= N) break;
            // (n - k) mod N, but safe for size_t
            std::size_t idx = (n >= k)? (n - k) : (N - (k - n) % N) % N;
            if (idx < s.size()) {
                y[n] += h[k] * s[idx];
            }
        }
    }
    return y;
}

} // namespace sonic
