#include "PointCollection.h"


void PointCollection::addPoint(const vec3 p) {
    points.push_back(p);
}

vec3 PointCollection::findNearestPoint(const vec3 p) {
    float minDist = 1.0f;

    vec3 closest;
    for (auto &pt: points) {
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
    geom.Draw(new GPUProgram(), GL_POINTS, vec3(1, 0, 0)); // Red color
}
