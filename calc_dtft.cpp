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
    double sample = 0;
    unsigned long sample_count = 0;

    while (std::getline(samples_file, line))
    {
        for (auto [index, token] : std::views::split(line, ',') | std::views::enumerate)
        {
            if (index == 2) { // x[n] column
                std::string_view value{token.begin(), token.end()};

                auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), sample);
                if (ec != std::errc{}) {
                    std::println("parse error on line {}: '{}'", sample_count, value);
                    continue;
                }

                std::println("x[{}] = {}", sample_count, sample);
                x.push_back(sample);
            }
        }
        sample_count++;
    }

    double omega = std::numbers::pi / 2.0;
    std::complex<double> X_omega(0.0, 0.0);

    for (auto [n, sample_val] : std::views::enumerate(x)) {
        std::complex<double> exp_term(std::cos(omega * n), -std::sin(omega * n));
        X_omega += sample_val * exp_term;
    }

    double magnitude = std::abs(X_omega);
    double phase = std::arg(X_omega);
    std::println("At omega = {} rad/sample:", omega);
    std::println("Magnitude: {}", magnitude);
    std::println("Phase (rad): {}", phase);
}