
    } else {
        std::cerr << " This type of file does not have Image Dimensions supported" << filename << std::endl;
    }
}

int main() {
    std::string filepath;
    std::cout << "Enter the file path: ";
    std::cin >> filepath;
    
    // Get just the filename from the full path
    std::filesystem::path pathObj(filepath);
    std::string filename = pathObj.filename().string();
    
    // Get metadata using variadic templates
    FileMetadata metadata = getMetadata(filepath, calculateSize, calculateCreationTime, calculateLastModifiedTime);
    