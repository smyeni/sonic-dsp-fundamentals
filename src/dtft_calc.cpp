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
    std::vector<double> x_windowed;
    double sample = 0;
    unsigned long sample_count = 0;
    const double F_s = 100000; //100kHz sampling rate

    while (std::getline(samples_file, line))
    {
        for (auto [index, token] : std::views::split(line, ',') | std::views::enumerate)
        {
            std::string_view value{token.begin(), token.end()};
            auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), sample);

            if (ec != std::errc{}) {
                std::println("parse error on line {}: '{}'", sample_count, value);
                continue;
            }

            if (index == 2) { // x[n] column
                std::println("x[{}] = {}", sample_count, sample);
                x.push_back(sample);
            }
			else if (index == 3) { // hann column
                std::println("hann[{}] = {}", sample_count, sample);
                hann.push_back(sample);
            }
        }
        sample_count++;
    }

    //Window the damn signal before spectrum analysis
    for (auto [n,sample] : std::views::enumerate(x))
    {
        x_windowed.push_back(hann[n] * x[n]);
    }

    std::vector<double> omegas;
    for (int k=0; k<360; k++)
    {
        omegas.push_back(k*0.0174532920); //deg incr
    }

    double omega = std::numbers::pi * 0.2; //Careful!!!

    std::ofstream outfile{"spectrum.csv"};
    outfile.imbue(std::locale::classic());

    println(outfile, "{},{},{},{}", "omega", "freq", "magnitude", "phase");
    double magnitude;
    double phase;

	//Hold tight, voodoo happening here
    for (auto omega : omegas)
    {
        std::complex<double> X_omega(0.0, 0.0);

        //Interrogate the N samples for similarity with current/each omega
        for (auto [n, sample_val] : std::views::enumerate(x_windowed)) {
            std::complex<double> exp_term(std::cos(omega * n), -std::sin(omega * n));
            X_omega += sample_val * exp_term;
        }

        magnitude = std::abs(X_omega);
        phase = std::arg(X_omega);
        println(outfile, "{:.6e},{},{:.6e},{:.6e}", omega, F_s*omega/(2*std::numbers::pi), magnitude, phase);
    }
}
