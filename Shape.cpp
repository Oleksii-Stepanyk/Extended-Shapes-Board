#include "Shape.h"
#include <cmath>

// Shape
Shape::Shape(const int id, std::string type, const int x, const int y) : id(id), type(std::move(type)), x(x), y(y)
{
}

int Shape::getId() const { return id; }
std::string Shape::getType() const { return type; }
int Shape::getX() const { return x; }
int Shape::getY() const { return y; }

std::string Shape::toString(bool isCommand)
{
    if (isCommand) { return "add"; }
    return std::to_string(id);
}

// Circle
Circle::Circle(const int id, const int radius, const int x, const int y)
    : Shape(id, "circle", x, y), radius(radius)
{
}

int Circle::getRadius() const { return radius; }

std::string Circle::toString(bool isCommand)
{
    const std::string var = Shape::toString(isCommand);
    return var + " circle " + std::to_string(radius)
        + ' ' + std::to_string(x) + ' ' + std::to_string(y);
}

std::vector<std::vector<char>> Circle::draw(bool Fill)
{
    std::vector figureGrid(2 * radius, std::vector(2 * radius, ' '));
    for (int i = 0; i <= 2 * radius; i++)
    {
        for (int j = 0; j <= 2 * radius; j++)
        {
            double distance = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));
            if (distance > radius - 0.65 && distance < radius + 0.2)
            {
                int gridX = x - radius + j;
                int gridY = y - radius + i;
                figureGrid[gridY][gridX] = '*';
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
Rectangle::Rectangle(const int id, const int width, const int height, const int x, const int y)
    : Shape(id, "rectangle", x, y), width(width), height(height)
{
}

int Rectangle::getWidth() const { return width; }
int Rectangle::getHeight() const { return height; }

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
        figureGrid[0][i] = '*';
        figureGrid[height - 1][i] = '*';
    }
    for (int i = 0; i < height; ++i)
    {
        figureGrid[i][0] = '*';
        figureGrid[i][width - 1] = '*';
    }
    return figureGrid;
}

bool Rectangle::operator==(const Rectangle& other) const
{
    return x == other.x && y == other.y && width == other.width && height == other.height;
}

// Triangle
Triangle::Triangle(const int id, const int height, const int x, const int y)
    : Shape(id, "triangle", x, y), height(height)
{
}

int Triangle::getHeight() const { return height; }

std::string Triangle::toString(bool isCommand)
{
    const std::string var = Shape::toString(isCommand);
    return var + " triangle " + std::to_string(height)
        + ' ' + std::to_string(x) + ' ' + std::to_string(y);;
}

std::vector<std::vector<char>> Triangle::draw(bool Fill)
{
    int width = 2 * height - 1;
    std::vector figureGrid(height, std::vector(width, ' '));
    for (int i = 0; i < height; ++i)
    {
        int leftMost = width - i;
        int rightMost = width + i;
        figureGrid[i][leftMost] = '*';
        figureGrid[i][rightMost] = '*';
    }
    for (int j = 0; j < width; ++j)
    {
        int baseX = height + 1 + j;
        int baseY = height - 1;
        figureGrid[baseY][baseX] = '*';
    }
}

bool Triangle::operator==(const Triangle& other) const
{
    return x == other.x && y == other.y && height == other.height;
}

// Line
Line::Line(const int id, const int x2, const int y2, const int x, const int y)
    : Shape(id, "line", x, y), x2(x2), y2(y2)
{
}

int Line::getX2() const { return x2; }
int Line::getY2() const { return y2; }

std::string Line::toString(bool isCommand)
{
    const std::string var = Shape::toString(isCommand);
    return var + " line " + std::to_string(x) + ' ' + std::to_string(y)
        + ' ' + std::to_string(x2) + ' ' + std::to_string(y2);
}

std::vector<std::vector<char>> Line::draw(bool Fill)
{
    int coordX2 = x2 - x;
    int coordY2 = y2 - y;
    int coordX = 0;
    int coordY = 0;
    int dx = abs(coordX - coordX2);
    int dy = abs(coordY2 - coordY);
    std::vector figureGrid(dy, std::vector(dx, ' '));
    int sx = (coordX < coordX2) ? 1 : -1;
    int sy = (coordY < coordY2) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        figureGrid[coordX][coordY] = '*';

        if (coordX == coordX2 && coordY == coordY2) break;
        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            coordX += sx;
        }
        if (e2 < dx) {
            err += dx;
            coordY += sy;
        }
    }
    return figureGrid;
}

bool Line::operator==(const Line& other) const
{
    return x == other.x && y == other.y && x2 == other.x2 && y2 == other.y2;
}
