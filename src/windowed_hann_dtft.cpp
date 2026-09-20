#include <print>
#include <array>
#include <cmath>
#include <ranges>
#include <complex>
#include <charconv>
#include <numbers>
#include <fstream>
#include <string>
#include <vector>

int main()
{
    std::ifstream samples_file{"signal.csv"};
    std::string line;
    std::getline(samples_file, line); // discard header

    std::vector<double> x;
    std::vector<double> hann; 

    double sample = 0;
    unsigned long sample_id = 0;

    while (std::getline(samples_file, line))
    {
        for (auto [token_count, token] : std::views::split(line, ',') | std::views::enumerate)
        {
            if (token_count == 2) { // x[n] column
                std::string_view value{token.begin(), token.end()};

                auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), sample);
                if (ec != std::errc{}) {
                    std::println("parse error on line {}: '{}'", sample_id, value);
                    continue;
                }

                std::println("x[{}] = {}", sample_id, sample);
                x.push_back(sample);
            }
        }
        sample_id++;
    }

    //Hanning window length depends on file length
    size_t N = x.size();
    for (size_t n=0; n < x.size(); n++) {
        hann.push_back( 0.5 * (1.0 - std::cos(2.0 * std::numbers::pi * sample_id / (N - 1))) );
    }
    std::println("Hann window length: {}", N);

    std::vector<double> omegas;
    for (int k=0; k<360; k++)
    {
        omegas.push_back(k*0.0174532920); //deg incr
    }

    double omega = std::numbers::pi * 0.2; //Careful!!!

	std::ofstream outfile{"spectrum.csv"};
	outfile.imbue(std::locale::classic());

    println(outfile, "{},{},{},{},{}", "omega", "freq", "magnitude", "phase","hann");
    double magnitude;
    double phase;

	//Hold tight, voodoo happening here
    for ( auto [idx,omega] : std::views::enumerate(omegas) )
    {
        std::complex<double> X_omega(0.0, 0.0);

        //Interrogate the N samples for similarity with current/each omega
        for (auto [n, sample_val] : std::views::enumerate(x)) {
            std::complex<double> exp_term(std::cos(omega * n), -std::sin(omega * n));
            X_omega += sample_val * exp_term;
        }

        magnitude = std::abs(X_omega);
        phase = std::arg(X_omega);
        println(outfile, "{:.6e},{},{:.6e},{:.6e},{:.6e}", 
            omega, 10000*omega/2/std::numbers::pi, magnitude, phase, hann[idx]);
    }
}