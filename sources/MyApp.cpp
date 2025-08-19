

#include "LineCollection.h"
#include "PointCollection.h"


/**
 * @class MyApp
 * @brief An OpenGL application for drawing points and lines.
 *
 * This class extends the glApp class to create a graphical application that
 * allows users to add points, draw lines, move lines, and find intersections
 * between lines. The application supports different modes of operation, which
 * can be changed using keyboard inputs.
 */
class MyApp final : public glApp {

    char mode = 'p'; // 'p' = point, 'l' = line, 'm' = move, 'i' = intersection

    PointCollection points;
    LineCollection lines;
    GPUProgram* shaderProg = nullptr;

    vec3 firstPoint;
    bool firstSelected = false;
    Line* selectedLine = nullptr;
    vec3 firstIntersectionPoint;
    bool firstLineSelected = false;

    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        uniform vec3 color;
        out vec3 fragColor;
        void main() {
            gl_Position = vec4(aPos, 1.0);
            fragColor = color;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 fragColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(fragColor, 1.0);
        }
    )";


  public:
    MyApp() : glApp(4, 5, 600, 600, "Grafika") {}


    /**
     * @brief Initializes the OpenGL application.
     *
     * This method is overridden to set up the initial OpenGL state and
     * resources. It enables point smoothing for better visual rendering of
     * points and initializes the shader program with predefined vertex and
     * fragment shader source codes.
     */
    void onInitialization() override {
        glEnable(GL_POINT_SMOOTH);
        shaderProg = new GPUProgram(vertexShaderSource, fragmentShaderSource);
    }


    /**
     * Renders the current frame by clearing the framebuffer and drawing
     * elements.
     *
     * This function overrides the `onDisplay` method from the base class. It
     * sets a background color using `glClearColor` with a gray tone and clears
     * the screen via `glClear`. Then, it invokes the `draw` method on both
     * `lines` and `points` objects to render all stored line and point
     * elements, respectively.
     */
    void onDisplay() override {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProg->Use();
        lines.draw(shaderProg);
        points.draw(shaderProg);
    }


    /**
     * Handles keyboard input to modify application mode and reset selection
     * states.
     *
     * This function overrides the `onKeyboard` method from the base class.
     * Depending on the key pressed ('p', 'l', 'm', or 'i'), it changes the
     * drawing mode in the application. It also resets flags related to point
     * and line selections, and clears any currently selected line.
     *
     * @param key The key that was pressed. Only 'p', 'l', 'm', and 'i' are
     * handled to change the mode. Other keys have no effect.
     */
    void onKeyboard(const int key) override {
        if (key == 'p' || key == 'l' || key == 'm' || key == 'i') {
            mode = static_cast<char>(key);
            firstSelected = false;
            firstLineSelected = false;
            selectedLine = nullptr;
            printf("Mode: %c\n", mode);
        }
    }


    /**
     * Handles mouse press events to add points, create lines, move lines, or
     * find intersections based on the current mode.
     *
     * This function overrides the `onMousePressed` method from the base class.
     * Depending on the current mode ('p', 'l', 'm', or 'i'), it performs the
     * following actions:
     * - 'p': Adds a new point at the pressed location transformed to normalized
     *   device coordinates.
     * - 'l': Selects two points to create a line between them. On the first
     * press, the nearest point is selected. On the second press, the nearest
     * point is selected again, and a line is created between the two points.
     * - 'm': Selects the nearest line to allow moving it.
     * - 'i': Selects two lines to calculate their intersection point, if it
     * exists, and adds the intersection point to the point collection.
     *
     * The function also ensures the screen is refreshed after each action.
     *
     * @param button The mouse button pressed (e.g., left, middle, or right).
     * @param pX The x-coordinate of the mouse press in pixel space.
     * @param pY The y-coordinate of the mouse press in pixel space.
     */
    void onMousePressed(MouseButton button, const int pX,
                        const int pY) override {
        const vec3 normalizedPoint = calculateNormalizedPoint(pX, pY);

        switch (mode) {
            case 'p':
                handlePointMode(normalizedPoint);
                break;
            case 'l':
                handleLineMode(normalizedPoint);
                break;
            case 'm':
                handleMoveMode(normalizedPoint);
                break;
            case 'i':
                handleIntersectionMode(normalizedPoint);
                break;
            default:
                printf("Unknown mode: %c\n", mode);
                break;
        }

        refreshScreen();
    }


    /**
     * Calculates a normalized 3D point based on pixel coordinates.
     *
     * This function computes the normalized coordinates in the range of [-1, 1]
     * for a given pixel coordinate within a 600-pixel-wide frame. The
     * Z-coordinate of the resulting 3D point is set to 1.0.
     *
     * @param pX The X-coordinate in pixel space.
     * @param pY The Y-coordinate in pixel space.
     * @return A 3D vector (vec3) representing the normalized point.
     */
    static vec3 calculateNormalizedPoint(const int pX, const int pY) {
        const float normalizedX = 2.0f * static_cast<float>(pX) / 600.0f - 1.0f;
        const float normalizedY = 1.0f - 2.0f * static_cast<float>(pY) / 600.0f;
        return {normalizedX, normalizedY, 1.0f};
    }


    /**
     * Handles and adds a point to the collection in point mode.
     *
     * This function takes a `vec3` object representing a point
     * and adds it to the internal points collection for rendering or
     * processing.
     *
     * @param point A `vec3` object representing the point coordinates to be
     * added.
     */
    void handlePointMode(const vec3& point) { points.addPoint(point); }


    /**
     * Handles the line creation process in a two-step selection mode.
     *
     * This function manages the selection of two points to define a line. On
     * the first call, it selects and stores the nearest point to the given
     * input. On the subsequent call, it selects another nearest point, then
     * defines and adds a line between these two points. The process resets
     * after each line is created.
     *
     * @param point The input point used to find the nearest point for line
     * creation.
     */
    void handleLineMode(const vec3& point) {
        if (!firstSelected) {
            firstPoint = points.findNearestPoint(point);
            firstSelected = true;
        } else {
            const vec3 secondPoint = points.findNearestPoint(point);
            lines.addLine(firstPoint, secondPoint);
            firstSelected = false;
        }
    }


    /**
     * Handles move mode by selecting the nearest line to a given point.
     *
     * This function checks whether a line is currently selected. If no line is
     * selected, it finds and assigns the nearest line to the provided point
     * from the collection of lines.
     *
     * @param point The 3D point used to determine the nearest line in the
     * context of move mode.
     */
    void handleMoveMode(const vec3& point) {
        if (!selectedLine)
            selectedLine = lines.findNearestLine(point);
    }


    /**
     * Handles the intersection selection mode and computes intersection points
     * between selected lines.
     *
     * This function allows a user to successively select lines to compute their
     * intersection point. The first invocation selects the nearest line to the
     * specified point. The second invocation selects another line and computes
     * the intersection point between the two selected lines. If the
     * intersection point is valid, it is added to the points container.
     *
     * @param point The 3D point used to determine the nearest line during the
     * selection phase.
     */
    void handleIntersectionMode(const vec3& point) {
        if (!firstLineSelected) {
            selectedLine = lines.findNearestLine(point);
            if (selectedLine) {
                firstIntersectionPoint = point;
                firstLineSelected = true;
            }
        } else {
            if (const Line* secondLine = lines.findNearestLine(point);
                secondLine && secondLine != selectedLine) {
                const vec3 intersection =
                    selectedLine->computeIntersection(*secondLine);
                if (intersection != vec3(0, 0, 0))
                    points.addPoint(intersection);
            }
            firstLineSelected = false;
            selectedLine = nullptr;
        }
    }


    /**
     * @brief Handles mouse motion events and updates the position of the
     * selected line accordingly.
     *
     * This method is called when the mouse is moved. If the application mode is
     * set to 'm' (move mode) and a line is selected, it calculates the new
     * normalized cursor position based on the provided pixel coordinates and
     * translates the selected line's position. The screen is refreshed to
     * display the updated line position.
     *
     * @param px The x-coordinate of the mouse cursor in pixels.
     * @param py The y-coordinate of the mouse cursor in pixels.
     */
    void onMouseMotion(const int px, const int py) override {
        if (mode == 'm' && selectedLine) {
            const vec3 newCursorPos = calculateNormalizedPoint(px, py);
            selectedLine->translate(newCursorPos);
            refreshScreen();
        }
    }


    /**
     * Releases the selected line when the mouse button is released in "move"
     * mode.
     *
     * This function overrides the `onMouseReleased` method from the base class.
     * It ensures that when the application is in "move" mode ('m') and the
     * mouse button is released, the currently selected line is deselected by
     * setting the `selectedLine` pointer to null.
     *
     * @param button The mouse button that was released (e.g., left, middle, or
     * right).
     * @param px The x-coordinate of the mouse release position in pixel space.
     * @param py The y-coordinate of the mouse release position in pixel space.
     */
    void onMouseReleased(MouseButton button, int px, int py) override {
        if (mode == 'm')
            selectedLine = nullptr;
    }


    ~MyApp() override { delete shaderProg; }

} app;
