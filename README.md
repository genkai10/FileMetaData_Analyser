# GP_Team_Project_FileMetaDataAnalayzer
Team project for Generic programming course - UE21CS342BA1
# File Metadata Analyzer

This project is a simple C++ program that analyzes file metadata, including file type, size, creation time, and last modified time. It also supports extracting image dimensions for image files.

## Description

The program consists of two main files:
- `signature.cpp`: Contains functions to determine file type based on file signatures using type traits and template friendship.
- `templatized_FMA.cpp`: Uses the functions defined in `signature.cpp` to analyze file metadata and extract image dimensions.

## Usage

1. **Clone the Repository:**
   ```sh

   git clone https://github.com/genkai10/FileMetaData_Analyser.git

2. **Compile the Code:**
   ```sh
   g++ templatized_FMA.cpp -o analyze_metadata

3. **Run the Program:**
   ```sh
   ./analyze_metadata

4. **Enter File Path:**
Enter the path to the file you want to analyze when prompted.

5. **View Output:**
The program will display the metadata of the file, including its type, size, creation time, last modified time, and image dimensions (if applicable).

## Testing

You can add different types of files to the repository to test the program with various file types. Ensure that the testing data files are located in the main branch of the repository alongside the code files.
