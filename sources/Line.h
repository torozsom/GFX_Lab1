#ifndef LINE_H
#define LINE_H

#include "framework.h"

class Line {
private:
    vec3 p1, p2;
    float A, B, C;

public:
    Line(vec3 point1, vec3 point2);

    bool contains(vec3 p) const;

    vec3 computeIntersection(const Line &other) const;

    void translate(vec3 newPoint);

    void draw() const;

    void printEquations() const;
};

#endif
