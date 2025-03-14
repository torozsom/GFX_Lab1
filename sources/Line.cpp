#include "Line.h"

Line::Line(const vec3 point1, const vec3 point2) : p1(point1), p2(point2) {
    A = p2.y - p1.y;
    B = p1.x - p2.x;
    C = A * p1.x + B * p1.y;
}

bool Line::contains(const vec3 p) const {
    return fabs(A * p.x + B * p.y - C) < 0.01;
}

vec3 Line::computeIntersection(const Line &other) const {
    float det = A * other.B - other.A * B;
    if (fabs(det) < 1e-6) return vec3(0, 0, 0); // No intersection

    float x = (other.B * C - B * other.C) / det;
    float y = (A * other.C - other.A * C) / det;
    return vec3(x, y, 1);
}

void Line::translate(vec3 p) {
    C = A * p.x + B * p.y;
}

void Line::draw() {
    Geometry<vec3> geom;
    geom.Vtx() = {p1, p2};
    geom.updateGPU();
    geom.Draw(new GPUProgram(), GL_LINES, vec3(0, 1, 1)); // Cyan color
}
