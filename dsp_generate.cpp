#include <cmath>
#include <print>
#include <array>
#include <ranges>
#include <numbers>
#include <fstream>
#include <algorithm>

int main()
{
	std::array<float,21> x{0};
	std::array<float,21> phase{0};

	const float freq = 10000; //10kHz
	const float F_s = 10*freq;

	float start = 0.0f;
	float Ts = 1.0f/F_s;
	const double pi = std::numbers::pi;
	const float f_digital = 2*pi*freq/F_s;

	std::println("{:>3},{:>12},{:>12}", "index", "phase", "x[n]");
	for (std::size_t n = 0; n < phase.size(); ++n) 
	{
		phase[n] = n * f_digital;
		x[n] = std::sin(phase[n]);
		std::println( "{:>3}, {:>14.6e}, {:>14.6e}", n, phase[n], x[n] );
	}

	std::ofstream file{"signal.csv"};
	file.imbue(std::locale::classic());

	std::println(file, "{},{},{}", "index", "phase", "x[n]");

	for (auto [index, phase_val] : std::views::enumerate(phase)) {
		std::println(file, "{}, {:.6e}, {:.6e}", index, phase_val, x[index]);
	}
    
	std::println("The end {}", phase.size());
}
