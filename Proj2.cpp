#include <iostream>
#include <fstream>
#include <cstring> // Include this header for memcmp

// Function to check file signature
bool checkFileSignature(const std::string& filename, const std::string& signature) {
    std::ifstream file(filename, std::ios::binary);
    if (file) {
        char buffer[signature.size()];
        file.read(buffer, signature.size());
        return file && std::memcmp(buffer, signature.c_str(), signature.size()) == 0; // Use std::memcmp
    }
    return false;
}

int main() {
    std::string filename = "fileType.cpp";
    std::string jpgSignature = "\xFF\xD8\xFF"; // JPEG file signature

    if (checkFileSignature(filename, jpgSignature)) {
        std::cout << "File appears to be a JPEG." << std::endl;
    } else {
        std::cout << "File does not appear to be a JPEG." << std::endl;
    }

    return 0;
}
