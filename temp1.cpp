#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // for std::hex
#include <concepts> // for concepts
#include <cstring>

// Define a concept for signature
template<typename T>
concept Signature = requires(T s) {
    { std::size(s) } -> std::same_as<size_t>;
    { s.data() } -> std::convertible_to<const char*>;
};

// Define a concept for file type
template<typename T>
concept FileType = requires(T s) {
    { std::string(s) };
};

// File signature and type pair
template<Signature S, FileType T>
struct SignatureFileType {
    S signature;
    T fileType;
};

// List of supported file signatures and types
constexpr auto fileSignatures = [] {
    return std::array{
        SignatureFileType{"\xFF\xD8\xFF\xE0\x00\x10\x4A\x46", "JPEG"},
        SignatureFileType{"\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", "PNG"},
        SignatureFileType{"\x25\x50\x44\x46", "PDF"},
        SignatureFileType{"\xD0\xCF\x11\xE0\xA1\xB1\x1A\xE1", "DOC"},
        SignatureFileType{"\x47\x49\x46\x38\x39\x61", "GIF"},
        SignatureFileType{"\x2F\x2F", "CPP"},
        SignatureFileType{"\xEF\xBB\xBF", "UTF-8 text"}
        // Add more signatures as needed
    };
}();

// Function to get file type from signature
template<Signature S>
std::string getFileType(const S& signature) {
    for (const auto& entry : fileSignatures) {
        if (std::memcmp(signature.data(), entry.signature, std::min(signature.size(), std::size(entry.signature))) == 0) {
            return entry.fileType;
        }
    }
    return "Unknown";
}

// Function to read file signature and determine file type
template<Signature S>
std::string determineFileType(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "Unknown";
    }

    S buffer;
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    std::string fileType = getFileType(buffer);
    std::stringstream signatureStream;
    signatureStream << std::hex << std::uppercase << std::setfill('0');
    for (size_t i = 0; i < buffer.size(); ++i) {
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
    std::string type = determineFileType<std::array<char, 8>>(filename); // Adjust size according to your needs
    std::cout << "Signature and file type for " << filename << ": " << type << std::endl;

    return 0;
}
