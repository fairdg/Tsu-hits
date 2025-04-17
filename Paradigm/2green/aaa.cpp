#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <omp.h>
#include <chrono>
#include <algorithm>
#include <immintrin.h>

using namespace std;
using namespace chrono;

void sepiaImageSIMD(unsigned char* data, int width, int height, int channels) {
    if (data == nullptr) return;

    int rowSize = width * channels;

    __m256 factorR = _mm256_set1_ps(0.393f);
    __m256 factorG = _mm256_set1_ps(0.769f);
    __m256 factorB = _mm256_set1_ps(0.189f);

    __m256 factorR2 = _mm256_set1_ps(0.349f);
    __m256 factorG2 = _mm256_set1_ps(0.686f);
    __m256 factorB2 = _mm256_set1_ps(0.168f);

    __m256 factorR3 = _mm256_set1_ps(0.272f);
    __m256 factorG3 = _mm256_set1_ps(0.534f);
    __m256 factorB3 = _mm256_set1_ps(0.131f);

    __m256 scale = _mm256_set1_ps(0.8f);
    __m256 maxVal = _mm256_set1_ps(255.0f);

    for (int y = 0; y < height; y++) {
        unsigned char* row = data + y * rowSize;

        int x = 0;
        for (; x + 8 <= width; x += 8) {
            float r[8], g[8], b[8];

            for (int i = 0; i < 8; i++) {
                r[i] = row[(x + i) * channels + 0];
                g[i] = row[(x + i) * channels + 1];
                b[i] = row[(x + i) * channels + 2];
            }

            __m256 red = _mm256_loadu_ps(r);
            __m256 green = _mm256_loadu_ps(g);
            __m256 blue = _mm256_loadu_ps(b);

            __m256 resultR = _mm256_min_ps(maxVal, _mm256_mul_ps(
                _mm256_add_ps(
                    _mm256_add_ps(
                        _mm256_mul_ps(red, factorR),
                        _mm256_mul_ps(green, factorG)
                    ),
                    _mm256_mul_ps(blue, factorB)
                ), scale));

            __m256 resultG = _mm256_min_ps(maxVal, _mm256_mul_ps(
                _mm256_add_ps(
                    _mm256_add_ps(
                        _mm256_mul_ps(red, factorR2),
                        _mm256_mul_ps(green, factorG2)
                    ),
                    _mm256_mul_ps(blue, factorB2)
                ), scale));

            __m256 resultB = _mm256_min_ps(maxVal, _mm256_mul_ps(
                _mm256_add_ps(
                    _mm256_add_ps(
                        _mm256_mul_ps(red, factorR3),
                        _mm256_mul_ps(green, factorG3)
                    ),
                    _mm256_mul_ps(blue, factorB3)
                ), scale));

            _mm256_storeu_ps(r, resultR);
            _mm256_storeu_ps(g, resultG);
            _mm256_storeu_ps(b, resultB);

            for (int i = 0; i < 8; i++) {
                row[(x + i) * channels + 0] = static_cast<unsigned char>(r[i]);
                row[(x + i) * channels + 1] = static_cast<unsigned char>(g[i]);
                row[(x + i) * channels + 2] = static_cast<unsigned char>(b[i]);
            }
        }

        for (; x < width; x++) {
            unsigned char r = row[x * channels + 0];
            unsigned char g = row[x * channels + 1];
            unsigned char b = row[x * channels + 2];

            unsigned char newR = std::min(255, int(0.393f * r + 0.769f * g + 0.189f * b));
            unsigned char newG = std::min(255, int(0.349f * r + 0.686f * g + 0.168f * b));
            unsigned char newB = std::min(255, int(0.272f * r + 0.534f * g + 0.131f * b));

            row[x * channels + 0] = newR;
            row[x * channels + 1] = newG;
            row[x * channels + 2] = newB;
        }
    }
}

