#include "FileHandler.h"

FileHandler::FileHandler(const std::string& filename, const char* mode)
{
    file = fopen(filename.c_str(), mode);
}

FileHandler::~FileHandler()
{
    fclose(file);
}

std::string FileHandler::getData() const
{
    std::string result;
    while (!feof(file))
    {
        char buffer[256];
        fgets(buffer, 256, file);
        result.append(buffer);
    }
    return result;
}

void FileHandler::writeData(const std::string& data) const
{
    fputs(data.c_str(), file);
}
