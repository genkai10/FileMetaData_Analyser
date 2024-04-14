#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>

// Forward declaration
template<typename T>
class SignatureFileType;

// Template friend function declaration
template<typename T>
std::string determineFileType(const std::string& filename, size_t signatureSize);

// Base class for signature and file type pair
template<typename T>
class SignatureFileType {
    friend std::string determineFileType<T>(const std::string&, size_t);

public:
    virtual ~SignatureFileType() {}
    virtual bool matchSignature(const char* buffer, size_t bufferSize) const = 0;
    virtual std::string getFileType() const = 0;
};

// Derived class for JPEG file type
class JPEGFileType : public SignatureFileType<JPEGFileType> {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 3 && std::memcmp(buffer, "\xFF\xD8\xFF", 3) == 0;
    }

    std::string getFileType() const override {
        return "JPEG";
    }
};

// Derived class for PNG file type
class PNGFileType : public SignatureFileType<PNGFileType> {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 8 && std::memcmp(buffer, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8) == 0;
    }

    std::string getFileType() const override {
        return "PNG";
    }
};

// Derived class for PDF file type
class PDFFileType : public SignatureFileType<PDFFileType> {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 4 && std::memcmp(buffer, "\x25\x50\x44\x46", 4) == 0;
    }

    std::string getFileType() const override {
        return "PDF";
    }
};

// Derived class for DOCX file type
class DOCXFileType : public SignatureFileType<DOCXFileType> {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 4 && std::memcmp(buffer, "\x50\x4B\x03\x04", 4) == 0;
    }

    std::string getFileType() const override {
        return "DOCX";
    }
};

// Derived class for MP3 file type
class MP3FileType : public SignatureFileType<MP3FileType> {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 3 && std::memcmp(buffer, "ID3", 3) == 0;
    }

    std::string getFileType() const override {
        return "MP3";
    }
};

// Derived class for MP4 file type
class MP4FileType : public SignatureFileType<MP4FileType> {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 12 && std::memcmp(buffer, "\x00\x00\x00\x18\x66\x74\x79\x70\x4D\x53\x4E\x56", 12) == 0;
    }

    std::string getFileType() const override {
        return "MP4";
    }
};

// Derived class for GIF file type
class GIFFileType : public SignatureFileType<GIFFileType> {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 6 && std::memcmp(buffer, "GIF", 3) == 0;
    }

    std::string getFileType() const override {
        return "GIF";
    }
};

// Template friend function definition as a lambda
template<typename T>
auto determineFileType = [](const std::string& filename, size_t signatureSize) -> std::string {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "Unknown";
    }

    std::unique_ptr<char[]> buffer(new char[signatureSize]);
    file.read(buffer.get(), signatureSize);

    // Create an instance of the derived class
    T fileType;

    // Check if the signature matches
    if (fileType.matchSignature(buffer.get(), signatureSize)) {
        return fileType.getFileType();
    }

    return "Unknown";
};

// // Usage example
// int main() {
//     std::string filename = "example.jpg";
//     size_t signatureSize = 3; // Adjust the signature size as needed
//     std::string type = determineFileType<JPEGFileType>(filename, signatureSize);
//     std::cout << "File type for " << filename << ": " << type << std::endl;

//     return 0;
// }