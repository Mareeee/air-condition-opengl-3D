# 3D OpenGL Project - Air Conditioner Simulation

This project is a 3D simulation of an air conditioner unit, implemented using OpenGL (version 3.0+), the programmable pipeline, shaders, models, and the FreeType library for text rendering. The application runs in full-screen mode and includes all required techniques from the assignment.

## Demo
- [Here](https://youtu.be/OM_72xGszmE) you can find a short demo of the working application.

## Technologies Used
- **OpenGL 3.3 Core** (programmable pipeline)
- **GLFW** - window and input handling  
- **GLEW** - OpenGL function loading  
- **FreeType** - text rendering  
- **C++**

## Features

### Controls
- **Left Mouse Click on the lamp** - turns the AC on/off
- **SPACE** - takes/empties/returns the water basin
- **ESC** - exits the application

### Water Basin
- A transparent basin is placed below the AC.
- Water fills the basin gradually every second.
- If the basin becomes full → the AC automatically turns off.
- The AC cannot be turned back on until the basin is emptied (SPACE).

### Rendering & Other Requirements
- Full-screen display
- Frame limiter at **75 FPS**
- Implemented techniques: shaders, uniforms, transparency, textures, models, mouse & keyboard input

## Run Instructions
1. Install dependencies (GLFW, GLEW, Assimp, Assimp.redist, FreeType).
2. Build the project.
3. Run the executable - the application launches in full-screen mode.
