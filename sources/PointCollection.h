#ifndef POINTCOLLECTION_H
#define POINTCOLLECTION_H

#include <vector>
#include "Line.h"

class PointCollection {
private:
    std::vector<vec3> points;

public:
    void addPoint(vec3 p);

    vec3 findNearestPoint(vec3 p);

    void draw() const;
};

#endif
