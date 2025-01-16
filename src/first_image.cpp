#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

int main() {

    // ofstream is used for writing files
    std::ofstream output_file{ "images/first_image.ppm" };

    // If we couldn't open the output file stream for writing
    if (!output_file)
    {
        // Print an error and exit
        std::cerr << "file could not be opened for writing!\n";
        return 1;
    }

    // Image

    int image_width = 256;
    int image_height = 256;

    // Render

    output_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            output_file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::clog << "\rDone.                 \n";
}