#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <stdexcept>

#include "include/my_exception.hpp"
#include "include/color.hpp"
#include "include/vec3.hpp"

std::ofstream create_output_fstream(int &argc, char* argv[]){
    // Check if the user provided a path to the PPM file 
    if (argc < 2) { 
        std::cerr << "Usage: " << argv[0] << " <output_file_path>\n"; 
        throw_line("incorrect commandline arguments"); 
    } 
    std::string output_file_path = argv[1];
    std::ofstream output_file{output_file_path}; 

    // If we couldn't open the output file stream for writing
    if (!output_file) {
        // Print an error and exit
        std::cerr << "file could not be opened for writing!\n";
        throw_line("file could not be opened for writing"); 
    }

    return output_file;
}

int main(int argc, char* argv[]){ 
    // Image

    int image_width = 256;
    int image_height = 256;

    // Render
    std::ostream output_file = create_output_fstream(argc,argv);

    output_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
            write_color(output_file, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}