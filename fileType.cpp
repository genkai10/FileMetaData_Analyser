#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstring>

std::map<std::string, std::string> fileSignatures = {
    {"\xFF\xD8\xFF", "JPEG"},
    {"\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", "PNG"},
    // Add more signatures as needed
};

std::string getFileType(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return "Unknown";
    }

    char buffer[8];
    file.read(buffer, sizeof(buffer));

    for (const auto& signature : fileSignatures) {
        if (memcmp(buffer, signature.first.c_str(), signature.first.size()) == 0) {
            return signature.second;
        }
    }

    return "Unknown";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string type = getFileType(filename);
    std::cout << "File signature for " << filename << ": " << type << std::endl;

    return 0;
}
