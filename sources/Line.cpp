#include "Line.h"
#include <iostream>



/**
 * @brief Constructs a Line object given two points.
 *
 * This constructor initializes a line using two specified points in 2D
 * space and computes the implicit line equation coefficients (A, B, and C)
 * based on these points. The line is represented in both implicit and parametric forms.
 *
 * @param point1 The first point defining the line.
 * @param point2 The second point defining the line.
 *
 * The implicit equation of the line is calculated as:
 * `Ax + By = C`, where:
 * - A = point2.y - point1.y
 * - B = point1.x - point2.x
 * - C = A * point1.x + B * point1.y
 *
 * The constructor also outputs the line equations using the printEquations() method.
 */
Line::Line(const vec3 point1, const vec3 point2) : p1(point1), p2(point2) {
    A = p2.y - p1.y;
    B = p1.x - p2.x;
    C = A * p1.x + B * p1.y;
    printEquations();
}


/**
 * @brief Checks if a point lies on the line.
 *
 * Determines if a given point is on the current line by calculating the shortest
 * perpendicular distance from the point to the line and comparing it to a small
 * tolerance threshold.
 *
 * @param p The point to check, represented as a 3D vector.
 *
 * @return True if the point lies on the line within a tolerance of 0.01 units,
 * false otherwise.
 */
bool Line::contains(const vec3 p) const {
    float distance = fabs(A * p.x + B * p.y - C) / sqrt(A * A + B * B);
    return distance < 0.01f;
}


/**
 * @brief Computes the intersection point of two lines.
 *
 * Calculates the intersection point of the current line with another line
 * using the implicit line equations. If the lines are parallel, it returns
 * a zero vector.
 *
 * @param other The other line to check for intersection.
 *
 * @return The intersection point as a 3D vector. If the lines are parallel,
 * returns (0, 0, 0).
 */
vec3 Line::computeIntersection(const Line& other) const {
    float det = A * other.B - other.A * B;
    if (fabs(det) < 1e-6f)
        return vec3(0, 0, 0);

    float x = (other.B * C - B * other.C) / det;
    float y = (A * other.C - other.A * C) / det;
    return vec3(x, y, 1);
}


/**
 * @brief Translates the line to a new point.
 *
 * Moves the line to a new point while maintaining its direction and length.
 * The new point is used to recalculate the line's implicit equation coefficients.
 *
 * @param newPoint The new point to translate the line to.
 */
void Line::translate(const vec3 newPoint) {
    C = A * newPoint.x + B * newPoint.y;
    vec3 direction = p2 - p1;
    float t1 = (newPoint.x - p1.x) / direction.x;

    if (fabs(direction.x) < 1e-6f)
        t1 = (newPoint.y - p1.y) / direction.y;

    p1 = newPoint - direction * 2.0f;
    p2 = newPoint + direction * 2.0f;
}


/**
 * @brief Draws the line on the screen.
 *
 * Renders the line using OpenGL by creating a geometry object and setting
 * the appropriate shader program. The line is drawn with a specified width
 * and color.
 */
void Line::draw() const {
    Geometry<vec3> geom;
    geom.Vtx() = {p1, p2};
    geom.updateGPU();
    glLineWidth(3.0f);
    geom.Draw(new GPUProgram(), GL_LINES, vec3(0, 1, 1));
}


/**
 * @brief Prints the implicit and parametric equations of the line.
 *
 * Outputs the implicit equation coefficients (A, B, C) and the parametric
 * representation of the line to the console.
 */
void Line::printEquations() const {
    std::cout << "Implicit: " << A << "x + " << B << "y = " << C << "\n";
    std::cout << "Parametric: (x, y) = (" << p1.x << ", " << p1.y << ") + t * ("
            << (p2.x - p1.x) << ", " << (p2.y - p1.y) << ")\n";
}
