#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <filesystem>
#include <concepts>

// Forward declaration of templatizedFunction
template<typename T>
void templatizedFunction();

// Define a traits class to associate file signatures with their types
template<unsigned long long Signature>
struct FileTypeTraits;

// Specialization for JPEG files
template<>
struct FileTypeTraits<0xFFD8FFE000104A46ULL> {
    static constexpr const char* type = "JPEG";
    friend void templatizedFunction<char>();
};

// Specialization for PNG files
template<>
struct FileTypeTraits<0x89504E470D0A1A0AULL> {
    static constexpr const char* type = "PNG";
    friend void templatizedFunction<char>();
};

// Specialization for PDF files
template<>
struct FileTypeTraits<0x255044462D312E3> {
    static constexpr const char* type = "PDF File";
    friend void templatizedFunction<char>();
};

// Specialization for DOCX files
template<>
struct FileTypeTraits<0x504B030414000600ULL> {
    static constexpr const char* type = "DOCX File";
    friend void templatizedFunction<char>();
};

// Specialization for GIF files
template<>
struct FileTypeTraits<0x474946383961ULL> {
    static constexpr const char* type = "GIF";
    friend void templatizedFunction<char>();
};

// Specialization for CPP source files
template<>
struct FileTypeTraits<0x23ULL> {
    static constexpr const char* type = "CPP Source File";
    friend void templatizedFunction<char>();
};

// Specialization for UTF-8 text files
template<>
struct FileTypeTraits<0xEFBBBFULL> {
    static constexpr const char* type = "UTF-8 text";
    friend void templatizedFunction<char>();
};

// Function to get file type from signature
template<unsigned long long Signature>
std::string getFileType() {
    return FileTypeTraits<Signature>::type;
}

// Concept for validating signature size
template<size_t N>
concept ValidSignatureSize = (N >= 8); // Adjust as needed

// Templated function to get file type from signature
template<size_t N>
std::string getFileType(const char (&signature)[N]) requires ValidSignatureSize<N>;

// Function to read file signature and determine file type
template<size_t N>
std::string determineFileType(const std::string& filename) requires ValidSignatureSize<N>;

#endif // SIGNATURE_HPP