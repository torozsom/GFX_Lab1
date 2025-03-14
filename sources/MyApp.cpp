#include <iostream>
#include "PointCollection.h"
#include "LineCollection.h"



class MyApp : public glApp {

private:
    char mode = 'p'; // 'p' = point, 'l' = line, 'm' = move, 'i' = intersection
    PointCollection points;
    LineCollection lines;

    vec3 firstPoint;
    bool firstSelected = false;
    Line *selectedLine = nullptr;

    vec3 firstIntersectionPoint;
    bool firstLineSelected = false;

public:
    MyApp() : glApp(4, 5, 600, 600, "Grafika")
    { }


    void onInitialization() override {
        glEnable(GL_POINT_SMOOTH);
    }


    void onDisplay() override {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        lines.draw();
        points.draw();
    }


    void onKeyboard(const int key) override {
        if (key == 'p' || key == 'l' || key == 'm' || key == 'i') {
            mode = key;
            firstSelected = false;
            firstLineSelected = false;
            selectedLine = nullptr;
            std::cout << "Mode: " << mode << "\n";
        }
    }


    void onMousePressed(MouseButton button, const int px, const int py) override {
        vec3 p = vec3(2.0f * px / 600.0f - 1.0f, 1.0f - 2.0f * py / 600.0f, 1);

        if (mode == 'p') {
            points.addPoint(p);
        } else if (mode == 'l') {
            if (!firstSelected) {
                firstPoint = points.findNearestPoint(p);
                firstSelected = true;
            } else {
                vec3 secondPoint = points.findNearestPoint(p);
                lines.addLine(firstPoint, secondPoint);
                firstSelected = false;
            }
        } else if (mode == 'm') {
            if (!selectedLine) {
                selectedLine = lines.findNearestLine(p);
            }
        } else if (mode == 'i') {
            if (!firstLineSelected) {
                selectedLine = lines.findNearestLine(p);
                if (selectedLine) {
                    firstIntersectionPoint = p; // Store for reference
                    firstLineSelected = true;
                }
            } else {
                Line *secondLine = lines.findNearestLine(p);
                if (secondLine && secondLine != selectedLine) {
                    vec3 intersection = selectedLine->computeIntersection(*secondLine);
                    if (intersection != vec3(0, 0, 0)) {
                        points.addPoint(intersection);
                    }
                }
                firstLineSelected = false;
                selectedLine = nullptr;
            }
        }
        refreshScreen();
    }


    void onMouseMotion(const int px, const int py) override {
        if (mode == 'm' && selectedLine) {
            vec3 p = vec3(2.0f * px / 600.0f - 1.0f, 1.0f - 2.0f * py / 600.0f, 1);
            selectedLine->translate(p);
            refreshScreen();
        }
    }


    void onMouseReleased(MouseButton button, int px, int py) override {
        if (mode == 'm')
            selectedLine = nullptr;
    }


} app;
