#include "BlackBoard.h"

#include <cmath>
#include <iostream>
#include <sstream>

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

BlackBoard::BlackBoard(const int width, const int height) : grid(height, std::vector(width, ' '))
{
    selectedShape = nullptr;
    boardWidth = width;
    boardHeight = height;
}

void BlackBoard::draw()
{
    std::cout << "Board: \n";
    for (auto& i : grid)
    {
        for (const char j : i)
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
    for (const auto shape : shapes)
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

void BlackBoard::addShape(const std::vector<std::string>& parameters, bool& isOkay)
{
    if (parameters.size() < 6 || parameters.size() > 7)
    {
        std::cout << "Invalid parameters\n";
        isOkay = false;
    }
    else if (parameters[0] == "circle" && parameters.size() == 6)
    {
        const bool isFill = parameters[1] == "fill";
        const std::string& type = parameters[1];
        const std::string& color = parameters[2];
        const int radius = std::stoi(parameters[3]);
        const int x = std::stoi(parameters[4]);
        const int y = std::stoi(parameters[5]);
        if (x + radius < 0 || x - radius > boardWidth || y + radius < 0 || y - radius > boardHeight || radius <= 0 ||
            radius * 2 > boardWidth)
        {
            std::cout << "Circle is completely out of bounds\n";
            return;
        }
        auto* circle = new Circle(shapes.size() + 1, type, color, radius, x, y);
        for (const auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Circle*>(shape); currentShape != nullptr)
            {
                if (*currentShape == *circle)
                {
                    std::cout << "Circle is overlapping with another circle\n";
                    delete circle;
                    isOkay = false;
                    return;
                }
            }
        }
        shapes.push_back(circle);
        _insertShape(circle, isFill);
    }
    else if (parameters[0] == "rectangle" && parameters.size() == 7)
    {
        const bool isFill = parameters[1] == "fill";
        const std::string& type = parameters[1];
        const std::string& color = parameters[2];
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
        auto* rectangle = new Rectangle(shapes.size() + 1, type, color, width, height, x, y);
        for (const auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Rectangle*>(shape); currentShape != nullptr)
            {
                if (*currentShape == *rectangle)
                {
                    std::cout << "Rectangle is overlapping with another rectangle\n";
                    delete rectangle;
                    isOkay = false;
                    return;
                }
            }
        }
        shapes.push_back(rectangle);
        _insertShape(rectangle, isFill);
    }
    else if (parameters[0] == "triangle" && parameters.size() == 6)
    {
        const bool isFill = parameters[1] == "fill";
        const std::string& type = parameters[1];
        const std::string& color = parameters[2];
        const int height = std::stoi(parameters[3]);
        const int x = std::stoi(parameters[4]);
        const int y = std::stoi(parameters[5]);
        if (x + height < 0 || y + height < 0 || x > boardWidth || y > boardHeight || height <= 1 || height >
            boardHeight)
        {
            std::cout << "Triangle is completely out of bounds\n";
            return;
        }
        auto* triangle = new Triangle(shapes.size() + 1, type, color, height, x, y);
        for (const auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Triangle*>(shape); currentShape != nullptr)
            {
                if (*currentShape == *triangle)
                {
                    std::cout << "Triangle is overlapping with another triangle\n";
                    delete triangle;
                    isOkay = false;
                    return;
                }
            }
        }
        shapes.push_back(triangle);
        _insertShape(triangle, isFill);
    }
    else if (parameters[0] == "line" && parameters.size() == 6)
    {
        constexpr bool isFill = false;
        const std::string& color = parameters[1];
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
        auto* line = new Line(shapes.size() + 1, "frame", color, x, y, x2, y2);
        for (const auto shape : shapes)
        {
            if (const auto* currentShape = dynamic_cast<Line*>(shape); currentShape != nullptr)
            {
                if (*currentShape == *line)
                {
                    std::cout << "Line is overlapping with another line\n";
                    delete line;
                    isOkay = false;
                    return;
                }
            }
        }
        shapes.push_back(line);
        _insertShape(line, isFill);
    }
    else
    {
        std::cout << "Invalid command\n";
        isOkay = false;
    }
}

void BlackBoard::select(const std::string& id)
{
    if (std::stoi(id) > shapes.size())
    {
        std::cout << "Invalid ID\n";
        return;
    }
    for (const auto shape : shapes)
    {
        if (shape->getID() == std::stoi(id))
        {
            selectedShape = shape;
            return;
        }
    }
}

