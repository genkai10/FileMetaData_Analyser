#include <iostream>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include "signature3.cpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <string>
#include <concepts>
#include <vector>
#include <algorithm>
#include <typeinfo>

namespace fs = std::filesystem;

template<typename T>
concept IdenticalString = requires(T str) {
    { str == std::string("Unknown") }; // Requires T to be comparable with the given string
};


// Define a struct to hold metadata
struct FileMetadata {
    std::string filename;
    std::string path;
    std::size_t size; // size in bytes
    std::time_t creation_time;
    std::time_t last_modified_time;
    std::string type;
    int width = 0; // additional metadata for image files
    int height = 0; // additional metadata for image files

    // Declare getMetadata as a friend function template
    template<typename... Args>
    friend FileMetadata getMetadata(const std::string& filename, Args&&... args);
};

template<typename T>
void compareStrings(const T& str) requires IdenticalString<T> {

    if (str == "Unknown") {
        std::cout << "Variable is 'Unknown'. Ending program." << std::endl;
        std::exit(EXIT_FAILURE);
    } else {
        std::cout << "File Type known. Proceeding" << std::endl;
    }
}

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

// Function to handle extra metadata for JPEG files
void handleExtraMetadataJPEG(FileMetadata& metadata) {
    int width, height, channels;
    unsigned char* image = stbi_load(metadata.path.c_str(), &width, &height, &channels, 0);
    if (image) {
        metadata.width = width;
        metadata.height = height;
        stbi_image_free(image);
    } else {
        std::cerr << "Error: Unable to load image " << metadata.path << std::endl;
    }
}

// Function to handle extra metadata for PNG files
void handleExtraMetadataPNG(FileMetadata& metadata) {
    handleExtraMetadataJPEG(metadata); // PNG metadata handled similarly to JPEG
}

// Function to handle extra metadata for GIF files
void handleExtraMetadataGIF(FileMetadata& metadata) {
    // GIF metadata handling logic here
     handleExtraMetadataJPEG(metadata);
}

// Templated function to handle extra metadata based on file type
template<typename FileType>
void handleExtraMetadata(FileMetadata& metadata);

// Specialization for JPEG files
template<>
void handleExtraMetadata<std::integral_constant<int, 0>>(FileMetadata& metadata) {
    handleExtraMetadataJPEG(metadata);
}

// Specialization for PNG files
template<>
void handleExtraMetadata<std::integral_constant<int, 1>>(FileMetadata& metadata) {
    handleExtraMetadataPNG(metadata);
}

// Specialization for GIF files
template<>
void handleExtraMetadata<std::integral_constant<int, 2>>(FileMetadata& metadata) {
    handleExtraMetadataGIF(metadata);
}

int main() {
    std::string filepath;
    std::cout << "Enter the file path: ";
    std::cin >> filepath;
    
    // Get just the filename from the full path
    std::filesystem::path pathObj(filepath);
    std::string filename = pathObj.filename().string();
    
    

    // Get metadata for the single file
    FileMetadata metadata = getMetadata(filepath, calculateSize, calculateCreationTime, calculateLastModifiedTime);
    
    // Determine file type
    // metadata.type = determineFileType(metadata.path, 8);

    const std::type_info& type = typeid(metadata.type);
    std::cout << "Type of metadata.type: " << type.name() << std::endl;

    compareStrings(metadata.type);

    // Handle extra metadata based on file type
    if (metadata.type == "JPEG") {
        handleExtraMetadata<std::integral_constant<int, 0>>(metadata);
    } else if (metadata.type == "PNG") {
        handleExtraMetadata<std::integral_constant<int, 1>>(metadata);
    } else if (metadata.type == "GIF") {
        handleExtraMetadata<std::integral_constant<int, 2>>(metadata);
    }
    
    // Print metadata for the single file
    std::cout << "Metadata for Single File:" << std::endl;
    std::cout << "Type: " << metadata.type << std::endl;
    std::cout << "Filename: " << metadata.filename << std::endl;
    std::cout << "Path: " << metadata.path << std::endl;
    std::cout << "Size: " << metadata.size << " bytes" << std::endl;
    std::cout << "Creation Time: " << std::asctime(std::localtime(&metadata.creation_time));
    std::cout << "Last Modified Time: " << std::asctime(std::localtime(&metadata.last_modified_time));
    if (metadata.width!=0 && metadata.height!=0) {
        std::cout << "Width: " << metadata.width << std::endl;
        std::cout << "Height: " << metadata.height << std::endl;
    }
    
    // Ask user if they want to compare multiple files
    char choice;
    std::cout << "Do you want to compare multiple files? (y/n): ";
    std::cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        std::vector<std::string> filepaths;
        std::string filepath;
        do {
            std::cout << "Enter the file path: ";
            std::cin >> filepath;
            filepaths.push_back(filepath);
            std::cout << "Do you want to add another file? (y/n): ";
            std::cin >> choice;
        } while (choice == 'y' || choice == 'Y');
        
        // Gather metadata for each file
        std::vector<FileMetadata> metadatas;
        for (const auto& path : filepaths) {
            FileMetadata metadata = getMetadata(path, calculateSize, calculateCreationTime, calculateLastModifiedTime);
            metadata.type = determineFileType(metadata.path, 8);
            if (metadata.type == "JPEG") {
                handleExtraMetadata<std::integral_constant<int, 0>>(metadata);
            } else if (metadata.type == "PNG") {
                handleExtraMetadata<std::integral_constant<int, 1>>(metadata);
            } else if (metadata.type == "GIF") {
                handleExtraMetadata<std::integral_constant<int, 2>>(metadata);
            }
            metadatas.push_back(metadata);
        }
        
        // Find the largest file - lambda expressions are used here
        auto largestFile = std::max_element(metadatas.begin(), metadatas.end(), [](const FileMetadata& a, const FileMetadata& b) {
            return a.size < b.size;
        });
        
        // Print metadata for the largest file
        std::cout << "Largest File:" << std::endl;
        std::cout << "Type: " << largestFile->type << std::endl;
        std::cout << "Filename: " << largestFile->filename << std::endl;
        std::cout << "Path: " << largestFile->path << std::endl;
        std::cout << "Size: " << largestFile->size << " bytes" << std::endl;
        std::cout << "Creation Time: " << std::asctime(std::localtime(&largestFile->creation_time));
        std::cout << "Last Modified Time: " << std::asctime(std::localtime(&largestFile->last_modified_time));
        if (metadata.width!=0 && metadata.height!=0) {
            std::cout << "Width: " << largestFile->width << std::endl;
            std::cout << "Height: " << largestFile->height << std::endl;
        }
    }
    
    return 0;
}
