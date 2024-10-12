#pragma once
#include <cstdio>
#include <string>

class FileHandler
{
    FILE* file;

public:
    [[nodiscard]] std::string getData() const;
    void writeData(const std::string&) const;
    bool isOpen() const { return file != nullptr; }

    explicit FileHandler(const std::string&, const char*);
    ~FileHandler();

    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
    FileHandler(FileHandler&&) = delete;
    FileHandler& operator=(FileHandler&&) = delete;
};