void BlackBoard::select(const std::vector<std::string>& parameters)
{
    const int x = std::stoi(parameters[0]);
    const int y = std::stoi(parameters[1]);

    for (int i = shapes.size() - 1; i >= 0; --i)
    {
        const auto shape = shapes[i];
        auto shapeGrid = shape->draw(shape->getType() == "fill");
        int shapeX = shape->getX();
        int shapeY = shape->getY();

        if (auto curShape = dynamic_cast<Circle*>(shape))
        {
            const int radius = curShape->getRadius();
            if (const double distance = sqrt((x - shapeX) * (x - shapeX) + (y - shapeY) * (y - shapeY)); distance <=
                radius)
            {
                selectedShape = shape;
                return;
            }
        }
        else if (auto curShape = dynamic_cast<Rectangle*>(shape))
        {
            const int width = curShape->getWidth();
            const int height = curShape->getHeight();
            if (x >= shapeX && x < shapeX + width && y >= shapeY && y < shapeY + height)
            {
                selectedShape = shape;
                return;
            }
        }
        else if (auto curShape = dynamic_cast<Triangle*>(shape))
        {
            const int height = curShape->getHeight();
            const int baseX = shapeX;
            const int baseY = shapeY + height - 1;
            if (y >= shapeY && y <= baseY && x >= baseX - (y - shapeY) && x <= baseX + (y - shapeY))
            {
                selectedShape = shape;
                return;
            }
        }
        else if (auto curShape = dynamic_cast<Line*>(shape))
        {
            const int x2 = curShape->getX2();
            const int y2 = curShape->getY2();
            const int dx = abs(x2 - shapeX);
            const int dy = abs(y2 - shapeY);
            const int sx = (shapeX < x2) ? 1 : -1;
            const int sy = (shapeY < y2) ? 1 : -1;
            int err = dx - dy;

            while (true)
            {
                if (shapeX == x && shapeY == y)
                {
                    selectedShape = shape;
                    return;
                }
                if (shapeX == x2 && shapeY == y2) break;
                const int e2 = 2 * err;
                if (e2 > -dy)
                {
                    err -= dy;
                    shapeX += sx;
                }
                if (e2 < dx)
                {
                    err += dx;
                    shapeY += sy;
                }
            }
        }
    }
    std::cout << "No shape found at the given coordinates\n";
}

void BlackBoard::remove()
{
    if (selectedShape == nullptr)
    {
        std::cout << "No shape selected\n";
        return;
    }
    for (int i = 0; i < shapes.size(); i++)
    {
        if (shapes[i] == selectedShape)
        {
            shapes.erase(shapes.begin() + i);
            break;
        }
    }
    _redrawBoard();
}

void BlackBoard::edit(const std::vector<std::string>& args)
{
    if (args.empty() || args.size() > 4)
    {
        std::cout << "Invalid parameters count\n";
        return;
    }
    if (selectedShape == nullptr)
    {
        std::cout << "No shape selected\n";
        return;
    }
    std::string id = std::to_string(selectedShape->getID());
    std::string shape = selectedShape->toString(true);
    std::vector<std::string> parameters = split(shape, ' ');
    for (int i = 0; i < args.size(); ++i)
    {
        parameters[i + 4] = args[i];
    }
    parameters.erase(parameters.begin());
    shapes.erase(std::ranges::find(shapes, selectedShape));
    bool isOkay = true;
    addShape(parameters, isOkay);
    if (!isOkay)
    {
        shapes.push_back(selectedShape);
        _redrawBoard();
        return;
    }
    select(id);
    _redrawBoard();
}

void BlackBoard::paint(const std::string& newColor)
{
    if (selectedShape == nullptr)
    {
        std::cout << "No shape selected\n";
        return;
    }
    selectedShape->setColor(newColor);
    _redrawBoard();
}

void BlackBoard::move(const std::vector<std::string>& coords)
{
    if (selectedShape == nullptr)
    {
        std::cout << "No shape selected\n";
        return;
    }
    selectedShape->setX(std::stoi(coords[0]));
    selectedShape->setY(std::stoi(coords[1]));
    shapes.erase(std::ranges::find(shapes, selectedShape));
    shapes.push_back(selectedShape);
    _redrawBoard();
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

std::string BlackBoard::getHeight() const
{
    return std::to_string(boardHeight);
}

std::string BlackBoard::getWidth() const
{
    return std::to_string(boardWidth);
}

std::vector<std::string> BlackBoard::_getShapes()
{
    std::vector<std::string> shapeCommands;
    for (const auto shape : shapes)
    {
        std::string shapeCommand = shape->toString(true);
        shapeCommands.push_back(shapeCommand);
    }
    return shapeCommands;
}

void BlackBoard::_insertShape(Shape* shape, bool isFill)
{
    std::vector<std::vector<char>> shapeGrid = shape->draw(isFill);
    for (int i = 0; i < shapeGrid.size(); i++)
    {
        for (int j = 0; j < shapeGrid[i].size(); j++)
        {
            if (shapeGrid[i][j] != ' ' && shape->getX() + j >= 0 && shape->getY() + i >= 0 && shape->getY() + i
                < boardHeight && shape->getX() + j < boardWidth)
            {
                grid[shape->getY() + i][shape->getX() + j] = shapeGrid[i][j];
            }
        }
    }
}

void BlackBoard::_redrawBoard()
{
    grid = std::vector(boardHeight, std::vector(boardWidth, ' '));
    for (const auto shape : shapes)
    {
        _insertShape(shape, shape->getType() == "fill");
    }
}
