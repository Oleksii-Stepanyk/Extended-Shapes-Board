#include "BlackBoard.h"

#include <cmath>
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
        if (auto* currentShape = dynamic_cast<Circle*>(shape))
        {
            std::cout << currentShape->getId() << " circle " << currentShape->getRadius() << ' ' << currentShape->getX()
                << ' ' << currentShape->getY() << '\n';
        }
        else if (auto* currentShape = dynamic_cast<Rectangle*>(shape))
        {
            std::cout << currentShape->getId() << " rectangle " << currentShape->getWidth() << ' ' << currentShape->
                getHeight()
                << ' ' << currentShape->getX() << ' ' << currentShape->getY() << '\n';
        }
        else if (auto* currentShape = dynamic_cast<Triangle*>(shape))
        {
            std::cout << currentShape->getId() << " triangle " << currentShape->getHeight() << ' ' << currentShape->
                getX()
                << ' ' << currentShape->getY() << '\n';
        }
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
    if (parameters.size() < 4 || parameters.size() > 5)
    {
        std::cout << "Invalid parameters\n";
    }
    else if (parameters[0] == "circle" && parameters.size() == 4)
    {
        const int radius = std::stoi(parameters[1]);
        const int x = std::stoi(parameters[2]);
        const int y = std::stoi(parameters[3]);
        if (x + radius < 0 || x - radius > boardWidth || y + radius < 0 || y - radius > boardHeight || radius <= 0 ||
            radius * 2 > boardWidth)
        {
            std::cout << "Circle is completely out of bounds\n";
            return;
        }
        for (auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Circle*>(shape); currentShape != nullptr)
            {
                if (currentShape->getX() == x && currentShape->getY() == y && currentShape->getRadius() == radius)
                {
                    std::cout << "Circle is overlapping with another circle\n";
                    return;
                }
            }
        }
        auto* circle = new Circle(shapes.size() + 1, radius, x, y);
        shapes.push_back(circle);
        previousGrid = grid;
        drawCircle(radius, x, y);
        canUndo = true;
    }
    else if (parameters[0] == "rectangle" && parameters.size() == 5)
    {
        const int width = std::stoi(parameters[1]);
        const int height = std::stoi(parameters[2]);
        const int x = std::stoi(parameters[3]);
        const int y = std::stoi(parameters[4]);
        if (x + width < 0 || y + height < 0 || x > boardWidth || y > boardHeight ||
            width <= 0 || width > boardWidth || height <= 0 || height > boardHeight)
        {
            std::cout << "Rectangle is completely out of bounds\n";
            return;
        }
        for (auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Rectangle*>(shape); currentShape != nullptr)
            {
                if (currentShape->getX() == x && currentShape->getY() == y && currentShape->getHeight() == height &&
                    currentShape->getWidth() == width)
                {
                    std::cout << "Rectangle is overlapping with another rectangle\n";
                    return;
                }
            }
        }
        auto* rectangle = new Rectangle(shapes.size() + 1, width, height, x, y);
        shapes.push_back(rectangle);
        previousGrid = grid;
        drawRectangle(width, height, x, y);
        canUndo = true;
    }
    else if (parameters[0] == "triangle" && parameters.size() == 4)
    {
        const int height = std::stoi(parameters[1]);
        const int x = std::stoi(parameters[2]);
        const int y = std::stoi(parameters[3]);
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
        shapes.push_back(triangle);
        previousGrid = grid;
        drawTriangle(height, x, y);
        canUndo = true;
    }
    else if (parameters[0] == "line" && parameters.size() == 5)
    {
        const int x = std::stoi(parameters[1]);
        const int y = std::stoi(parameters[2]);
        const int x2 = std::stoi(parameters[3]);
        const int y2 = std::stoi(parameters[4]);
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
        auto* triangle = new Line(shapes.size() + 1, x, y, x2, y2);
        shapes.push_back(triangle);
        drawLine(x, y, x2, y2);
        previousGrid = grid;
        canUndo = true;
    }
    else
    {
        std::cout << "Invalid command\n";
    }
}

void BlackBoard::undo()
{
    if (canUndo)
    {
        grid = previousGrid;
        previousGrid.clear();

        const auto lastShape = shapes.back();
        shapes.pop_back();
        delete lastShape;

        canUndo = false;
    }
    else
    {
        std::cout << "Nothing to undo\n";
    }
}

