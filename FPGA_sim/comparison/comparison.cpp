#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <complex>

const int WIDTH = 2048;
const int HEIGHT = 2048;
constexpr double pi = 3.14159265358979323846;

std::vector<std::complex<double>> poles_zeros = {{500, 500}, 
                                                {-200, -100}, 
                                                {400, 300},
                                                {-900, 500},
                                                {20, 150}, 
                                                {-50, -300},
                                                {400, -200},
                                                {-50, 10}};

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

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    const char* ppm_filename = "comparison_temp.ppm";
    std::ofstream ofs(ppm_filename);
    ofs << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    for (int y = 0; y < HEIGHT; ++y){
        for (int x = 0; x < WIDTH; ++x){
            std::complex<double> z(x - WIDTH / 2.0, (HEIGHT - 1 - y) - HEIGHT / 2.0);
            std::vector<std::complex<double>> w(poles_zeros.size());
            std::vector<double> theta(poles_zeros.size());
            std::vector<double> r(poles_zeros.size());
            double phase = 0;
            double log_mag = 0;
            
            double sum_zeros_phase = 0;
            double sum_poles_phase = 0;
            double sum_zeros_mag = 0;
            double sum_poles_mag = 0;

            for (size_t i = 0; i < poles_zeros.size(); ++i) {
                w[i] = z - poles_zeros[i];

                theta[i] = std::arg(w[i]);
                r[i] = (std::log2(std::abs(w[i])) - std::floor(std::log2(std::abs(w[i]))));

                size_t no_zeros = 4;
            
                if (i < no_zeros){
                    sum_zeros_phase += theta[i];
                    sum_zeros_mag += r[i];
                } else {
                    sum_poles_phase += theta[i];
                    sum_poles_mag += r[i];
                }
            }

            phase = std::remainder(sum_zeros_phase - sum_poles_phase, 2 * pi);
            double raw_mag = sum_zeros_mag - sum_poles_mag;
            log_mag = raw_mag - std::floor(raw_mag);

            double brightness = log_mag;

            int red, green, blue;

            phase_to_rgb(phase, red, green, blue);

            red = static_cast<int>(red * brightness);
            green = static_cast<int>(green * brightness);
            blue = static_cast<int>(blue * brightness);

            ofs << red << " " << green << " " << blue << " ";
        }
        ofs << "\n";
    }

    ofs.close();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Frame generation time: " << elapsed.count() << " seconds\n";

        int ret = std::system("convert comparison_temp.ppm comparison.png");
    if (ret != 0) {
        std::cerr << "Error: Could not convert PPM to PNG. Make sure ImageMagick is installed.\n";
        return 1;
    }

    std::cout << "Image saved to comparison.png\n";
    return 0;
}