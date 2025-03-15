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
    // Update C to shift the line while keeping its direction
    C = A * newPoint.x + B * newPoint.y;
    vec3 direction = p2 - p1;
    // Ensure direction is non-zero to avoid division by zero
    float t = (fabs(direction.x) > 1e-6f) ? (newPoint.x - p1.x) / direction.x : (newPoint.y - p1.y) / direction.y;
    // Update endpoints to span the viewport
    p1 = newPoint - direction * 10.0f; // Extend far beyond -1
    p2 = newPoint + direction * 10.0f; // Extend far beyond 1
    std::cout << "Translated to: (" << newPoint.x << ", " << newPoint.y << ")\n";
}


/**
 * @brief Draws the line on the screen.
 *
 * Renders the line using OpenGL by creating a geometry object and setting
 * the appropriate shader program. The line is drawn with a specified width
 * and color.
 */
void Line::draw(GPUProgram* prog) const {
    vec3 direction = p2 - p1;
    std::vector<vec3> endpoints;

    // Parametric form: p(t) = p1 + t * (p2 - p1)
    float t_xmin = (-1.0f - p1.x) / direction.x;
    float t_xmax = (1.0f - p1.x) / direction.x;
    float t_ymin = (-1.0f - p1.y) / direction.y;
    float t_ymax = (1.0f - p1.y) / direction.y;

    if (direction.x != 0) {
        vec3 x_min = p1 + t_xmin * direction;
        vec3 x_max = p1 + t_xmax * direction;
        if (x_min.y >= -1.0f && x_min.y <= 1.0f) endpoints.push_back(x_min);
        if (x_max.y >= -1.0f && x_max.y <= 1.0f) endpoints.push_back(x_max);
    }
    if (direction.y != 0) {
        vec3 y_min = p1 + t_ymin * direction;
        vec3 y_max = p1 + t_ymax * direction;
        if (y_min.x >= -1.0f && y_min.x <= 1.0f) endpoints.push_back(y_min);
        if (y_max.x >= -1.0f && y_max.x <= 1.0f) endpoints.push_back(y_max);
    }

    if (endpoints.size() >= 2) {
        Geometry<vec3> geom;
        geom.Vtx() = {endpoints[0], endpoints[1]};
        geom.updateGPU();
        glLineWidth(3.0f);
        geom.Draw(prog, GL_LINES, vec3(0, 1, 1));
    } else {
        std::cout << "Line not visible: insufficient endpoints\n";
    }
}


/**
 * @brief Prints the implicit and parametric equations of the line.
 *
 * Outputs the implicit equation coefficients (A, B, C) and the parametric
 * representation of the line to the console.
 */
void Line::printEquations() const {
    std::cout   << "Line added \n"
                << "\t Implicit: " << A << " x + " << B << " y = " << C << "\n"
                << "\t Parametric: r(t) = (" << p1.x << ", " << p1.y << ") + "
                << "(" << p2.x - p1.x << ", " << p2.y - p1.y << ")t \n";
}
