#include <print>
#include <complex>
#include "convolution.h"
#include "circular_convolution.h"

int main( int argc, char *args[] ) 
{
    if (argc < 4) {
       std::println("Usage: {} <signal_file> <h_file> <out_file>\n", args[0]);
       return -1;
    }

    std::string signal_file(args[1]);
    std::string h_file(args[2]);
    std::string out_file(args[3]);

	// signal.csv: index,phase,x[n],hann,x_win
	// 0 1 2 3 4
	auto s = sonic::load_column( signal_file, 2); // x[n]
	auto s_win = sonic::load_column( signal_file, 4); // x_win if you want windowed

	// h.csv: n,h or just h
	// if n,h -> col 1 is h
	auto h = sonic::load_column( h_file, 1 );
	if(h.empty()) 
        h = sonic::load_column( h_file, 0 ); // fallback single col

	// tones3.csv: n,x,x_windowed,phase1...
	// 0 1 2...
	//auto tones = sonic::load_column("tones3.csv", 1);

    //-----------------------------------

    auto y_lin = sonic::convolve(s,h);

{
    std::ofstream y_file( out_file );
    std::println(y_file, "{},{}", "n", "y_Real");
    for (std::size_t k = 0; k < y_lin.size(); ++k)
    {
        //y_file << k << "," << y_lin[k].real() << "," << y_lin[k].imag() << "\n";
        println(y_file, "{},{}", k, y_lin[k]);
    }
}

/*
    std::println("Linear {}:", y_lin.size());
    for(size_t i=0;i<y_lin.size();++i) std::println(" y[{}]={}", i, y_lin[i]);

    for (size_t N : {7,5,4,3}) {
        auto y_c = sonic::circular_convolve(s,h,N);
        std::println("\nCircular N={}:", N);
        for(size_t i=0;i<y_c.size();++i) std::println(" yc[{}]={}", i, y_c[i]);
    }
*/
}
