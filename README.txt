Simple 3D Engine: OpenGL + SDL2

This project is a basic graphical application written in C++ using OpenGL 4.1, SDL2 for window and input handling, and the GLM library for 3D mathematical operations. The application renders a rotating, colored square and allows free movement around the scene using a simple first-person (FPS) camera.

🚀 Features
OpenGL Rendering: Uses the modern graphics pipeline (Core Profile) with VAO, VBO, and EBO.
3D Camera: Full handling of view and projection matrices (Model-View-Projection).
Scene Navigation: Movement in 3D space using the keyboard and camera rotation using the mouse (Mouse Look).
External Shaders: Loading and compiling shader code (Vertex & Fragment) directly from external files.
🛠 Requirements and Dependencies

To compile and run the project, you need the following libraries:

C++ Compiler (supporting C++11 or newer)
SDL2 – Window handling, OpenGL context, and input (keyboard/mouse)
GLAD – Loading OpenGL functions and extensions
GLM – Header-only math library for 3D graphics
🎮 Controls

The application captures the mouse cursor immediately after launch, allowing smooth camera control.

Mouse: Look around / Rotate camera (left/right)
Up Arrow: Move forward
Down Arrow: Move backward
Left Arrow: Move left (strafing)
Right Arrow: Move right (strafing)
Closing the window: Terminates the program (Escape is not bound by default; close via the system window controls).
📂 Expected File Structure

Note that the code loads shaders from a specific relative path. To run the program without modifying the code, make sure your project structure looks roughly like this:

├── src/
│   ├── shaders/
│   │   ├── vert.glsl    # Vertex shader
│   │   └── frag.glsl    # Fragment shader
│   ├── main.cpp         # Main file with game loop and rendering
│   ├── CAMERA.HPP       # Camera class header file
│   └── CAMERA.CPP       # Camera class implementation (if separated)
├── build/               # Folder containing the executable (e.g., after using CMake)
