#include <fstream>
#include <iostream>
#include <vector>
#include "../include/canvas.h"
#include "../include/lodepng.h"

using namespace life;

/// Saves an image as a **ascii** PPM file.
bool save_ppm3(const unsigned char* data, size_t w, size_t h, size_t d,
               const std::string& file_name_) {
        std::ofstream ofs_file(file_name_, std::ios::out);
        if (not ofs_file.is_open()) return false;

        ofs_file << "P3\n"
                 << w << " " << h << "\n"
                 << "255\n";

        size_t i{0};
        while (i < (w * h * d)) {
                // depth traversal, usually 3.
                for (auto id = 0; id < 3; id++) ofs_file << (int)*(data + i++) << " ";
                ofs_file << std::endl;
                i++;  // to skip alpha channel.
        }

        // Did it not fail?
        auto result = not ofs_file.fail();

        ofs_file.close();

        return result;
}

// Example 1
// Encode from raw pixels to disk with a single function call
// The image argument has width * height RGBA pixels or width * height * 4 bytes
void encode_png(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
        // Encode the image
        unsigned error = lodepng::encode(filename, image, width, height);

        // if there's an error, display it
        if (error)
                std::cout << "encoder error " << error << ": " << lodepng_error_text(error)
                          << std::endl;
}

// Saves image to filename given as argument. Warning, this overwrites the file without warning!
int main(int argc, char* argv[]) {
        // NOTE: this sample will overwrite the file or test.png without warning!
        const char* filename = argc > 1 ? argv[1] : "test.png";

        // generate some image
        // Width: 48 * 40 = 1920
        // Height: 27 * 40 = 1080
        unsigned width = 48, height = 27;
        short block_size = 40;

        Canvas image(width, height, block_size);

        for (auto x = 0u; x < width; x++) {
                for (auto y = 0u; y < height; y++) {
                        if (y % 2) {
                                if (!(x % 2)) {
                                        image.pixel(Point2(x, y), DEEP_SKY_BLUE);
                                } else {
                                        image.pixel(Point2(x, y), RED);
                                }
                        } else {
                                if (!(x % 2)) {
                                        image.pixel(Point2(x, y), RED);
                                } else {
                                        image.pixel(Point2(x, y), DEEP_SKY_BLUE);
                                }
                        }
                }
        }

        encode_png(filename, image.pixels(), image.width(), image.height());
        // save_ppm3(image.pixels(), image.width(), image.height(), 4, "test.ppm");
        
        /*
                unsigned char matrix[800][600][4];

                // Define a cor BRANCA
                std::memset(matrix, 255, 800 * 600 * 4);

                // Preenche com zeros os canais G e B, definindo o vermelho.
                for (int i = 0; i < 400; i++) {
                        for (int j = 0; j < 600; j++) {
                                matrix[i][j][1] = 0;
                                matrix[i][j][2] = 0;
                        }
                }

                encode_png(filename, (const unsigned char*)matrix, (unsigned)800, (unsigned)600);
        */
}