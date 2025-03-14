#include "Line.h"
#include <iostream>


Line::Line(const vec3 point1, const vec3 point2) : p1(point1), p2(point2) {
    A = p2.y - p1.y;
    B = p1.x - p2.x;
    C = A * p1.x + B * p1.y;
    printEquations();
}


bool Line::contains(const vec3 p) const {
    float distance = fabs(A * p.x + B * p.y - C) / sqrt(A * A + B * B);
    return distance < 0.01f;
}


vec3 Line::computeIntersection(const Line &other) const {
    float det = A * other.B - other.A * B;
    if (fabs(det) < 1e-6f)
        return vec3(0, 0, 0);

    float x = (other.B * C - B * other.C) / det;
    float y = (A * other.C - other.A * C) / det;
    return vec3(x, y, 1);
}


void Line::translate(const vec3 newPoint) {
    C = A * newPoint.x + B * newPoint.y;
    vec3 direction = p2 - p1;
    float t1 = (newPoint.x - p1.x) / direction.x;

    if (fabs(direction.x) < 1e-6f)
        t1 = (newPoint.y - p1.y) / direction.y;

    p1 = newPoint - direction * 2.0f;
    p2 = newPoint + direction * 2.0f;
}


void Line::draw() const {
    Geometry<vec3> geom;
    geom.Vtx() = {p1, p2};
    geom.updateGPU();
    glLineWidth(3.0f);
    geom.Draw(new GPUProgram(), GL_LINES, vec3(0, 1, 1));
}


void Line::printEquations() const {
    std::cout << "Implicit: " << A << "x + " << B << "y = " << C << "\n";
    std::cout << "Parametric: (x, y) = (" << p1.x << ", " << p1.y << ") + t * ("
            << (p2.x - p1.x) << ", " << (p2.y - p1.y) << ")\n";
}
