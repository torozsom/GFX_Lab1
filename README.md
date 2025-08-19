# Interactive 2D Points and Lines

This project is an interactive OpenGL application that allows users to create, manipulate, and visualize points and
lines in a 2D space. You can add points, draw lines between them, move lines, and find intersection points—all rendered
in real-time using OpenGL. This README provides the necessary background on OpenGL and physics concepts used in the
project, explains each class's role, and describes how shaders bring the visuals to life.

## Table of Contents

- [Background Knowledge](#background-knowledge)
    - [Coordinate Systems and Vectors](#coordinate-systems-and-vectors)
    - [Lines in 2D Space](#lines-in-2d-space)
    - [Distance from a Point to a Line](#distance-from-a-point-to-a-line)
    - [Line Intersection](#line-intersection)
    - [OpenGL Basics](#opengl-basics)
- [Project Structure and Classes](#project-structure-and-classes)
    - [Line](#line)
    - [LineCollection](#linecollection)
    - [PointCollection](#pointcollection)
    - [MyApp](#myapp)
    - [GPUProgram](#gpuprogram)
    - [Geometry](#geometry)
    - [Texture](#texture)
- [Shaders](#shaders)
    - [Vertex Shader](#vertex-shader)
    - [Fragment Shader](#fragment-shader)
- [How to Use the Project](#how-to-use-the-project)
- [Getting Started](#getting-started)

---

## Background Knowledge

To understand this project, you need some basics in physics (geometry) and computer graphics (OpenGL). Let’s break it
down.

### Coordinate Systems and Vectors

In computer graphics, we work with **coordinate systems** to position objects. This project uses a 2D **Cartesian
coordinate system**, where points are defined by `x` (horizontal) and `y` (vertical) values. OpenGL uses **Normalized
Device Coordinates (NDC)**, a space where the visible area ranges from `(-1, -1)` (bottom-left) to `(1, 1)` (top-right).

- **Vectors**: A vector is like an arrow with direction and length. Here, points are stored as `vec3` (3D vectors) with
  `x`, `y`, and a `z` value set to 1.0 for consistency. We use vectors for:
    - **Position**: Where a point is.
    - **Direction**: Which way a line goes.
    - Key operations:
        - **Addition/Subtraction**: Move points or find differences.
        - **Normalization**: Make a vector’s length 1, useful for direction.

### Lines in 2D Space

Lines are central to this project. They’re defined in two ways:

1. **Parametric Form**:  $$\mathbf{r}(t) = \mathbf{p} + t \mathbf{d}$$
    - $\mathbf{p}$: A point on the line (e.g., `p1`).
    - $\mathbf{d}$: Direction vector (e.g., `p2 - p1`).
    - $t$: A parameter that moves along the line.
    - Used for rendering and finding intersections.


2. **Implicit Form**:  $$Ax + By = C$$
    - $A$, $B$, $C$: Constants derived from two points.
    - Used to check if a point is on the line.

### Distance from a Point to a Line

To see if a point is “on” a line, we calculate its perpendicular distance to the line in implicit form ($Ax + By = C$):

$$
\text{Distance} = \frac{|A p_x + B p_y - C|}{\sqrt{A^2 + B^2}}
$$

If this distance is tiny (e.g., < 0.01), the point is considered on the line. This helps us select lines or check
containment.

### Line Intersection

To find where two lines cross, we solve their implicit equations:

- Line 1: $A_1 x + B_1 y = C_1$
- Line 2: $A_2 x + B_2 y = C_2$

Intersection point \((x, y)\):

$$
x = \frac{B_1 C_2 - B_2 C_1}{A_1 B_2 - A_2 B_1}, \quad y = \frac{A_2 C_1 - A_1 C_2}{A_1 B_2 - A_2 B_1}
$$

If the denominator ($A_1 B_2 - A_2 B_1$) is near zero, the lines are parallel and don’t intersect (we return
`(0, 0, 0)`).

### OpenGL Basics

**OpenGL** is a tool to draw graphics using your computer’s GPU (graphics processing unit). Here’s what you need to
know:

- **Vertices**: Points in space (e.g., a point or line endpoint).
- **Primitives**: Shapes OpenGL can draw, like `GL_POINTS` (dots) or `GL_LINES` (line segments).
- **Buffers**: GPU memory to store vertex data.
- **Shaders**: Mini-programs that run on the GPU:
    - **Vertex Shader**: Positions vertices.
    - **Fragment Shader**: Colors pixels.
- **Uniforms**: Values sent from your code to shaders (e.g., color).

This project uses OpenGL to render points and lines in a window, making them interactive.

---

## Project Structure and Classes

The codebase is split into classes, each with a specific job. Here’s why they’re needed and how they work.

### Line

- **Why It’s Needed**: Represents a single 2D line, handling its math and drawing.
- **How It Works**:
    - Stores two points (`p1`, `p2`) and computes implicit coefficients (`A`, `B`, `C`).
    - **contains(vec3 p)**: Checks if a point is on the line using the distance formula.
    - **computeIntersection(Line& other)**: Finds the crossing point with another line.
    - **translate(vec3 newPoint)**: Moves the line to pass through a new point, keeping its direction.
    - **draw(GPUProgram* prog)**: Clips the line to the NDC square (`[-1, 1]`) and renders it in cyan.

### LineCollection

- **Why It’s Needed**: Manages multiple lines, making it easy to add or find them.
- **How It Works**:
    - Stores lines in a `vector`.
    - **addLine(vec3 p1, vec3 p2)**: Creates and adds a new `Line`.
    - **findNearestLine(vec3 p)**: Returns the closest line to a point (or `nullptr` if none).
    - **draw(GPUProgram* prog)**: Draws all lines.

### PointCollection

- **Why It’s Needed**: Handles multiple points for creating lines or showing intersections.
- **How It Works**:
    - Stores points in a `vector`.
    - **addPoint(vec3 p)**: Adds a point and logs it.
    - **findNearestPoint(vec3 p)**: Finds the closest point to a given location.
    - **draw(GPUProgram* prog)**: Renders all points as red dots.

### MyApp

- **Why It’s Needed**: The main class that runs the app and handles user input.
- **How It Works**:
    - Extends `glApp` to manage modes (`p` for points, `l` for lines, `m` for move, `i` for intersections).
    - **onInitialization()**: Sets up OpenGL (e.g., smooth points) and shaders.
    - **onDisplay()**: Clears the screen and draws points/lines.
    - **onKeyboard(int key)**: Switches modes via keys.
    - **onMousePressed()**: Adds points, creates lines, selects lines, or finds intersections based on mode.

### GPUProgram

- **Why It’s Needed**: Manages shaders, linking your code to the GPU.
- **How It Works**:
    - Compiles and links vertex/fragment shaders.
    - **setUniform()**: Sends data (e.g., color) to shaders.
    - Used by `Geometry` and `Line`/`PointCollection` to render.

### Geometry

- **Why It’s Needed**: Prepares vertex data for the GPU and draws it.
- **How It Works**:
    - Stores vertices in a CPU `vector` and GPU buffers (VAO/VBO).
    - **updateGPU()**: Sends vertex data to the GPU.
    - **Draw(GPUProgram* prog, int type, vec3 color)**: Renders points (`GL_POINTS`) or lines (`GL_LINES`).

### Texture

- **Why It’s Needed**: Supports textures (not used here but included for future expansion).
- **How It Works**:
    - Loads or generates texture data and binds it to OpenGL.
    - Could add visual effects to points/lines later.

---

## Shaders

Shaders are GPU programs that control rendering. This project uses two simple ones.

### Vertex Shader

- **What It Does**: Positions vertices and assigns them a color.
- **Code**:
  ```glsl
  #version 330 core
  layout(location = 0) in vec3 aPos;
  uniform vec3 color;
  out vec3 fragColor;
  void main() {
      gl_Position = vec4(aPos, 1.0);
      fragColor = color;
  }
  ```
- **How It’s Used**: Takes a vertex position (`aPos`) and a color (`color`) from the CPU, sets the position in NDC, and
  passes the color to the fragment shader.

### Fragment Shader

- **What It Does**: Colors each pixel.
- **Code**:
  ```glsl
  #version 330 core
  in vec3 fragColor;
  out vec4 FragColor;
  void main() {
      FragColor = vec4(fragColor, 1.0);
  }
  ```
- **How It’s Used**: Receives the color from the vertex shader and applies it to pixels, with full opacity (`1.0`).

- **Usage in Project**:
    - `MyApp` sets up these shaders.
    - `Geometry` uses them to draw points (red: `vec3(1, 0, 0)`) and lines (cyan: `vec3(0, 1, 1)`).

---

## How to Use the Project

1. **Modes** (switch with keyboard):
    - `p`: Point mode – Click to add red points.
    - `l`: Line mode – Click twice to select two points and draw a cyan line.
    - `m`: Move mode – Click a line, drag to move it, release to drop.
    - `i`: Intersection mode – Click two lines to add their intersection as a point.

2. **Rendering**:
    - Points: Red dots (size 10).
    - Lines: Cyan segments (width 3), clipped to the `[-1, 1]` NDC square.

3. **Interaction**:
    - Mouse clicks and drags control actions based on the mode.
    - The window updates in real-time.

---

## Getting Started

1. **Prerequisites**:
    - C++ compiler (e.g., GCC, MSVC).
    - OpenGL libraries (GLFW, GLAD included in `framework.h`).
    - GLM library for vector math.

2. **Build**:
    - Clone the repo: `git clone https://github.com/torozsom/GFX_Lab1.git`.
    - Compile with your C++ environment (e.g., `g++ *.cpp -lglfw -lGL` on Linux).

3. **Run**:
    - Execute the binary to launch a 600x600 window.

Explore the code, tweak it, and experiment with graphics concepts!

---
