#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <complex>
#include <ranges>
#include <numbers>
#include <chrono>

int main() {
    /*
    const double f_signal = 10000.0;     // 10 kHz
    const double f_sample = 100000.0;    // 100 kHz
    const double pi = std::numbers::pi;

    // 1. Generate a larger sample buffer so the CPU actually has to sweat
    constexpr size_t N = 1000;
    std::vector<double> x(N);
    for (size_t n = 0; n < N; ++n) {
        x[n] = std::sin(2.0 * pi * f_signal * n / f_sample);
    }
    */

    const double f_sigA = 10000.0; // 10 kHz
    const double f_sigB = 24000.0; // 24 kHz (Our new dance partner)
    const double f_sample = 100000.0; // 100 kHz

    constexpr size_t N = 1000;
    std::vector<double> x(N);

    for (size_t n = 0; n < N; ++n) {
        // We mix (add) the two waves together in the exact same time slot
        double tone_A = 1.0 * std::sin(2.0 * std::numbers::pi * f_sigA * n / f_sample);
        double tone_B = 0.5 * std::sin(2.0 * std::numbers::pi * f_sigB * n / f_sample); // Half the amplitude!
        
        x[n] = tone_A + tone_B;
    }


    // 2. Set up our frequency sweep (500 interrogation frequencies)
    std::vector<double> omegas;
    for (double f = 1000.0; f <= 50000.0; f += 100.0) {
        omegas.push_back(2.0 * std::numbers::pi * f / f_sample);
    }

    std::cout << "--- SONIC DSP BENCHMARK HARNESS ---" << "\n";
    std::cout << "Analyzing " << N << " samples across " << omegas.size() << " frequencies.\n\n";

    // =================================================================
    // ENGINE 1: THE BRUTE FORCE METHOD (Trig Evals Inside Inner Loop)
    // =================================================================
    auto start_time_1 = std::chrono::high_resolution_clock::now();
    
    // We create a dummy variable to force the compiler to actually do the math 
    // and prevent it from optimizing the unused loop away entirely.
    double dummy_checksum_1 = 0.0; 

    for (auto omega : omegas) {
        std::complex<double> X_omega(0.0, 0.0);
        for (auto [n, sample_val] : std::views::enumerate(x)) {
            // Overkill: 2 heavy trig evaluations on EVERY iteration
            std::complex<double> exp_term(std::cos(omega * n), -std::sin(omega * n));
            X_omega += sample_val * exp_term;
        }
        dummy_checksum_1 += std::abs(X_omega);
    }

    auto end_time_1 = std::chrono::high_resolution_clock::now();
    auto duration_1 = std::chrono::duration_cast<std::chrono::microseconds>(end_time_1 - start_time_1).count();

    // =================================================================
    // ENGINE 2: THE PHASOR ROTATION METHOD (Complex Multiplications)
    // =================================================================
    auto start_time_2 = std::chrono::high_resolution_clock::now();
    double dummy_checksum_2 = 0.0;

    for (auto omega : omegas) {
        std::complex<double> X_omega(0.0, 0.0);
        
        // Trigonometry tax paid exactly TWICE per frequency here:
        const std::complex<double> rotation_step(std::cos(omega), -std::sin(omega));
        std::complex<double> current_phasor(1.0, 0.0);

        for (double sample_val : x) {
            // Pure, lightning-fast complex arithmetic inside the loop
            X_omega += sample_val * current_phasor;
            
            // Advancing the damn phase via vector multiplication!
            current_phasor *= rotation_step; 
        }
        dummy_checksum_2 += std::abs(X_omega);
    }

    auto end_time_2 = std::chrono::high_resolution_clock::now();
    auto duration_2 = std::chrono::duration_cast<std::chrono::microseconds>(end_time_2 - start_time_2).count();

    // =================================================================
    // THE RECEIPT SPREADSHEET
    // =================================================================
    std::cout << "Engine 1 (Brute Force): " << duration_1 << " microseconds.\n";
    std::cout << "Engine 2 (Phasor Multiplier): " << duration_2 << " microseconds.\n";
    
    double speedup = static_cast<double>(duration_1) / duration_2;
    std::cout << "Performance Increase: " << speedup << "x faster!\n";

     // Add this to the very bottom to prevent Dead Code Elimination!
    std::cout << "\n[Harness Checksums - Do Not Delete]\n";
    std::cout << "Engine 1 Checksum: " << dummy_checksum_1 << "\n";
    std::cout << "Engine 2 Checksum: " << dummy_checksum_2 << "\n";
}
