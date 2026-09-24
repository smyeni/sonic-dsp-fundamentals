// idft_fir.cpp  –  Frequency-sampling FIR design (modern C++23)
#include <algorithm>
#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <numbers>
#include <print>
#include <ranges>
#include <vector>
#include <cstdio>
#include <string>

using Complex = std::complex<double>;
using CVec    = std::vector<Complex>;

// ------------------------------------------------------------------
// Inverse DFT  (O(N²) – perfectly fine for educational N ≤ 512)
// ------------------------------------------------------------------
[[nodiscard]]
CVec idft(const CVec& H)
{
    const std::size_t N = H.size();
    CVec h(N);

    for (std::size_t n = 0; n < N; ++n) {
        Complex sum{};
        for (std::size_t k = 0; k < N; ++k) {
            const double θ = 2.0 * std::numbers::pi * static_cast<double>(k * n) / N;
            sum += H[k] * std::exp(Complex{0.0, θ});
        }
        h[n] = sum / static_cast<double>(N);
    }
    return h;
}

// ------------------------------------------------------------------
// Hamming window  (symmetric, length N)
// ------------------------------------------------------------------
[[nodiscard]]
std::vector<double> hamming(std::size_t N)
{
    std::vector<double> w(N);
    for (std::size_t n = 0; n < N; ++n)
        w[n] = 0.54 - 0.46 * std::cos(2.0 * std::numbers::pi * n / (N - 1));
    return w;
}

// ------------------------------------------------------------------
int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::println(stderr, "Usage: {} <N> <cutoff_Hz>", argv[0]);
        return 1;
    }

    const std::size_t N = std::stoul(argv[1]);
    if (N < 8 || (N & 1)) {                     // keep even for clean centre
        std::println(stderr, "N should be even and ≥ 8");
        return 1;
    }

    const double Fs = 8000; //Sampling freq

    // --------------------------------------------------------------
    // 1. Ideal frequency response  (high-pass style, same as yours)
    // --------------------------------------------------------------
    CVec H(N, 0.0);
    double cutoff = std::stoul( argv[2] );
    double bin_sep = Fs/N;
    std::size_t n_cutoff = static_cast<std::size_t>(cutoff/bin_sep);
    std::println("Cutof: {}", cutoff);
    std::println("Num_cutoff_intervals: {}", n_cutoff);

    //for (std::size_t k = 0; k < 7; ++k) {
    for (std::size_t k = 0; k < n_cutoff; ++k) 
    {
        H[k] = 1.0;   // low-side pass
    }

    //for (std::size_t k = 58; k < N;      ++k) {
    //for (std::size_ k = 116; k < N;      ++k) {
    for (std::size_t k = N-n_cutoff; k < N; ++k) 
    {
        H[k] = 1.0;   // high-side pass
    }
    std::println("BW bounds: {} and {}",n_cutoff, N-n_cutoff);

    // bins 7 … 57 remain 0 → stop-band
    // bins 14 … 116 remain 0 → stop-band

    // --------------------------------------------------------------
    // 2. IDFT → impulse response
    // --------------------------------------------------------------
    CVec h = idft(H);

    // --------------------------------------------------------------
    // 3. Circular shift so that the peak sits at the centre
    // --------------------------------------------------------------
    CVec h_centered = h;
    std::ranges::rotate(h_centered, h_centered.begin() + N/2);

    // --------------------------------------------------------------
    // 4. Apply Hamming window
    // --------------------------------------------------------------
    const auto w = hamming(N);
    CVec h_win(N);
    for (std::size_t n = 0; n < N; ++n)
        h_win[n] = h_centered[n] * w[n];

    // --------------------------------------------------------------
    // 5. Renormalise so that sum(h_win) == 1  (unity DC / pass-band gain)
    // --------------------------------------------------------------
    const double dc = std::ranges::fold_left(h_win, 0.0,
                        [](double acc, Complex z){ return acc + z.real(); });
    for (auto& z : h_win) z /= dc;

    // --------------------------------------------------------------
    // 6. Write CSV files (same layout you already use)
    // --------------------------------------------------------------
    {
        std::ofstream f("h.csv");
        std::println(f, "k,h_real,h_imag,h_centered_real,h_centered_imag,h_win_real,h_win_imag");
        for (std::size_t k = 0; k < N; ++k)
            std::println(f, "{},{},{},{},{},{},{}",
                         k,
                         h[k].real(),          h[k].imag(),
                         h_centered[k].real(), h_centered[k].imag(),
                         h_win[k].real(),      h_win[k].imag());
    }
    {
        std::ofstream f("H.csv");
        std::println(f, "k,H_real,H_imag");
        for (std::size_t k = 0; k < N; ++k)
            std::println(f, "{},{},{}", k, H[k].real(), H[k].imag());
    }
    {
        std::ofstream f("h_windowed.csv");
        std::println(f, "k,h_real,h_imag,hamming");
        for (std::size_t k = 0; k < N; ++k)
            std::println(f, "{},{},{},{}",
                         k, h_win[k].real(), h_win[k].imag(), w[k]);
    }

    // --------------------------------------------------------------
    // 7. Launch the plotter (same as before)
    // --------------------------------------------------------------
    FILE* pipe = popen("find $HOME -name 'plot_FIR_filt.py' -print", "r");

    if (!pipe)
        return 1;

    char buffer[256];
    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe))
        result += buffer;

    pclose(pipe);

    //std::system("python3 plot_FIR.py &");
	result.erase(result.find_last_not_of("\n\r") + 1);
	std::string command = "python3 \"" + result + "\"";
	std::system(command.c_str());

    std::println("Done.  Wrote h.csv, H.csv, h_windowed.csv");
    return 0;
}
