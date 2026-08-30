#include <cmath>
#include <print>
#include <array>
#include <ranges>
#include <numbers>
#include <fstream>
#include <algorithm>

int main()
{
	constexpr size_t N = 128; //64 = 2^6
	std::array<float,N> x{0};
	std::array<float,N> x_win{0};
	std::array<float,N> phase{0};
	std::array<float,N> hann{0};

	const float f_signal = 10000; //10kHz
	const float F_s = 10*f_signal;

	//float start = 0.0f;
	//float Ts = 1.0f/F_s;
	const double pi = std::numbers::pi;
	const float f_digital = 2*pi*f_signal/F_s;

	std::println("{:>3},{:>12},{:>12}", "index", "phase", "x[n]");

	for (std::size_t n = 0; n < phase.size(); ++n) 
	{
		phase[n] = n * f_digital;
		x[n] = std::sin(phase[n]);
		std::println( "{:>3},{:>14.6e},{:>14.6e}", n, phase[n], x[n] );
	}

	std::ofstream file{"signal.csv"};
	file.imbue(std::locale::classic());

	std::println(file, "{},{},{},{},{}", "index", "phase", "x[n]", "hann","x_win");

	//Hanning window length depends on file length
	for (size_t n=0; n < x.size(); n++) {
		hann[n] = 0.5 * (1.0 - std::cos(2.0 * std::numbers::pi * n / (N - 1)));
	    x_win[n] = hann[n] * x[n];
	}

	for (auto [index, phase_val] : std::views::enumerate(phase)) {
	    std::println(file, "{},{:.6e},{:.6e},{:.6e},{:.6e}", index, phase_val, x[index], hann[index], x_win[index]);
	}

	std::println("The end {}", phase.size());
}
