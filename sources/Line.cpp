#include "Line.h"

/**
 * @brief Constructs a Line object given two points.
 *
 * This constructor initializes a line using two specified points in 2D
 * space and computes the implicit line equation coefficients (A, B, and C)
 * based on these points. The line is represented in both implicit and
 * parametric forms.
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
 * The constructor also outputs the line equations using the printEquations()
 * method.
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
 * Determines if a given point is on the current line by calculating the
 * shortest perpendicular distance from the point to the line and comparing it
 * to a small tolerance threshold.
 *
 * @param p The point to check, represented as a 3D vector.
 *
 * @return True if the point lies on the line within a tolerance of 0.01 units,
 * false otherwise.
 */
bool Line::contains(const vec3 p) const {
    const float distance = fabs(A * p.x + B * p.y - C) / sqrt(A * A + B * B);
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
    const float det = A * other.B - other.A * B;
    if (fabs(det) < 1e-6f)
        return {0, 0, 0};

    const float x = (other.B * C - B * other.C) / det;
    const float y = (A * other.C - other.A * C) / det;
    return {x, y, 1};
}

/**
 * @brief Translates the line to pass through a new point.
 *
 * This method adjusts the position of the line so that it passes through
 * the specified point. The implicit equation coefficient (C) of the line
 * is updated accordingly, and the endpoints of the line (p1 and p2) are
 * recalculated, maintaining its direction and scaling appropriately.
 *
 * @param newPoint The new point through which the line should pass.
 */
void Line::translate(const vec3 newPoint) {
    C = A * newPoint.x + B * newPoint.y;
    const vec3 direction = normalize(p2 - p1);
    p1 = newPoint - direction * 2.0f;
    p2 = newPoint + direction * 2.0f;
}

/**
 * @brief Renders the line segment within the unit square using a GPU program.
 *
 * This method calculates the intersection points of the line with the
 * boundaries of the unit square in normalized device coordinates (NDC: [-1, 1]
 * for both x and y axes). If the line intersects the square, it renders the
 * visible segment using the specified GPU program.
 *
 * @param prog A pointer to the GPUProgram used for rendering the line.
 *
 * The method operates as follows:
 * - Computes the parametric intersection points of the line with the square
 * boundaries.
 * - Determines valid endpoints that lie within the bounds of the square.
 * - If at least two valid endpoints are found, they are sent to the GPU for
 * rendering.
 *
 * The rendered line segment is drawn with a width of 3 and a cyan color (RGB:
 * (0, 1, 1)).
 */
void Line::draw(GPUProgram* prog) const {
    const vec3 direction = p2 - p1;
    std::vector<vec3> endpoints;

    const float t_xmin = (-1.0f - p1.x) / direction.x;
    const float t_xmax = (1.0f - p1.x) / direction.x;
    const float t_ymin = (-1.0f - p1.y) / direction.y;
    const float t_ymax = (1.0f - p1.y) / direction.y;

    if (direction.x != 0) {
        const vec3 x_min = p1 + t_xmin * direction;
        const vec3 x_max = p1 + t_xmax * direction;
        if (x_min.y >= -1.0f && x_min.y <= 1.0f)
            endpoints.push_back(x_min);
        if (x_max.y >= -1.0f && x_max.y <= 1.0f)
            endpoints.push_back(x_max);
    }

    if (direction.y != 0) {
        const vec3 y_min = p1 + t_ymin * direction;
        const vec3 y_max = p1 + t_ymax * direction;
        if (y_min.x >= -1.0f && y_min.x <= 1.0f)
            endpoints.push_back(y_min);
        if (y_max.x >= -1.0f && y_max.x <= 1.0f)
            endpoints.push_back(y_max);
    }

    if (endpoints.size() >= 2) {
        Geometry<vec3> geom;
        geom.Vtx() = {endpoints[0], endpoints[1]};
        geom.updateGPU();
        glLineWidth(3.0f);
        geom.Draw(prog, GL_LINES, vec3(0, 1, 1));
    }
}

/**
 * @brief Prints the implicit and parametric equations of the line.
 *
 * Outputs the implicit equation coefficients (A, B, C) and the parametric
 * representation of the line to the console.
 */
void Line::printEquations() const {
    printf("Line added \n"
           "\t Implicit: %.2f x + %.2f y = %.2f \n"
           "\t Parametric: r(t) = (%.2f, %.2f) + "
           "(%.2f, %.2f)t \n",
           A, B, C, p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
}
