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
    return var + " circle " + std::to_string(radius)
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
        }
    }
    return figureGrid;
}

bool Circle::operator==(const Circle& other) const
{
    return x == other.x && y == other.y && radius == other.radius;
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
    return var + " rectangle " + std::to_string(width) + ' ' + std::to_string(height)
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
    return figureGrid;
}

bool Rectangle::operator==(const Rectangle& other) const
{
    return x == other.x && y == other.y && width == other.width && height == other.height;
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
    return var + " triangle " + std::to_string(height)
        + ' ' + std::to_string(x) + ' ' + std::to_string(y);;
}

std::vector<std::vector<char>> Triangle::draw(bool Fill)
{
    int width = 2 * height - 1;
    std::vector figureGrid(height + 1, std::vector(width + 1, ' '));
    for (int i = 0; i < height; ++i)
    {
        int leftMost = width - i;
        int rightMost = width + i;
        figureGrid[i][leftMost] = color[0];
        figureGrid[i][rightMost] = color[0];
    }
    for (int j = 0; j < width; ++j)
    {
        int baseX = height + 1 + j;
        int baseY = height - 1;
        figureGrid[baseY][baseX] = color[0];
    }
    return figureGrid;
}

bool Triangle::operator==(const Triangle& other) const
{
    return x == other.x && y == other.y && height == other.height;
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
    return var + " line " + std::to_string(x) + ' ' + std::to_string(y)
        + ' ' + std::to_string(x2) + ' ' + std::to_string(y2);
}

std::vector<std::vector<char>> Line::draw(bool Fill)
{
    int dx = abs(x - x2);
    int dy = abs(y2 - y);
    std::vector figureGrid(2*dy+1, std::vector(2*dx+1, ' '));
    int x = (2 * dx - 1) / 2;
    int y = (2 * dy - 1) / 2;
    int x2 = this->x2 - x;
    int y2 = this->y2 - y;
    int sx = (x < x2) ? 1 : -1;
    int sy = (y < y2) ? 1 : -1;

    int err = dx - dy;

    while (true)
    {
        figureGrid[x][y] = color[0];

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
    return figureGrid;
}

bool Line::operator==(const Line& other) const
{
    return x == other.x && y == other.y && x2 == other.x2 && y2 == other.y2;
}
