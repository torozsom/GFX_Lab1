#ifndef LINECOLLECTION_H
#define LINECOLLECTION_H

#include <vector>
#include "Line.h"


/**
 * @class LineCollection
 * @brief Manages a collection of lines, providing functionality to add, draw, and manipulate lines.
 *
 * The LineCollection class allows users to manage a set of Line objects. It supports adding lines
 * using two points, finding the nearest line to a given point, and rendering all lines.
 * This class is essential for graphical applications where operations on multiple lines are needed.
 */
class LineCollection {

private:
    std::vector<Line> lines;

public:
    void addLine(vec3 p1, vec3 p2);

    Line* findNearestLine(vec3 p);

    void draw() const;

    std::vector<Line>& getLines() { return lines; }
};

#endif
