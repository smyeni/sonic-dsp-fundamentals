#include <print>
#include <vector>
#include "convolution.h"

std::vector<double>  sonic::convolve( std::vector<double> x, std::vector<double> h )
{
    std::size_t L = x.size() + h.size() - 1;
    std::vector<double>  y(L,0);

    for (size_t n=0; n<L; ++n)
    {
        //Pin down n and sum over all k
        for ( size_t k=0; k < h.size(); k++ )
        {
            //n - k < 0 ??
            if ( n < k ) 
            {
                std::println( "n={} -> xn = -ve, hn={} => REJECT!", n,k );
                break;
            }

            if ( (n-k) < x.size() )
            {
                std::println( "n={} -> xn={}, hn={}", n,n-k,k );
                y[n] += h[k] * x[n-k]; 
            }
            else {
                std::println( "n={} -> xn={}, hn={} => REJECT!", n,n-k,k );
            }
        }
    }

    return y;
}
