#ifndef POINTCOLLECTION_H
#define POINTCOLLECTION_H


#include "Line.h"
#include <vector>


/**
 * @class PointCollection
 * @brief Manages a collection of points, providing functionality to add, find
 * nearest point, and draw points.
 *
 * The PointCollection class allows users to manage a set of vec3 points. It
 * supports adding points, finding the nearest point to a given location, and
 * rendering all points. This class is essential for graphical applications
 * where operations on multiple points are needed.
 */
class PointCollection {

    std::vector<vec3> points;

  public:
    void addPoint(vec3 p);
    [[nodiscard]] vec3 findNearestPoint(vec3 p) const;
    void draw(GPUProgram* prog) const;
};

#endif
