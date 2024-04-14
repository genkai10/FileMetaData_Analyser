#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

// Base class for signature and file type pair
class SignatureFileType {
public:
    virtual ~SignatureFileType() {}
    virtual bool matchSignature(const char* buffer, size_t bufferSize) const = 0;
    virtual std::string getFileType() const = 0;
};

// Derived class for JPEG file type
class JPEGFileType : public SignatureFileType {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 8 && std::memcmp(buffer, "\xFF\xD8\xFF\xE0\x00\x10\x4A\x46", 8) == 0;
    }

    std::string getFileType() const override {
        return "JPEG";
    }
};

// Derived class for PNG file type
class PNGFileType : public SignatureFileType {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 6 && std::memcmp(buffer, "\x89\x50\x4E\x47\x0D\x0A", 6) == 0;
    }

    std::string getFileType() const override {
        return "PNG";
    }
};

// Derived class for PDF file type
class PDFFileType : public SignatureFileType {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 4 && std::memcmp(buffer, "\x25\x50\x44\x46", 4) == 0;
    }

    std::string getFileType() const override {
        return "PDF";
    }
};

// Derived class for DOCX file type
class DOCXFileType : public SignatureFileType {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 8 && std::memcmp(buffer, "\x50\x4B\x03\x04\x14\x00\x06\x00", 8) == 0;
    }

    std::string getFileType() const override {
        return "DOCX";
    }
};

// Derived class for MP3 file type
class MP3FileType : public SignatureFileType {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 3 && std::memcmp(buffer, "\x49\x44\x33", 3) == 0;
    }

    std::string getFileType() const override {
        return "MP3";
    }
};

// Derived class for MP4 file type
class MP4FileType : public SignatureFileType {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 8 && std::memcmp(buffer, "\x00\x00\x00\x18\x66\x74\x79\x70", 8) == 0;
    }

    std::string getFileType() const override {
        return "MP4";
    }
};


// Derived class for GIF file type
class GIFFileType : public SignatureFileType {
public:
    bool matchSignature(const char* buffer, size_t bufferSize) const override {
        return bufferSize >= 6 && std::memcmp(buffer, "\x47\x49\x46\x38\x39\x61", 6) == 0;
    }

    std::string getFileType() const override {
        return "GIF";
    }
};

// Add more derived classes for other file types as needed...

// Lambda template for determining file type based on signature
auto determineFileType = [](const std::string& filename, size_t signatureSize) -> std::string {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "Unknown";
    }

    char buffer[signatureSize];
    file.read(buffer, sizeof(buffer));

    // Array of file type objects
    SignatureFileType* fileTypeObjects[] = {new JPEGFileType(), new PNGFileType(), new PDFFileType(), new DOCXFileType(), new MP3FileType(), new MP4FileType() /* Add more objects as needed... */};

    // Iterate over file type objects and check if any matches the signature
    for (const auto& fileType : fileTypeObjects) {
        if (fileType->matchSignature(buffer, signatureSize)) {
            std::string fileTypeString = fileType->getFileType();
            for(auto &f : fileTypeObjects)
            {
                delete f;
            }
            return fileTypeString;
        }
    }
    for(auto &f : fileTypeObjects)
    {
        delete f;
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
