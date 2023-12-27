#ifndef GFILEREADER_H
#define GFILEREADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

class gFileReader {
public:
    static std::string ReadFile(std::string filePath) {

        std::string content;
        std::ifstream fileStream;

        // Ensure ifstream objects can throw exceptions:
        fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // Open files
            fileStream.open(filePath);
            std::stringstream contentStream;

            // Read file's buffer contents into stream
            contentStream << fileStream.rdbuf();

            // Close file handlers
            fileStream.close();

            // Convert stream into string
            content = contentStream.str();
        } catch (std::ifstream::failure& e) {
            printf("error: failed to read file with path %s (%s)\n", filePath.c_str(), e.what());
        }

        return content;
    }

};


#endif // GFILEREADER_H