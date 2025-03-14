#include "PointCollection.h"
#include <iostream>



void PointCollection::addPoint(vec3 p) {
    points.push_back(p);
    std::cout << "Point added: (" << p.x << ", " << p.y << ")\n";
}


vec3 PointCollection::findNearestPoint(vec3 p) {
    float minDist = 1.0f;
    vec3 closest = vec3(0, 0, 1);
    for (const auto &pt: points) {
        float dist = length(pt - p);
        if (dist < minDist) {
            minDist = dist;
            closest = pt;
        }
    }
    return closest;
}


void PointCollection::draw() const {
    Geometry<vec3> geom;
    geom.Vtx() = points;
    geom.updateGPU();
    glPointSize(10.0f);
    geom.Draw(new GPUProgram(), GL_POINTS, vec3(1, 0, 0)); // Red
}
