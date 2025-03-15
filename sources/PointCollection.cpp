#include "PointCollection.h"



/**
 * @brief Adds a point to the collection.
 *
 * This method appends a point represented as a vec3 object to the internal
 * collection of points. Additionally, it prints the coordinates of the added
 * point to standard output.
 *
 * @param p The point to be added, represented as a vec3 object.
 */
void PointCollection::addPoint(const vec3 p) {
    points.push_back(p);
    printf("Point added: (%.2f, %.2f)\n", p.x, p.y);
}


/**
 * @brief Finds the nearest point in the collection to a given point.
 *
 * This method iterates through the collection of points and finds the one
 * closest to the specified point p. It returns the closest point as a vec3
 * object.
 *
 * @param p The point to which the nearest point is to be found, represented as a vec3 object.
 * @return The nearest point in the collection, represented as a vec3 object.
 */
vec3 PointCollection::findNearestPoint(const vec3 p) {
    float minDist = 1.0f;
    vec3 closest = vec3(0, 0, 1);
    for (const auto& pt: points) {
        float dist = length(pt - p);
        if (dist < minDist) {
            minDist = dist;
            closest = pt;
        }
    }
    return closest;
}


/**
 * @brief Draws the points in the collection.
 *
 * This method creates a Geometry object, sets its vertex data to the points
 * in the collection, and then draws it using a GPU program. The points are
 * rendered as red dots on the screen.
 */
void PointCollection::draw(GPUProgram* prog) const {
    Geometry<vec3> geom;
    geom.Vtx() = points;
    geom.updateGPU();
    glPointSize(10.0f);
    geom.Draw(prog, GL_POINTS, vec3(1, 0, 0)); // Red
}
