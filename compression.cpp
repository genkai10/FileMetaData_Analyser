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
#include <iomanip>
#include <zlib.h>

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
        std::cerr << "This type of file does not have Image Dimensions supported" << std::endl;
    }
}

// Type trait to determine if a file is compressible
template<typename T>
concept CompressibleFile = std::is_same_v<T, std::ifstream>;

// Function to compress a file using zlib
void compressFile(const std::string& filename, const std::string& fileType) {
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file for compression: " << filename << std::endl;
        return;
    }

    std::ofstream compressedFile(filename + ".compressed", std::ios::binary);
    if (!compressedFile) {
        std::cerr << "Error: Unable to create compressed file" << std::endl;
        return;
    }

    z_stream zs;
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;

    if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) {
        std::cerr << "Error: deflateInit failed" << std::endl;
        return;
    }

    char buffer[1024];
    zs.avail_in = 0;
    zs.next_in = Z_NULL;

    do {
        inputFile.read(buffer, sizeof(buffer));
        zs.avail_in = static_cast<uInt>(inputFile.gcount());
        if (zs.avail_in == 0)
            break;
        zs.next_in = reinterpret_cast<Bytef*>(buffer);

        int ret;
        do {
            zs.avail_out = sizeof(buffer);
            zs.next_out = reinterpret_cast<Bytef*>(buffer);

            ret = deflate(&zs, Z_FINISH);
            if (ret == Z_STREAM_ERROR) {
                std::cerr << "Error: deflate failed" << std::endl;
                (void)deflateEnd(&zs);
                return;
            }

            compressedFile.write(buffer, sizeof(buffer) - zs.avail_out);
        } while (zs.avail_out == 0);
    } while (inputFile.good());

    (void)deflateEnd(&zs);

    inputFile.close();
    compressedFile.close();

    std::string compressedFilename = filename + ".compressed." + fileType;
    
    if (rename((filename + ".compressed").c_str(), compressedFilename.c_str()) != 0) {
        std::cerr << "Error: Unable to rename compressed file" << std::endl;
        return;
    }

    std::cout << "File compressed successfully. Compressed file saved as: " << compressedFilename << std::endl;

    // Display metadata of compressed file
    FileMetadata compressedMetadata = getMetadata(compressedFilename, calculateSize, calculateCreationTime, calculateLastModifiedTime);
    std::cout << "Metadata for Compressed File:" << std::endl;
    std::cout << "Filename: " << compressedMetadata.filename << std::endl;
    std::cout << "Path: " << compressedMetadata.path << std::endl;
    std::cout << "Size: " << compressedMetadata.size << " bytes" << std::endl;
    std::cout << "Creation Time: " << std::put_time(std::localtime(&compressedMetadata.creation_time), "%c %Z") << std::endl;
    std::cout << "Last Modified Time: " << std::put_time(std::localtime(&compressedMetadata.last_modified_time), "%c %Z") << std::endl;
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
    metadata.type = determineFileType(metadata.path, 8);
    
    // Print metadata for the single file
    std::cout << "Metadata for Single File:" << std::endl;
    std::cout << "Type: " << metadata.type << std::endl;
    std::cout << "Filename: " << metadata.filename << std::endl;
    std::cout << "Path: " << metadata.path << std::endl;
    std::cout << "Size: " << metadata.size << " bytes" << std::endl;
    std::cout << "Creation Time: " << std::put_time(std::localtime(&metadata.creation_time), "%c %Z") << std::endl;
    std::cout << "Last Modified Time: " << std::put_time(std::localtime(&metadata.last_modified_time), "%c %Z") << std::endl;

    if (metadata.type == "PNG" || metadata.type == "JPEG" || metadata.type == "GIF") {
        // Handle image metadata retrieval
        ImageDimensions dimensions;
        getMetadata(metadata.path, dimensions, handleImage);
    } else {
        // Calculate metadata for non-image files
        std::cout << "Not an image type of file, hence no dimensions for this file" << std::endl;
    }

    // Ask user if they want to compress the file
    char compressChoice;
    std::cout << "Do you want to compress the file? (y/n): ";
    std::cin >> compressChoice;

    if (compressChoice == 'y' || compressChoice == 'Y') {
        compressFile(metadata.path,metadata.type);
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
        
        ImageDimensions dimensions;
        getMetadata(largestFile->path, dimensions, handleImage);
    }
    
    return 0;
}
