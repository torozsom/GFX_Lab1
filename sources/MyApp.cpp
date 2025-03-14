#include "framework.h"
#include <vector>
#include <iostream>

// Data structures to store points and lines
std::vector<vec3> points;
std::vector<std::pair<vec3, vec3> > lines;

char mode = 'p'; // Current mode: 'p' = point, 'l' = line, 'm' = move, 'i' = intersection

class MyApp : public glApp {
public:
    MyApp() : glApp(4, 5, 900, 600, "Grafika") {
    }

    void onInitialization() override {
        // Nothing specific to initialize yet
    }

    void onDisplay() override {
        glClearColor(0.5f, 0.5f, 0.5f, 1); // Grey background
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw points (red)
        Geometry<vec3> pointGeom;
        pointGeom.Vtx() = points;
        pointGeom.updateGPU();
        pointGeom.Draw(new GPUProgram(), GL_POINTS, vec3(1, 0, 0));

        // Draw lines (cyan)
        Geometry<vec3> lineGeom;
        for (const auto &line: lines) {
            lineGeom.Vtx().push_back(line.first);
            lineGeom.Vtx().push_back(line.second);
        }
        lineGeom.updateGPU();
        lineGeom.Draw(new GPUProgram(), GL_LINES, vec3(0, 1, 1));
    }

    void onKeyboard(int key) override {
        if (key == 'p' || key == 'l' || key == 'm' || key == 'i') {
            mode = key;
        }
    }

    void onMousePressed(MouseButton button, int px, int py) override {
        vec3 normalizedPoint = vec3(2.0f * px / 600.0f - 1.0f, 1.0f - 2.0f * py / 600.0f, 1);

        if (mode == 'p') {
            // Add a new red point
            points.push_back(normalizedPoint);
            std::cout << "Added point: (" << normalizedPoint.x << ", " << normalizedPoint.y << ")\n";
        } else if (mode == 'l') {
            static vec3 firstPoint;
            static bool firstSelected = false;

            if (!firstSelected) {
                firstPoint = normalizedPoint;
                firstSelected = true;
            } else {
                lines.emplace_back(firstPoint, normalizedPoint);
                std::cout << "Added line from (" << firstPoint.x << ", " << firstPoint.y << ") "
                        << "to (" << normalizedPoint.x << ", " << normalizedPoint.y << ")\n";
                firstSelected = false;
            }
        }

        refreshScreen();
    }
} app;
