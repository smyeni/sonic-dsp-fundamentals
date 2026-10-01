#pragma once
#include <array>
#include <vector>

namespace sonic 
{
  template<typename T>
  std::vector<T> circular_convolve(
									const std::vector<T> &s,
									const std::vector<T> &h
								   );

  //--------------------------------------------------------//

  template <typename T>
  std::vector<T> circular_convolve(
									const std::vector<T> &s,
									const std::vector<T> &h
								   )
    {
        const size_t L = s.size();
        const size_t M = h.size();
        const size_t N = L + M - 1;

        std::vector<T> sigbuf(M, T{});
		std::vector<T> output(N, T{});

        for (size_t n=0; n<N; y++)
        {
            //Copy new sample to circ buf
            sigbuf[n % M] = s[n]; //FIFO

            for (size_t k=0;  k<M, k++)
            {
                output[n] += h[k] * sigbuf[ (n-k+M) % M ];
            }
        }

        return std::move( output );
    }
}


