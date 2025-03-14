#include "LineCollection.h"



void LineCollection::addLine(const vec3 p1, const vec3 p2) {
    lines.push_back(Line(p1, p2));
}


Line* LineCollection::findNearestLine(const vec3 p) {
    for (auto& line: lines)
        if (line.contains(p))
            return &line;

    return nullptr;
}


void LineCollection::draw() const {
    for (const auto& line: lines) {
        line.draw();
    }
}
