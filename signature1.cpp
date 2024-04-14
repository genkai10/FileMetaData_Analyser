#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip> // for std::hex
#include <cstring>

// Signature and file type pair
struct SignatureFileType {
    std::string signature;
    std::string fileType;
};

// List of signature and file type pairs
std::vector<SignatureFileType> signatures = {
    {"\xFF\xD8\xFF\xE0\x00\x10\x4A\x46", "JPEG"},
    {"\x89\x50\x4E\x47\x0D\x0A", "PNG"},
    {"\x25\x50\x44\x46", "PDF File"},
    {"\x50\x4B\x03\x04\x14\x00\x06\x00", "DOCX File"},
    {"\x47\x49\x46\x38\x39\x61", "GIF"},
    {"\x23", "CPP Source File"},
    {"\xEF\xBB\xBF", "UTF-8 text"},
    {"\x00\x00\x00\x18\x66\x74\x79\x70", "MP4"},
    // Add more signatures as needed
};

// Lambda template for determining file type based on signature
auto determineFileType = [](const std::string& filename, size_t signatureSize) -> std::string {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "Unknown";
    }

    char buffer[signatureSize];
    file.read(buffer, sizeof(buffer));

    for (const auto& signature : signatures) {
        const std::string& sig = signature.signature;
        if (std::memcmp(buffer, sig.c_str(), std::min(signatureSize, sig.size())) == 0) {
            return signature.fileType;
        }
    }

    return "Unknown";
};

// int main(int argc, char* argv[]) {
//     if (argc != 3) {
//         std::cerr << "Usage: " << argv[0] << " <filename> <signatureSize>" << std::endl;
//         return 1;
//     }

//     std::string filename = argv[1];
//     size_t signatureSize = std::stoul(argv[2]);
//     std::string type = determineFileType(filename, signatureSize);
//     std::cout << "File type for " << filename << ": " << type << std::endl;

//     return 0;
// }