void BlackBoard::clear()
{
    grid = std::vector(boardHeight, std::vector(boardWidth, ' '));
    previousGrid.clear();
    canUndo = false;
    for (const auto shape : shapes)
    {
        delete shape;
    }
    shapes.clear();
}

std::vector<std::string> BlackBoard::_getShapes()
{
    std::vector<std::string> shapeCommands;
    std::string command;
    for (auto shape : shapes)
    {
        if (auto* currentShape = dynamic_cast<Circle*>(shape))
        {
            command += "add circle " + std::to_string(currentShape->getRadius()) + ' '
                + std::to_string(currentShape->getX()) + ' ' + std::to_string(currentShape->getY());
        }
        else if (auto* currentShape = dynamic_cast<Rectangle*>(shape))
        {
            command += "add rectangle " + std::to_string(currentShape->getWidth()) + ' '
                + std::to_string(currentShape->getHeight()) + ' ' + std::to_string(currentShape->getX()) + ' '
                + std::to_string(currentShape->getY());
        }
        else if (auto* currentShape = dynamic_cast<Triangle*>(shape))
        {
            command += "add triangle " + std::to_string(currentShape->getHeight()) + ' '
                + std::to_string(currentShape->getX()) + ' ' + std::to_string(currentShape->getY());
        }
        else if (auto* currentShape = dynamic_cast<Line*>(shape))
        {
            command += "add line " + std::to_string(currentShape->getX()) + ' ' + std::to_string(currentShape->getY()) +
                ' '
                + std::to_string(currentShape->getX2()) + ' ' + std::to_string(currentShape->getY2());
        }
        shapeCommands.push_back(command);
        command.clear();
    }
    return shapeCommands;
}

void BlackBoard::drawCircle(int radius, int x, int y)
{
    for (int i = 0; i <= 2 * radius; i++)
    {
        for (int j = 0; j <= 2 * radius; j++)
        {
            double distance = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));
            if (distance > radius - 0.65 && distance < radius + 0.2)
            {
                int gridX = x - radius + j;
                int gridY = y - radius + i;
                if (gridX >= 0 && gridX < boardWidth && gridY >= 0 && gridY < boardHeight)
                {
                    grid[gridY][gridX] = '*';
                }
            }
        }
    }
}

void BlackBoard::drawRectangle(int width, int height, int x, int y)
{
    for (int i = 0; i < width; ++i)
    {
        if (y >= 0 && y < boardHeight && x + i >= 0 && x + i < boardWidth)
        {
            grid[y][x + i] = '*';
        }
        if (y + height - 1 >= 0 && y + height - 1 < boardHeight && x + i >= 0 && x + i < boardWidth)
        {
            grid[y + height - 1][x + i] = '*';
        }
    }
    for (int i = 0; i < height; ++i)
    {
        if (y + i >= 0 && y + i < boardHeight && x >= 0 && x < boardWidth)
        {
            grid[y + i][x] = '*';
        }
        if (y + i >= 0 && y + i < boardHeight && x + width - 1 >= 0 && x + width - 1 < boardWidth)
        {
            grid[y + i][x + width - 1] = '*';
        }
    }
}

void BlackBoard::drawTriangle(int height, int x, int y)
{
    for (int i = 0; i < height; ++i)
    {
        int leftMost = x - i;
        int rightMost = x + i;
        int posY = y + i;
        if (posY < boardHeight)
        {
            if (leftMost >= 0 && leftMost < boardWidth)
                grid[y + i][x - i] = '*';
            if (rightMost >= 0 && rightMost < boardWidth && leftMost != rightMost)
                grid[y + i][x + i] = '*';
        }
    }
    for (int j = 0; j < 2 * height - 1; ++j)
    {
        int baseX = x - height + 1 + j;
        int baseY = y + height - 1;
        if (baseX >= 0 && baseX < boardWidth && baseY < boardHeight)
            grid[baseY][baseX] = '*';
    }
}

void BlackBoard::drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        grid[y1][x1] = '*';

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}


std::string BlackBoard::getHeight() const
{
    return std::to_string(boardHeight);
}

std::string BlackBoard::getWidth() const
{
    return std::to_string(boardWidth);
}
