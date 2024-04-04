#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip> // for std::hex
#include<cstring>

// File signature map
std::map<std::string, std::string> fileSignatures = {
    {"\xFF\xD8\xFF\xE0\x00\x10\x4A\x46", "JPEG"},
    {"\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", "PNG"},
    {"\x25\x50\x44\x46", "PDF"},
    {"\xD0\xCF\x11\xE0\xA1\xB1\x1A\xE1", "DOC"},
    {"\x47\x49\x46\x38\x39\x61", "GIF"},
    {"\x2F\x2F", "CPP"},
    {"\xEF\xBB\xBF", "UTF-8 text"},
    // Add more signatures as needed
};

// Templated function to get file type from signature
template<size_t N>
std::string getFileType(const char (&signature)[N]) {
    for (const auto& entry : fileSignatures) {
        const std::string& fileSignature = entry.first;
        if (memcmp(signature, fileSignature.c_str(),std::min(N, fileSignature.size())) == 0) {
            return entry.second;
        }
    }
    return "Unknown";
}

// Function to read file signature and determine file type
template<size_t N>
std::string determineFileType(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "Unknown";
    }

    char buffer[N];
    file.read(buffer, sizeof(buffer));

    std::string fileType = getFileType(buffer);
    std::stringstream signatureStream;
    signatureStream << std::hex << std::uppercase << std::setfill('0');
    for (size_t i = 0; i < N; ++i) {
        signatureStream << std::setw(2) << static_cast<int>(buffer[i] & 0xFF);
    }
    std::string signature = signatureStream.str();

    return signature + " (" + fileType + ")";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string type = determineFileType<8>(filename); // Adjust size according to your needs
    std::cout << "Signature and file type for " << filename << ": " << type << std::endl;

    return 0;
}
