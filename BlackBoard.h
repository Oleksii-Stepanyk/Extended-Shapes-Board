#pragma once
#include <vector>
#include "Shape.h"

class BlackBoard
{
    std::string availableShapes[4] = {
        "circle fill|frame color radius coordinates", "rectangle fill|frame color width height coordinates",
        "triangle fill|frame color height coordinates","line color coordStart coordEnd" };

int boardWidth = 0;
int boardHeight = 0;
std::vector<Shape*> shapes;
std::vector<std::vector<char>> grid;
void _updateBoard(Shape*, bool);

public
:
BlackBoard(int, int);   

void draw();
void list() const;
void showShapes();
void addShape(std::vector<std::string>& parameters);
void clear();

std::vector<std::string> _getShapes();
std::string getHeight() const;
std::string getWidth() const;
};
