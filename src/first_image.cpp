#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

int main(int argc, char* argv[]){ 

    // Check if the user provided a path to the PPM file 
    if (argc < 2) { 
        std::cerr << "Usage: " << argv[0] << " <output_file_path>\n"; 
        return 1; 
    } 
    std::string output_file_path = argv[1];
    std::ofstream output_file{output_file_path}; 

    // If we couldn't open the output file stream for writing
    if (!output_file) {
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