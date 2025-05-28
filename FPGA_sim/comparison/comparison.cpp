#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <chrono>

// Image dimensions
const int WIDTH = 640;
const int HEIGHT = 480;

// Function parameters: poles and zeroes
std::vector<std::complex<double>> poles = { {0.5, 0.9} };
std::vector<std::complex<double>> zeroes = { {-0.5, 0} };

// Evaluate complex function H(z) = product(z - zeroes) / product(z - poles)
std::complex<double> evaluate_function(const std::complex<double>& z) {
    std::complex<double> numerator(1.0, 0.0);
    std::complex<double> denominator(1.0, 0.0);
    for (const auto& zero : zeroes) numerator *= (z - zero);
    for (const auto& pole : poles) denominator *= (z - pole);
    return numerator / denominator;
}

// Map phase [-pi, pi] to RGB (simple HSV to RGB mapping)
void phase_to_rgb(double phase, int& r, int& g, int& b) {
    double hue = (phase + M_PI) / (2 * M_PI); // [0,1]
    double s = 1.0, v = 1.0;
    double c = v * s;
    double x = c * (1 - fabs(fmod(hue * 6, 2) - 1));
    double m = v - c;
    double r1, g1, b1;
    if (hue < 1.0/6)      { r1 = c; g1 = x; b1 = 0; }
    else if (hue < 2.0/6) { r1 = x; g1 = c; b1 = 0; }
    else if (hue < 3.0/6) { r1 = 0; g1 = c; b1 = x; }
    else if (hue < 4.0/6) { r1 = 0; g1 = x; b1 = c; }
    else if (hue < 5.0/6) { r1 = x; g1 = 0; b1 = c; }
    else                  { r1 = c; g1 = 0; b1 = x; }
    r = static_cast<int>((r1 + m) * 255);
    g = static_cast<int>((g1 + m) * 255);
    b = static_cast<int>((b1 + m) * 255);
}

void mag_to_rgb(double mag, int& r, int& g, int& b) {
    // Simple mapping: low magnitude to blue, high magnitude to red
    double norm_mag = mag ;
    r = static_cast<int>(norm_mag * 255);
    g = 0;
    b = static_cast<int>((1.0 - norm_mag) * 255);
}

int main() {
    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Write to a temporary PPM file
    const char* ppm_filename = "comparison_temp.ppm";
    std::ofstream ofs(ppm_filename);
    ofs << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // Normalise axis to [-1, 1]
            double re = 2.0 * x / (WIDTH - 1) - 1.0;
            double im = 2.0 * y / (HEIGHT - 1) - 1.0;
            std::complex<double> z(re, im);

            // Evaluate function
            std::complex<double> val = evaluate_function(z);

            // Calculate phase and magnitude
            double phase = std::arg(val);
            double mag = std::abs(val);

            // Map phase to hue-based RGB first
            int r, g, b;
            phase_to_rgb(phase, r, g, b);

            // Brightness factor: darker as mag → 1
            double brightness = std::max(0.0, 1.0 - mag);  // or tweak this scaling
            r = static_cast<int>(r * brightness);
            g = static_cast<int>(g * brightness);
            b = static_cast<int>(b * brightness);

            // Write RGB to file
            ofs << r << " " << g << " " << b << " ";
        }
        ofs << "\n";
    }

    ofs.close();

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Frame generation time: " << elapsed.count() << " seconds\n";

    // Convert PPM to PNG using ImageMagick's convert command
    int ret = std::system("convert comparison_temp.ppm comparison.png");
    if (ret != 0) {
        std::cerr << "Error: Could not convert PPM to PNG. Make sure ImageMagick is installed.\n";
        return 1;
    }

    std::cout << "Image saved to comparison.png\n";
    return 0;
}