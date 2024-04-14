#include <iostream>
#include <string>
#include <chrono>

// Class to represent file metadata
template<typename... Attributes>
class FileMetadata {
private:
    std::string filename;
    // Variadic template for metadata attributes
    std::tuple<Attributes...> attributes;

public:
    FileMetadata(const std::string& filename, const Attributes&... attrs) 
        : filename(filename), attributes(std::make_tuple(attrs...)) {}

    // Getters for filename and metadata attributes
    const std::string& getFilename() const {
        return filename;
    }

    // Function to get specific metadata attribute by type
    template<typename T>
    const T& getAttribute() const {
        return std::get<T>(attributes);
    }
};
