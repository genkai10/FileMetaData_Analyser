#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <concepts>
#include <array>
#include <sstream>
#include <stdexcept>
#include <cstring>

// Define a concept for signature
template<typename T>
concept Signature = requires(T s) {
    { std::size(s) } -> std::same_as<size_t>;
    { s.data() } -> std::convertible_to<const char*>;
};

// File signature and type pair
template<Signature S>
struct SignatureFileType {
    S signature;
    const char* fileType;
};

// List of supported file signatures and types
constexpr std::array fileSignatures = {
    SignatureFileType<std::array<char, 8>>{{'\xFF', '\xD8', '\xFF', '\xE0', '\x00', '\x10', '\x4A', '\x46'}, "JPEG"},
    SignatureFileType<std::array<char, 8>>{{'\x89', '\x50', '\x4E', '\x47', '\x0D', '\x0A', '\x1A', '\x0A'}, "PNG"},
    SignatureFileType<std::array<char, 4>>{{'\x25', '\x50', '\x44', '\x46'}, "PDF"},
    SignatureFileType<std::array<char, 8>>{{'\xD0', '\xCF', '\x11', '\xE0', '\xA1', '\xB1', '\x1A', '\xE1'}, "DOC"},
    SignatureFileType<std::array<char, 6>>{{'\x47', '\x49', '\x46', '\x38', '\x39', '\x61'}, "GIF"},
    SignatureFileType<std::array<char, 2>>{{'\x2F', '\x2F'}, "CPP"},
    SignatureFileType<std::array<char, 3>>{{'\xEF', '\xBB', '\xBF'}, "UTF-8 text"}
};

// Function to get file type from signature
template<Signature S>
std::string getFileType(const S& signature) {
    for (const auto& entry : fileSignatures) {
        if (std::memcmp(signature.data(), entry.signature.data(), signature.size()) == 0) {
            return entry.fileType;
        }
    }
    return "Unknown";
}

// Function to read file signature and determine file type
template<Signature S>
std::string determineFileType(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    S buffer;
    if (!file.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
        throw std::runtime_error("Error reading file: " + filename);
    }

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
    try {
        std::string type = determineFileType<std::array<char, 8>>(filename); // Adjust size according to your needs
        std::cout << "Signature and file type for " << filename << ": " << type << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
