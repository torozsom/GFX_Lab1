#ifndef LINECOLLECTION_H
#define LINECOLLECTION_H

#include <vector>
#include "Line.h"

class LineCollection {
private:
    std::vector<Line> lines;

public:
    void addLine(vec3 p1, vec3 p2);

    Line* findNearestLine(vec3 p);

    void draw() const;

    std::vector<Line>& getLines() { return lines; }
};

#endif
