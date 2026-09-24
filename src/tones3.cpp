#include <cmath>
#include <print>
#include <vector>
#include <ranges>
#include <cstdio>
#include <numbers>
#include <fstream>
#include <algorithm>

int main( int argc, char* args[] )
{
    if (argc < 3)
    {
        std::println( "Usage: {} <N> <outfile>", args[0] );
        return -1;
    }

	const size_t N = std::stoul( args[1] );

	std::vector<double> x1(N,0);
	std::vector<double> x2(N,0);
	std::vector<double> x3(N,0);
	std::vector<double> sig_raw(N,0);
	std::vector<double> hamming(N,0);
	std::vector<double> sig_windowed(N,0);

	std::vector<double> phase1(N,0);
	std::vector<double> phase2(N,0);
	std::vector<double> phase3(N,0);

	const double f1_signal = 500;   //500Hz
	const double f2_signal = 1500;  //1.5kHz
	const double f3_signal = 3000;  //3kHz
	const double F_s = 8000;        //8kHz

	const double pi = std::numbers::pi;

    //Digital freqs (per sample phase stride)
	const double f1_digital = 2*pi*f1_signal/F_s;
	const double f2_digital = 2*pi*f2_signal/F_s;
	const double f3_digital = 2*pi*f3_signal/F_s;

	//std::println( "{:>3},{:>12},{:>12}", "index", "x[n], "phase" );

	for (std::size_t n = 0; n < x1.size(); ++n) 
	{
		phase1[n] = n * f1_digital;
		x1[n] = std::sin(phase1[n]);

		phase2[n] = n * f2_digital;
		x2[n] = std::sin(phase2[n]);

		phase3[n] = n * f3_digital;
		x3[n] = std::sin(phase3[n]);

        sig_raw[n] = x1[n] + x2[n] + x3[n];
        hamming[n] = 0.54 - 0.46 * std::cos( (2*std::numbers::pi*n) / (N-1) );
	}

    //It's windowing time
    for ( auto [n, win] : std::views::enumerate(hamming) )
    {
       sig_windowed[n] = win * sig_raw[n]; 
    }


	const std::string filename( args[2]);

	{
		std::ofstream file{ filename };
		file.imbue(std::locale::classic());

		std::println( file, "{},{},{},{},{},{},{},{},{}", "index", "x[n]", "sig_win",  "phase1", "phase2", "phase3", "x1", "x2", "x3" );

		for ( std::size_t n = 0; n < sig_windowed.size(); n++ ) 
		{
			std::println( file, "{},{:.6e},{:.6e},{:.6e},{:.6e},{:.6e},{:.6e},{:.6e},{:.6e}", 
				 n, 
				 sig_raw[n], 
				 sig_windowed[n], 
				 std::fmod(phase1[n],2*std::numbers::pi), 
				 std::fmod(phase2[n], 2*std::numbers::pi), 
				 std::fmod(phase3[n],2*std::numbers::pi),
				 x1[n], x2[n], x3[n] );
		}
	}

    std::system( std::format( "python plot_tones.py {} &", filename ).c_str() );

	std::println("The end {}", x1.size());
}
