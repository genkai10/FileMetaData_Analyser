#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // Include this header for std::stringstream
#include <iomanip> // Include this header for std::setw and std::setfill
#include <unordered_map>

// Define magic numbers/signatures for various file types
std::unordered_map<std::string, std::string> fileSignatures = {
    {"FFD8FFE0", "JPEG"},
    {"89504E470D0A1A0A", "PNG"},
    {"474946383761", "GIF"},
    // Add more signatures as needed
};

// Function to read the header of the file and determine its type
std::string determineFileType(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return "Unknown";
    }

    char buffer[8] = {0}; // Read the first 8 bytes
    file.read(buffer, 8);

    std::stringstream ss;
    for (int i = 0; i < 8; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);
    }
    std::string signature = ss.str();

    file.close();

    // Check the signature against known file types
    for (const auto& entry : fileSignatures) {
        if (signature.find(entry.first) != std::string::npos) {
            return entry.second;
        }
    }

    return "Unknown";
}

int main() {
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;

    std::string fileType = determineFileType(filename);
    std::cout << "File type: " << fileType << std::endl;

    return 0;
}
