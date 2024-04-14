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