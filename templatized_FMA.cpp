#include <iostream>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include "signature.cpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <string>
#include <concepts>

namespace fs = std::filesystem;

// Define a struct to hold metadata
struct FileMetadata {
    std::string filename;
    std::string path;
    std::size_t size; // size in bytes
    std::time_t creation_time;
    std::time_t last_modified_time;
    std::string type;
};

// Define a struct to hold image dimensions
struct ImageDimensions {
    int width;
    int height;
};

// Function to get file metadata
template<typename... Args>
FileMetadata getMetadata(const std::string& filename, Args&&... args) {
    FileMetadata metadata;
    metadata.filename = filename;
    metadata.path = fs::absolute(filename).string(); // get absolute path
    
    // Calculate metadata based on variadic arguments
    (..., args(metadata)); // Call each function in args with metadata
    
    return metadata;
}

// Functions to calculate different metadata
void calculateSize(FileMetadata& metadata) {
    metadata.size = fs::file_size(metadata.path);
}

void calculateCreationTime(FileMetadata& metadata) {
    struct stat fileStat;
    if (stat(metadata.path.c_str(), &fileStat) == 0) {
        metadata.creation_time = fileStat.st_ctime;
    } else {
        std::cerr << "Error: Unable to get creation time for " << metadata.path << std::endl;
    }
}

void calculateLastModifiedTime(FileMetadata& metadata) {
    struct stat fileStat;
    if (stat(metadata.path.c_str(), &fileStat) == 0) {
        metadata.last_modified_time = fileStat.st_mtime;
    } else {
        std::cerr << "Error: Unable to get last modified time for " << metadata.path << std::endl;
    }
}

// Concept for handling image files
template<typename T>
concept ImageHandler = requires(T handler, const std::string& filename, ImageDimensions& dimensions) {
    { stbi_info(filename.c_str(), &dimensions.width, &dimensions.height, nullptr) };
};

// Template specialization for image dimensions
template <ImageHandler T>
void getMetadata(const std::string& filename, ImageDimensions& dimensions, T handler) {
    handler(filename, dimensions);
}

// Function to handle image files
void handleImage(const std::string& filename, ImageDimensions& dimensions) {
    int width, height, channels;
    if (stbi_info(filename.c_str(), &width, &height, &channels)) {
        dimensions.width = width;
        dimensions.height = height;
        std::cout << "Image Dimensions Retrieved Successfully: " << dimensions.width << "x" << dimensions.height << std::endl;
    } else {
        std::cerr << " This type of file does not have Image Dimensions supported" << filename << std::endl;
    }
}

int main() {
    std::string filepath;
    std::cout << "Enter the file path: ";
    std::cin >> filepath;
    
    // Get just the filename from the full path
    std::filesystem::path pathObj(filepath);
    std::string filename = pathObj.filename().string();
    
    // Get metadata using variadic templates
    FileMetadata metadata = getMetadata(filepath, calculateSize, calculateCreationTime, calculateLastModifiedTime);
    
    // Determine file type
    metadata.type = determineFileType<8>(metadata.path);
    
    // Print metadata
    std::cout << "Type: " << metadata.type << std::endl;
    std::cout << "Filename: " << metadata.filename << std::endl;
    std::cout << "Path: " << metadata.path << std::endl;
    std::cout << "Size: " << metadata.size << " bytes" << std::endl;
    std::cout << "Creation Time: " << std::asctime(std::localtime(&metadata.creation_time));
    std::cout << "Last Modified Time: " << std::asctime(std::localtime(&metadata.last_modified_time));
    
    ImageDimensions dimensions;
    getMetadata(metadata.path, dimensions, handleImage);
    
    return 0;
}

