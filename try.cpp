#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <string>

int main() {
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;

    int width, height, channels;
    if (stbi_info(filename.c_str(), &width, &height, &channels)) {
        std::cout << "Image dimensions: " << width << " x " << height << std::endl;
    } else {
        std::cerr << "Failed to retrieve image dimensions." << std::endl;
    }

    return 0;
}
