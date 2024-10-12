#pragma once
#include <vector>
#include "Shape.h"

std::vector<std::string> split(const std::string& str, const char& delimiter);

class BlackBoard
{
    std::string availableShapes[4] = {
        "circle fill|frame color radius coordinates", "rectangle fill|frame color width height coordinates",
        "triangle fill|frame color height coordinates", "line color coordStart coordEnd"
    };

    int boardWidth = 0;
    int boardHeight = 0;
    Shape* selectedShape;
    std::vector<Shape*> shapes;
    std::vector<std::vector<char>> grid;
    void _insertShape(Shape*, bool);
    void _redrawBoard();

public
:
    BlackBoard(int, int);

    void draw();
    void list() const;
    void showShapes();
    void addShape(const std::vector<std::string>&);
    void select(const std::string&);
    void select(const std::vector<std::string>&);
    void remove();
    void edit(const std::vector<std::string>&);
    void paint(const std::string&);
    void move(const std::vector<std::string>&);
    void clear();

    std::vector<std::string> _getShapes();
    std::string getHeight() const;
    std::string getWidth() const;
};
