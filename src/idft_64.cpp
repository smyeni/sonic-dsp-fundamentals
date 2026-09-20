#include <cmath>
#include <print>
#include <vector>
#include <ranges>
#include <numbers>
#include <complex>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>

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

int main( int argc, char* argv[] )
{
    if (argc < 2)
    {
        std::println("ERROR! Expected N as input\n");
        return -1;
    }

    // Frequency-domain samples H[k]
    const std::size_t N = std::stoul( argv[1] );

    std::vector<std::complex<double>> H(N);

    /*
    // First experiment: all frequency samples equal to 1.
    for (std::size_t k = 0; k < N; ++k)
    {
        H[k] = 1.0;
    }
    */

    //Pass bins
    for (int n=0; n<7; n++) {
        H[n] = 1;
    }

    //Stop band bins
    for (int n=7; n<58; n++) {
        H[n] = 0;
    }

    //Pass bins
    for ( std::size_t n=58; n<H.size(); n++ ) {
        H[n] = 1;
    }

    std::println("Filter frequency response:");
    for (std::size_t k = 0; k < N; ++k)
    {
        std::println( "H[{}] = {} {} j{}", k, H[k].real(), H[k].imag() < 0 ? "-" : "+", std::abs(H[k].imag()) );
    }

    auto h = idft(H);

    //Window the damn (FIR filter's) impulse response
    std::vector<double> hamming(N);
    std::vector< std::complex<double> > h_windowed(N);

//-----------Centering the impulse response ---------------------

/*
std::vector<std::complex<double>> h_centered(N);
for (std::size_t n = 0; n < N; ++n)
{
    h_centered[n] = h[(n + N/2) % N];  // circular shift by N/2
}
*/
    auto h_centered = h; //preserve h
    std::ranges::rotate( h_centered, h_centered.begin() + N/2 );

// Now window h_centered instead of h
for (std::size_t n = 0; n < N; ++n)
{
    hamming[n] = 0.54 - 0.46 * std::cos(2 * PI * n / (N - 1));
    h_windowed[n] = hamming[n] * h_centered[n];
}

//------------------- Re-normalize after windowing -------------------
double dc_gain = 0.0;
for (std::size_t n = 0; n < N; ++n)
    dc_gain += h_windowed[n].real();   // this literally IS "what happens to a constant input"

for (std::size_t n = 0; n < N; ++n)
    h_windowed[n] /= dc_gain;          // undo the uniform shrinkage, keep the shape change

//--------------------------------------------------------------------

    std::cout << std::fixed << std::setprecision(6);

    std::println("Time domain sequence:");
    for (std::size_t n = 0; n < N; ++n)
    {
        std::println( "h[{}] = {} {} j{}", n , h[n].real(), h[n].imag() < 0 ? "-" : "+" , std::abs(h[n].imag()) );
    }

    ////////////////////////////////////////////////

{
	std::ofstream h_file("h.csv");
	std::println(h_file, "k,h_real,h_imag,h_centered_real,h_centered_imag,h_win_Real,h_win_Imag");

	for (std::size_t k = 0; k < h.size(); ++k)
	{
		//h_file << k << "," << h[k].real() << "," << h[k].imag() << "," << h_windowed[k].real() << "," << h_windowed[k].imag() << "\n";
		h_file << k << "," << h[k].real() << "," << h[k].imag() << "," << h_centered[k].real() << "," << h_centered[k].imag() << "," 
               << h_windowed[k].real() << "," << h_windowed[k].imag() << "\n";
	}

    ////////////////////////////////////////////////

	std::ofstream H_file("H.csv");

	std::println(H_file, "k,H_real,H_imag");

	for (std::size_t k = 0; k < H.size(); ++k)
	{
		H_file << k << "," << H[k].real() << "," << H[k].imag() << "\n";
	}

    ////////////////////////////////////////////////

    std::ofstream hw_file( "h_windowed.csv" );
    std::println(hw_file, "{},{},{},{}", "k", "h_real", "h_imag","hamming");
    for (std::size_t k = 0; k < h.size(); ++k)
    {
        hw_file << k << "," << h_windowed[k].real() << "," << h_windowed[k].imag() << "," << hamming[k] << "\n";
    }
}

    ////////////////////////////////////////////////

    std::system("python3 plot_idft.py&");

    return 0;
}
