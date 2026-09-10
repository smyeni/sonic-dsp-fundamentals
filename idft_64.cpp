#include <cmath>
#include <print>
#include <vector>
#include <complex>
#include <iomanip>
#include <fstream>
#include <iostream>

constexpr double PI = std::acos(-1.0); //smart sh*t

std::vector<std::complex<double>> idft(const std::vector<std::complex<double>>& H)
{
    const std::size_t N = H.size();

    std::vector<std::complex<double>> h(N);

    for (std::size_t n = 0; n < N; ++n)
    {
        for (std::size_t k = 0; k < N; ++k)
        {
            const double bin_tilt_angle = 2.0 * PI * static_cast<double>(k * n) / static_cast<double>(N);

            const std::complex<double> tilt_exponential = std::exp(std::complex<double>(0.0, bin_tilt_angle));

            h[n] += H[k] * tilt_exponential; //filters out the component we want, cancels the rest
        }

        h[n] /= static_cast<double>(N);
    }

    return h; //what in modern c++ makes this cool!!!
}

///////////////////////////////////////////////////////////////////////

int main()
{
    // Frequency-domain samples H[k]
    constexpr std::size_t N = 64;

    std::vector<std::complex<double>> H(N);

    /*
    // First experiment: all frequency samples equal to 1.
    for (std::size_t k = 0; k < N; ++k)
    {
        H[k] = 1.0;
    }
    */

    //Pass bins
    /*
	H[0]  = 1;
	H[1]  = 1;
	H[2]  = 1;
	H[3]  = 1;
	H[4]  = 1;
	H[5]  = 1;
	H[6]  = 1;
    */
    for (int n=0; n<7; n++) {
        H[n] = 1;
    }

    //Stop band bins
    for (int n=7; n<58; n++) {
        H[n] = 0;
    }

    //Pass bins
    /*
	H[58] = 1;
	H[59] = 1;
	H[60] = 1;
	H[61] = 1;
	H[62] = 1;
	H[63] = 1;
    */
    for (int n=58; n<H.size(); n++) {
        H[n] = 1;
    }

    std::println("Filter frequency response:");
    for (std::size_t k = 0; k < N; ++k)
    {
        std::println( "H[{}] = {} {} j{}", k, H[k].real(), H[k].imag() < 0 ? "-" : "+", std::abs(H[k].imag()) );
    }

    const auto h = idft(H);

    std::cout << std::fixed << std::setprecision(6);

    std::println("Time domain sequence:");
    for (std::size_t n = 0; n < N; ++n)
    {
        std::println( "h[{}] = {} {} j{}", n , h[n].real(), h[n].imag() < 0 ? "-" : "+" , std::abs(h[n].imag()) );
    }

    ////////////////////////////////////////////////

	std::ofstream h_file("h.csv");

	h_file << "k,h_real,h_imag\n";

	for (std::size_t k = 0; k < h.size(); ++k)
	{
		h_file << k << "," << h[k].real() << "," << h[k].imag() << "\n";
	}

    ////////////////////////////////////////////////

	std::ofstream H_file("H.csv");

	H_file << "k,H_real,H_imag\n";

	for (std::size_t k = 0; k < H.size(); ++k)
	{
		H_file << k << "," << H[k].real() << "," << H[k].imag() << "\n";
	}

    ////////////////////////////////////////////////

    return 0;
}
