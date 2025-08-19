#ifndef LINE_H
#define LINE_H


#include "framework.h"
#include <stdio.h>


/**
 * @brief Represents a 2D line defined by two points and its implicit and
 * parametric equations.
 *
 * This class provides functionalities for managing a line in a Descartes
 * coordinate system. The line is uniquely defined by two points, and its
 * implicit equation is represented as Ax + By = C.
 */
class Line {

    vec3 p1, p2;
    float A, B, C;

  public:
    Line(vec3 point1, vec3 point2);

    [[nodiscard]] bool contains(vec3 p) const;
    [[nodiscard]] vec3 computeIntersection(const Line& other) const;

    void translate(vec3 newPoint);
    void draw(GPUProgram* prog) const;
    void printEquations() const;
};

#endif
