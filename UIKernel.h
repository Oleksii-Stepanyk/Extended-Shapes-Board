#pragma once
#include <memory>
#include <string>
#include "BlackBoard.h"

class UIKernel
{
    std::unique_ptr<BlackBoard> board;

public:
    UIKernel();
    ~UIKernel() = default;
    void startProgram();
    void parseCommand(const std::string&, bool& isOkay);
    void loadFile(const std::string&);
    void saveFile(const std::string&);
};