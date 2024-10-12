#include "BlackBoard.h"
#include <iostream>

BlackBoard::BlackBoard(const int width, const int height) : grid(height, std::vector(width, ' '))
{
    boardWidth = width;
    boardHeight = height;
}

void BlackBoard::draw()
{
    std::cout << "Board: \n";
    for (auto& i : grid)
    {
        for (char j : i)
        {
            std::cout << j;
        }
        std::cout << '\n';
    }
}

void BlackBoard::list() const
{
    if (shapes.empty())
    {
        std::cout << "No shapes\n";
        return;
    }
    for (auto shape : shapes)
    {
        std::cout << shape->toString(false) << '\n';
    }
}

void BlackBoard::showShapes()
{
    for (std::string& shape : availableShapes)
    {
        std::cout << shape << '\n';
    }
}

void BlackBoard::addShape(std::vector<std::string>& parameters)
{
    if (parameters.size() < 6 || parameters.size() > 7)
    {
        std::cout << "Invalid parameters\n";
    }
    else if (parameters[0] == "circle" && parameters.size() == 6)
    {
        const bool isFill = parameters[1] == "fill";
        const std::string color = parameters[2];
        const int radius = std::stoi(parameters[3]);
        const int x = std::stoi(parameters[4]);
        const int y = std::stoi(parameters[5]);
        if (x + radius < 0 || x - radius > boardWidth || y + radius < 0 || y - radius > boardHeight || radius <= 0 ||
            radius * 2 > boardWidth)
        {
            std::cout << "Circle is completely out of bounds\n";
            return;
        }
        auto* circle = new Circle(shapes.size() + 1, radius, x, y);
        for (auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Circle*>(shape); currentShape != nullptr)
            {
                if (*currentShape == *circle)
                {
                    std::cout << "Circle is overlapping with another circle\n";
                    delete circle;
                    return;
                }
            }
        }
        _updateBoard(circle, isFill);
    }
    else if (parameters[0] == "rectangle" && parameters.size() == 7)
    {
        const bool isFill = parameters[1] == "fill";
        const std::string color = parameters[2];
        const int width = std::stoi(parameters[3]);
        const int height = std::stoi(parameters[4]);
        const int x = std::stoi(parameters[5]);
        const int y = std::stoi(parameters[6]);
        if (x + width < 0 || y + height < 0 || x > boardWidth || y > boardHeight ||
            width <= 0 || width > boardWidth || height <= 0 || height > boardHeight)
        {
            std::cout << "Rectangle is completely out of bounds\n";
            return;
        }
        auto* rectangle = new Rectangle(shapes.size() + 1, width, height, x, y);
        for (auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Rectangle*>(shape); currentShape != nullptr)
            {
                if (*currentShape==*rectangle)
                {
                    std::cout << "Rectangle is overlapping with another rectangle\n";
                    delete rectangle;
                    return;
                }
            }
        }
        _updateBoard(rectangle, isFill);
    }
    else if (parameters[0] == "triangle" && parameters.size() == 6)
    {
        const bool isFill = parameters[1] == "fill";
        const std::string color = parameters[2];
        const int height = std::stoi(parameters[3]);
        const int x = std::stoi(parameters[4]);
        const int y = std::stoi(parameters[5]);
        if (x + height < 0 || y + height < 0 || x > boardWidth || y > boardHeight || height <= 1 || height >
            boardHeight)
        {
            std::cout << "Triangle is completely out of bounds\n";
            return;
        }
        for (auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Triangle*>(shape); currentShape != nullptr)
            {
                if (currentShape->getX() == x && currentShape->getY() == y && currentShape->getHeight() == height)
                {
                    std::cout << "Triangle is overlapping with another triangle\n";
                    return;
                }
            }
        }
        auto* triangle = new Triangle(shapes.size() + 1, height, x, y);
        _updateBoard(triangle, isFill);
    }
    else if (parameters[0] == "line" && parameters.size() == 6)
    {
        bool isFill = false;
        const std::string color = parameters[1];
        const int x = std::stoi(parameters[2]);
        const int y = std::stoi(parameters[3]);
        const int x2 = std::stoi(parameters[4]);
        const int y2 = std::stoi(parameters[5]);
        if (((x < 0 || y < 0) && (x2 < 0 || y2 < 0)) ||
            ((x >= boardWidth || y >= boardHeight) && (x2 >= boardWidth || y2 >= boardHeight)))
        {
            std::cout << "Line is completely out of bounds\n";
            return;
        }
        for (auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Line*>(shape); currentShape != nullptr)
            {
                if (currentShape->getX() == x && currentShape->getY() == y && currentShape->getX2() == x2 &&
                    currentShape->getY2() == y2)
                {
                    std::cout << "Line is overlapping with another line\n";
                    return;
                }
            }
        }
        auto* line = new Line(shapes.size() + 1, x, y, x2, y2);
        _updateBoard(line, isFill);
    }
    else
    {
        std::cout << "Invalid command\n";
    }
}

void BlackBoard::clear()
{
    grid = std::vector(boardHeight, std::vector(boardWidth, ' '));
    for (const auto shape : shapes)
    {
        delete shape;
    }
    shapes.clear();
}

std::vector<std::string> BlackBoard::_getShapes()
{
    std::vector<std::string> shapeCommands;
    for (auto shape : shapes)
    {
        shapeCommands.push_back(shape->toString(true));
    }
    return shapeCommands;
}

std::string BlackBoard::getHeight() const
{
    return std::to_string(boardHeight);
}

std::string BlackBoard::getWidth() const
{
    return std::to_string(boardWidth);
}

void BlackBoard::_updateBoard(Shape* shape, bool isFill)
{
    shapes.push_back(shape);
    shape->draw(isFill);
}
