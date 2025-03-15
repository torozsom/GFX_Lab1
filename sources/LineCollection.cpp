#include "LineCollection.h"


/**
 * Adds a new line to the collection using two provided points.
 *
 * @param p1 The starting point of the line.
 * @param p2 The ending point of the line.
 */
void LineCollection::addLine(const vec3 p1, const vec3 p2) {
    lines.push_back(Line(p1, p2));
}


/**
 * Finds the nearest line to the provided point.
 *
 * @param p The point to check against the lines.
 * @return A pointer to the nearest line if found, otherwise nullptr.
 */
Line* LineCollection::findNearestLine(const vec3 p) {
    for (auto& line: lines)
        if (line.contains(p))
            return &line;

    return nullptr;
}


/**
 * Draws all lines in the collection.
 */
void LineCollection::draw(GPUProgram* prog) const {
    for (const auto& line: lines)
        line.draw(prog);
}
