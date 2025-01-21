#include "include/common.hpp"
#include "include/my_exception.hpp"
#include "include/color.hpp"
#include "include/vec3.hpp"

// TODO: refactor to have output stream be like tiny-raytracer
// auto render() {

// Function to create output file stream
std::ofstream create_output_fstream(int &argc, char* argv[], int image_width = 256, int image_height = 256) {
    // Check if the user provided a path to the PPM file
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <output_file_path>\n";
        throw_line("incorrect command line arguments");
    }

    std::ofstream output_file{argv[1]};
    if (!output_file) {
        std::cerr << "file could not be opened for writing!\n";
        throw_line("file could not be opened for writing");
    }
    
    output_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    return output_file;
}

int main(int argc, char* argv[]) {
    // Image
    int image_width = 256;
    int image_height = 256;

    std::ofstream output_file = create_output_fstream(argc, argv);
    

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
            write_color(output_file, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}
