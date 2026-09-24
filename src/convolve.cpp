#include <print>
#include <string>
#include <vector>
#include <complex>
#include <cstddef>
#include <fstream>
#include <sstream>
#include "convolution.h"

std::vector<std::complex<double>>
			convolve(const std::vector<std::complex<double>>& x,
					 const std::vector<std::complex<double>>& h)
{
    const std::size_t Ns = x.size();
    const std::size_t Nh = h.size();

    std::vector<std::complex<double>> y(Ns + Nh - 1);

    for (std::size_t n = 0; n < y.size(); ++n)
    {
        for (std::size_t m = 0; m < Nh; ++m)
        {
            if (n >= m && (n - m) < Ns)
            {
                y[n] += h[m] * x[n - m];
            }
        }
    }

    return y;
}

std::vector<std::complex<double>> 
              read_signal(const std::string& filename)
{
    std::ifstream file(filename);

    std::string line;
    std::getline(file, line); // discard header

    std::vector<std::complex<double>> x;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string index;
        std::string value;
        std::string phase;

        std::getline(ss, index, ',');
        std::getline(ss, value, ',');
        std::getline(ss, phase, ',');

        x.emplace_back(std::stod(value), 0.0);
    }

    return x;
}

std::vector<std::complex<double>> 
                  read_filter(const std::string& filename)
{
    std::ifstream file(filename);

    std::string line;
    std::getline(file, line); // discard header

    std::vector<std::complex<double>> h;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string index;
        std::string real;
        std::string imag;

        std::getline(ss, index, ',');
        std::getline(ss, real, ',');
        std::getline(ss, imag, ',');

        h.emplace_back(std::stod(real), std::stod(imag));
    }

    return h;
}

////////////////////////////////////////////////////////////

int main(const int argc, const char* argv[])
{
    if (argc != 3) {
        std::println("Expected 2 inputs!!");
        std::println("{} <signal_file.csv> <h_file.csv>", argv[0]);
        return 1;
	}

    const std::string s_filename = argv[1];
    const std::string h_filename = argv[2];

	auto s = read_signal(s_filename);
	auto h = read_filter(h_filename);

	//auto y = sonic::convolve(s, h);
	auto y = convolve(s, h);

    std::ofstream ofile("filtered_" + s_filename);
	std::println( ofile, "{},{},{}", "n", "y_Real", "y_Imag" );
	for (std::size_t n = 0; n < y.size(); ++n)
	{
		std::println(ofile, "{},{},{}", n, y[n].real(), y[n].imag());
	}

    return 0;
}
