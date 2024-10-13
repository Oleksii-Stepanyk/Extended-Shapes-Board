#include "Shape.h"
#include <cmath>

// Shape
Shape::Shape(const int id, std::string figure, std::string type, std::string color, const int x, const int y) :
    id(id), figure(std::move(figure)), type(std::move(type)), color(std::move(color)), x(x), y(y)
{
}

std::string Shape::toString(bool isCommand)
{
    if (isCommand) { return "add"; }
    return std::to_string(id);
}

int Shape::getID() const
{
    return id;
}

std::string Shape::getType() const
{
    return type;
}

int Shape::getX() const
{
    return x;
}

int Shape::getY() const
{
    return y;
}

void Shape::setColor(const std::string& newColor)
{
    color = newColor;
}

void Shape::setX(int newX)
{
    x = newX;
}

void Shape::setY(int newY)
{
    y = newY;
}

// Circle
Circle::Circle(const int id, const std::string& type, const std::string& color, const int radius, const int x,
               const int y)
    : Shape(id, "circle", type, color, x, y), radius(radius)
{
}

std::string Circle::toString(bool isCommand)
{
    const std::string var = Shape::toString(isCommand);
    return var + " circle " + type + ' ' + color + ' ' + std::to_string(radius)
        + ' ' + std::to_string(x) + ' ' + std::to_string(y);
}

std::vector<std::vector<char>> Circle::draw(bool Fill)
{
    std::vector figureGrid(2 * radius + 1, std::vector(2 * radius + 1, ' '));
    for (int i = 0; i <= 2 * radius; i++)
    {
        for (int j = 0; j <= 2 * radius; j++)
        {
            double distance = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));
            if (distance > radius - 0.65 && distance < radius + 0.2)
            {
                int gridX = j;
                int gridY = i;
                figureGrid[gridY][gridX] = color[0];
            }
            else if (Fill && distance <= radius - 0.65)
            {
                int gridX = j;
                int gridY = i;
                figureGrid[gridY][gridX] = color[0];
            }
        }
    }
    return figureGrid;
}

bool Circle::operator==(const Circle& other) const
{
    return x == other.x && y == other.y && radius == other.radius;
}

int Circle::getRadius() const
{
    return radius;
}

// Rectangle
Rectangle::Rectangle(const int id, const std::string& type, const std::string& color, const int width, const int height,
                     const int x, const int y)
    : Shape(id, "rectangle", type, color, x, y), width(width), height(height)
{
}


std::string Rectangle::toString(bool isCommand)
{
    const std::string var = Shape::toString(isCommand);
    return var + " rectangle " + type + ' ' + color + ' ' + std::to_string(width) + ' ' + std::to_string(height)
        + ' ' + std::to_string(x) + ' ' + std::to_string(y);
}

std::vector<std::vector<char>> Rectangle::draw(bool Fill)
{
    std::vector figureGrid(height, std::vector(width, ' '));
    for (int i = 0; i < width; ++i)
    {
        figureGrid[0][i] = color[0];
        figureGrid[height - 1][i] = color[0];
    }
    for (int i = 0; i < height; ++i)
    {
        figureGrid[i][0] = color[0];
        figureGrid[i][width - 1] = color[0];
    }
    if (Fill)
    {
        for (int i = 1; i < height - 1; ++i)
        {
            for (int j = 1; j < width - 1; ++j)
            {
                figureGrid[i][j] = color[0];
            }
        }
    }
    return figureGrid;
}

bool Rectangle::operator==(const Rectangle& other) const
{
    return x == other.x && y == other.y && width == other.width && height == other.height;
}

int Rectangle::getWidth() const
{
    return width;
}

int Rectangle::getHeight() const
{
    return height;
}

// Triangle
Triangle::Triangle(const int id, const std::string& type, const std::string& color, const int height, const int x,
                   const int y)
    : Shape(id, "triangle", type, color, x, y), height(height)
{
}


std::string Triangle::toString(bool isCommand)
{
    const std::string var = Shape::toString(isCommand);
    return var + " triangle " + type + ' ' + color + ' ' + std::to_string(height)
        + ' ' + std::to_string(x) + ' ' + std::to_string(y);;
}

std::vector<std::vector<char>> Triangle::draw(bool Fill)
{
    int width = 2 * height - 1;
    std::vector figureGrid(height, std::vector(width, ' '));
    for (int i = 0; i < height; ++i)
    {
        int leftMost = height - 1 - i;
        int rightMost = height - 1 + i;
        figureGrid[i][leftMost] = color[0];
        figureGrid[i][rightMost] = color[0];

        if (Fill)
        {
            for (int j = leftMost + 1; j < rightMost; ++j)
            {
                figureGrid[i][j] = color[0];
            }
        }
    }
    for (int j = 0; j < width; ++j)
    {
        int baseX = j;
        int baseY = height - 1;
        figureGrid[baseY][baseX] = color[0];
    }
    return figureGrid;
}

bool Triangle::operator==(const Triangle& other) const
{
    return x == other.x && y == other.y && height == other.height;
}

int Triangle::getHeight() const
{
    return height;
}

// Line
Line::Line(const int id, const std::string& type, const std::string& color, const int x2, const int y2, const int x,
           const int y)
    : Shape(id, "line", type, color, x, y), x2(x2), y2(y2)
{
}

std::string Line::toString(bool isCommand)
{
    const std::string var = Shape::toString(isCommand);
    return var + " line " + color + ' ' + std::to_string(x) + ' ' + std::to_string(y)
        + ' ' + std::to_string(x2) + ' ' + std::to_string(y2);
}

std::vector<std::vector<char>> Line::draw(bool)
{
    int width = std::max(x, x2) + 1;
    int height = std::max(y, y2) + 1;
    std::vector grid(height, std::vector<char>(width, ' '));

    int dx = abs(x2 - x);
    int dy = abs(y2 - y);
    int sx = (x < x2) ? 1 : -1;
    int sy = (y < y2) ? 1 : -1;

    int err = dx - dy;

    while (true)
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            grid[y][x] = color[0];
        }

        if (x == x2 && y == y2) break;
        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }

    return grid;
}

bool Line::operator==(const Line& other) const
{
    return x == other.x && y == other.y && x2 == other.x2 && y2 == other.y2;
}

int Line::getX2() const
{
    return x2;
}

int Line::getY2() const
{
    return y2;
}