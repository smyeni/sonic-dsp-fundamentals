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
    const double f_signal = 10000;    //10kHz
    const double f_sampling = 100000; //100kHz sampling rate

    while ( std::getline(samples_file, line) )
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
    std::println("Number of samples: {}:{}\n", x.size(), hann.size());

    //-------------------------------------------------
    //                 The DFT dance
    //-------------------------------------------------

    const std::size_t N_samples = x.size();

    const double bin_hz = f_sampling/N_samples;
    const double bin_delta_omega = 2 * std::numbers::pi / N_samples;
    std::vector<double> bin_omegas(N_samples);

    //phase chasers
    for (int k=0; k < N_samples; k++)
    {
        bin_omegas[k] = k*bin_delta_omega;
    }
    std::println("Number of bin freqs: {}", bin_omegas.size());

    std::ofstream outfile{"spectrum.csv"};
    outfile.imbue(std::locale::classic());

    std::println(outfile, "{},{},{},{},{},{},{}","n", "omega", "freq", "magn_hann", "phase_raw", "phase_hann","magn_raw");
    double magn_raw;
    double magn_hann;
    double phase_raw;
    double phase_hann;

	//Hold tight, voodoo happening here
    for (auto [k,bin_omega] : std::views::enumerate(bin_omegas))
    {
        const std::complex<double> per_sample_phase_advance( std::cos(bin_omega), std::sin(bin_omega) );
        std::complex<double> phase_reverser(1, 0);
        std::complex<double> X_omega_hann(0.0, 0.0);
        std::complex<double> X_omega_raw(0.0, 0.0);

        //Interrogate the N samples for similarity with current/each omega:
        //  Our frequency hunter throws successive phase-advanced samples back to DC by applying the 
        //  negative of their accumulated phase advance. If they all land aligned, 
        //  the hunter has found the signal's frequency.

        for (auto [n, sample] : std::views::enumerate(x_windowed)) 
        {
            X_omega_raw  += x[n] * phase_reverser;          //phase throw-back (rect window)
            X_omega_hann += x_windowed[n] * phase_reverser; //phase throw-back (Hann window)

            //Prep an even more aggressive cumulative phase throw back for next sample
            phase_reverser *= std::conj( per_sample_phase_advance );
        }

        magn_hann = std::abs(X_omega_hann);
        magn_raw = std::abs(X_omega_raw);
        phase_hann = std::arg(X_omega_hann);
        phase_raw = std::arg(X_omega_raw);
        std::println(outfile, "{},{:.6e},{},{:.6e},{:.6e},{:.6e},{:.6e}",k, bin_omega, k*bin_hz, magn_hann, phase_raw, phase_hann, magn_raw);
    }
}
