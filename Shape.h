#pragma once
#include <string>
#include <vector>

class Shape
{
protected:
    int id = 0;
    std::string figure;
    std::string type;
    std::string color;
    int x;
    int y;

public:
    Shape(int, std::string, std::string, std::string, int, int);
    virtual ~Shape() = default;
    int getID() const;
    std::string getType() const;
    int getX() const;
    int getY() const;
    void setColor(const std::string&);
    void setX(int);
    void setY(int);

    virtual std::vector<std::vector<char>> draw(bool)
    {
        return {};
    }

    virtual std::string toString(bool);
};

class Circle final : public Shape
{
    int radius;

public:
    Circle(int, const std::string&, const std::string&, int, int, int);
    std::vector<std::vector<char>> draw(bool) override;
    std::string toString(bool) override;
    bool operator ==(const Circle&) const;
    int getRadius() const;
};

class Rectangle final : public Shape
{
    int width;
    int height;

public:
    Rectangle(int, const std::string&, const std::string&, int, int, int, int);
    std::vector<std::vector<char>> draw(bool) override;
    std::string toString(bool) override;
    bool operator ==(const Rectangle&) const;
    int getWidth() const;
    int getHeight() const;
};

class Triangle final : public Shape
{
    int height;

public:
    Triangle(int, const std::string&, const std::string&, int, int, int);
    std::vector<std::vector<char>> draw(bool) override;
    std::string toString(bool) override;
    bool operator ==(const Triangle&) const;
    int getHeight() const;
};

class Line final : public Shape
{
    int x2;
    int y2;

public:
    Line(int, const std::string&, const std::string&, int, int, int, int);
    std::vector<std::vector<char>> draw(bool) override;
    std::string toString(bool) override;
    bool operator ==(const Line&) const;
    int getX2() const;
    int getY2() const;
};
