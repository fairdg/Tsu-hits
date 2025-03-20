#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <omp.h>  
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

void solarization(unsigned char* data, int width, int height, int channels, unsigned char threshold = 128) {
    #pragma omp parallel for
    for (int i = 0; i < width * height * channels; i++) {
        data[i] = (data[i] > threshold) ? (255 - data[i]) : data[i];
    }
}

void posterization(unsigned char* data, int width, int height, int channels) {
    const int step = 256 / 4;
    #pragma omp parallel for
    for (int i = 0; i < width * height * channels; i++) {
        data[i] = (data[i] / step) * step + step / 2;
    }
}

void sepia(unsigned char* data, int width, int height, int channels) {
    #pragma omp parallel for
    for (int i = 0; i < width * height * channels; i += channels) {
        int r = data[i], g = data[i + 1], b = data[i + 2];

        data[i]     = min(255, static_cast<int>(0.393 * r + 0.769 * g + 0.189 * b));
        data[i + 1] = min(255, static_cast<int>(0.349 * r + 0.686 * g + 0.168 * b));
        data[i + 2] = min(255, static_cast<int>(0.272 * r + 0.534 * g + 0.131 * b));
    }
}

int main() {
    setlocale(0, "RUS");

    int width, height, channels;
    unsigned char* data = stbi_load("2400x2400.png", &width, &height, &channels, 0);
    if (!data) {
        cout << "error loading" << endl;
        return 1;
    }
    //cout << "Threads: " << omp_get_max_threads() << endl;
    auto start = high_resolution_clock::now();
    //sepia(data, width, height, channels);
    solarization(data, width, height, channels);
    //posterization(data, width, height, channels);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    cout << "time: " << duration.count() << " s\n";
    
    if (!stbi_write_png("output.png", width, height, channels, data, width * channels)) {
        cout << "error saving" << endl;
        stbi_image_free(data);
        return 1;
    }

    //system("start output.png");
    stbi_image_free(data);
    return 0;
}
