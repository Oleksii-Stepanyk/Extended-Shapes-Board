#include <iostream>
#include <sstream>
#include "UIKernel.h"
#include "FileHandler.h"

std::vector<std::string> split(const std::string& str, const char& delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (getline(ss, item, delimiter))
    {
        result.push_back(item);
    }
    return result;
}

std::string to_lower(const std::string& str)
{
    std::string result = str;
    for (char& c : result)
    {
        c += 32 * (c >= 'A' && c <= 'Z');
    }
    return result;
}

UIKernel::UIKernel()
{
    std::cout << "Enter the width and height of the board: ";
    int height, width;
    std::cin >> width >> height;
    board = std::make_unique<BlackBoard>(width, height);
}

void UIKernel::startProgram()
{
    std::string input;
    std::cin.ignore();
    while (to_lower(input) != "exit")
    {
        bool isOkay = true;
        std::cout << "Enter command: ";
        getline(std::cin, input);
        parseCommand(to_lower(input), isOkay);
    }
}

void UIKernel::parseCommand(const std::string& input, bool& isOkay)
{
    std::vector<std::string> args = split(input, ' ');
    if (args.empty())
    {
        std::cout << "Invalid command. Please try again.\n";
    }
    else if (args[0] == "draw")
    {
        board->draw();
    }
    else if (args[0] == "list")
    {
        board->list();
    }
    else if (args[0] == "shapes")
    {
        board->showShapes();
    }
    else if (args[0] == "add")
    {
        args.erase(args.begin());
        board->addShape(args);
    }
    else if (args[0] == "undo")
    {
        board->undo();
    }
    else if (args[0] == "clear")
    {
        board->clear();
    }
    else if (args[0] == "save" && args.size() == 2)
    {
        saveFile(args[1]);
    }
    else if (args[0] == "load" && args.size() == 2)
    {
        loadFile(args[1]);
    }
    else if (args[0] == "exit")
    {
        std::cout << "Exiting program...\n";
    }
    else
    {
        std::cout << "Invalid command. Please try again.\n";
        isOkay = false;
    }
}

void UIKernel::loadFile(const std::string& path)
{
    const FileHandler file(path, "r");
    if (!file.isOpen())
    {
        std::cout << "Error: File not found.\n";
        return;
    }

    const std::string data = file.getData();
    if (data.empty())
    {
        std::cout << "Error: File is empty.\n";
        return;
    }

    std::vector<std::string> lines = split(data, '\n');
    if (lines[0] != "board version 1.0")
    {
        std::cout << "Error: Invalid file format.\n";
        return;
    }
    lines.erase(lines.begin());

    const auto boardSize = split(lines[0], ' ');
    board.reset();
    board = std::make_unique<BlackBoard>(std::stoi(boardSize[0]), std::stoi(boardSize[1]));
    lines.erase(lines.begin());

    for (const std::string& line : lines)
    {
        bool isOkay = true;
        parseCommand(line, isOkay);
        if (!isOkay)
        {
            std::cout << "Error: Invalid command in file.\n";
            return;
        }
    }
}

void UIKernel::saveFile(const std::string& path)
{
    const FileHandler file(path, "w");
    if (!file.isOpen())
    {
        std::cout << "Error: File not found.\n";
        return;
    }

    file.writeData("board version 1.0\n");
    file.writeData(board->getWidth() + ' ' + board->getHeight() + '\n');

    auto shapes = board->_getShapes();
    int shapesCount = 0;
    for (const std::string& shapeCommand : shapes)
    {
        if (shapesCount != shapes.size() - 1)
        {
            file.writeData(shapeCommand + '\n');
            ++shapesCount;
        }
        else
        {
            file.writeData(shapeCommand);
        }
    }
}