void solarizeImageSIMD(unsigned char* data, int width, int height, int channels) {
    if (data == nullptr || channels < 3) return;

    int pixelCount = width * height * channels;
    int simdSize = pixelCount - (pixelCount % 32);

    __m256i threshold = _mm256_set1_epi8(128);
    __m256i maxVal = _mm256_set1_epi8(255);

    for (int i = 0; i < simdSize; i += 32) {
        __m256i pixels = _mm256_loadu_si256((__m256i*)(data + i));

        __m256i alpha;
        if (channels == 4) {
            alpha = _mm256_and_si256(pixels, _mm256_set1_epi32(0xFF000000));
            pixels = _mm256_and_si256(pixels, _mm256_set1_epi32(0x00FFFFFF));
        }

        __m256i diff = _mm256_subs_epu8(pixels, threshold);
        __m256i mask = _mm256_cmpgt_epi8(diff, _mm256_setzero_si256());

        __m256i inverted = _mm256_sub_epi8(maxVal, pixels);
        __m256i result = _mm256_blendv_epi8(pixels, inverted, mask);

        if (channels == 4) {
            result = _mm256_or_si256(result, alpha);
        }

        _mm256_storeu_si256((__m256i*)(data + i), result);
    }

    for (int i = simdSize; i < pixelCount; i++) {
        if (channels == 4 && (i % 4) == 3) continue;

        uint8_t v = data[i];
        data[i] = (v > 128) ? (255 - v) : v;
    }
}

void posterizeImageSIMD(unsigned char* data, int width, int height, int channels) {
    if (data == nullptr || channels < 3) return;

    const int levels = 4;
    const int step = 256 / levels;
    const int half_step = step / 2;

    __m256i stepVec = _mm256_set1_epi8(step);
    __m256i halfStepVec = _mm256_set1_epi8(half_step);

    int pixelCount = width * height * channels;
    int simdSize = pixelCount - (pixelCount % 32);

    for (int i = 0; i < simdSize; i += 32) {
        __m256i pixels = _mm256_loadu_si256((__m256i*)(data + i));

        __m256i alpha;
        if (channels == 4) {
            alpha = _mm256_and_si256(pixels, _mm256_set1_epi32(0xFF000000));
            pixels = _mm256_and_si256(pixels, _mm256_set1_epi32(0x00FFFFFF));
        }

        __m256i lo = _mm256_unpacklo_epi8(pixels, _mm256_setzero_si256());
        __m256i hi = _mm256_unpackhi_epi8(pixels, _mm256_setzero_si256());

        __m256i loDiv = _mm256_srli_epi16(lo, 6);
        __m256i hiDiv = _mm256_srli_epi16(hi, 6);

        __m256i loMul = _mm256_slli_epi16(loDiv, 6);
        __m256i hiMul = _mm256_slli_epi16(hiDiv, 6);

        loMul = _mm256_add_epi16(loMul, _mm256_set1_epi16(half_step));
        hiMul = _mm256_add_epi16(hiMul, _mm256_set1_epi16(half_step));

        __m256i result = _mm256_packus_epi16(loMul, hiMul);

        if (channels == 4) {
            result = _mm256_or_si256(result, alpha);
        }

        _mm256_storeu_si256((__m256i*)(data + i), result);
    }

    for (int i = simdSize; i < pixelCount; i++) {
        if (channels == 4 && (i % 4) == 3) continue;

        int v = data[i];
        v = (v / step) * step + half_step;
        data[i] = static_cast<uint8_t>(v);
    }
}


void solarization(unsigned char* data, int width, int height, int channels, unsigned char threshold = 128) {
    #pragma omp parallel for
    for (int i = 0; i < width * height * channels; i++) {
        if (data[i] > threshold) {
            data[i] = 255 - data[i];
        }
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

        int new_r = static_cast<int>(0.393 * r + 0.769 * g + 0.189 * b);
        int new_g = static_cast<int>(0.349 * r + 0.686 * g + 0.168 * b);
        int new_b = static_cast<int>(0.272 * r + 0.534 * g + 0.131 * b);

        if (new_r > 255) new_r = 255;
        if (new_g > 255) new_g = 255;
        if (new_b > 255) new_b = 255;

        data[i]     = new_r;
        data[i + 1] = new_g;
        data[i + 2] = new_b;
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
// 300x300 400x400 500x500 600x600 950x950 2400x2400

    auto start = high_resolution_clock::now();
    //sepia(data, width, height, channels);
    //solarization(data, width, height, channels);
    //posterization(data, width, height, channels);

    //posterizeImageSIMD(data, width, height, channels);
    //solarizeImageSIMD(data, width, height, channels);
    sepiaImageSIMD(data, width, height, channels);
    auto end = high_resolution_clock::now();
    
    duration<double> duration = end - start;
    cout << "time: " << duration.count() << " s\n";
    
    if (!stbi_write_png("output.png", width, height, channels, data, width * channels)) {
        cout << "error saving" << endl;
        stbi_image_free(data);
        return 1;
    }

    stbi_image_free(data);
    //system("start output.png");
    return 0;
}
