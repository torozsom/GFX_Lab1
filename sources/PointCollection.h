#ifndef POINTCOLLECTION_H
#define POINTCOLLECTION_H

#include <vector>
#include "Line.h"



/**
 * @class PointCollection
 * @brief Manages a collection of points, providing functionality to add, find nearest point, and draw points.
 *
 * The PointCollection class allows users to manage a set of vec3 points. It supports adding points,
 * finding the nearest point to a given location, and rendering all points. This class is essential for
 * graphical applications where operations on multiple points are needed.
 */
class PointCollection {
private:
    std::vector<vec3> points;

public:
    void addPoint(vec3 p);

    vec3 findNearestPoint(vec3 p);

    void draw(GPUProgram* prog) const;
};

#endif
