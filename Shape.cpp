#include "Shape.h"

Shape::Shape(const int id, std::string type, const int x, const int y) : id(id), type(std::move(type)), x(x), y(y) {}

int Shape::getId() const { return id; }
std::string Shape::getType() const { return type; }
int Shape::getX() const { return x; }
int Shape::getY() const { return y; }

Circle::Circle(const int id, const int radius, const int x, const int y)
: Shape(id, "circle", x, y), radius(radius) {}

int Circle::getRadius() const { return radius; }

Rectangle::Rectangle(const int id, const int width, const int height, const int x, const int y)
: Shape(id, "rectangle", x, y), width(width), height(height) {}

int Rectangle::getWidth() const { return width; }
int Rectangle::getHeight() const { return height; }

Triangle::Triangle(const int id, const int height, const int x, const int y)
: Shape(id, "triangle", x, y), height(height) {}

int Triangle::getHeight() const { return height; }

Line::Line(const int id, const int x2, const int y2, const int x, const int y)
: Shape(id, "line", x, y), x2(x2), y2(y2){}

int Line::getX2() const { return x2; }
int Line::getY2() const { return y2; }