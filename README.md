# LinAim

## About the Project

LinAim is a native Linux Aim Trainer built from scratch using C++ and OpenGL. Inspired by popular aim trainers like Aim Lab and competitive shooters like Valorant, this project aims to provide a lightweight, high-performance training environment specifically optimized for Linux users.

The project implements a **hierarchical state machine architecture** using class inheritance to manage different application screens (main menu, settings, FPS game). It features 3D rendering with skybox, spherical targets for aim training, and persistent JSON configurations.

### Main Features

- **Hierarchical State Machine** - Clean state management using OOP principles
- Interactive start menu with state-based navigation
- FPS training mode with 3D targets (spheres)
- Skybox cubemap for immersive environment
- Target positioning grid with slot-based spawning
- Bullet/projectile system
- Sensitivity and resolution settings
- Persistent settings in JSON file
- Graphical interface with ImGui

---

## Installation and Usage

### Prerequisites

Make sure you have the following dependencies installed on your system:

- **C++20 Compiler** (GCC 10+ or Clang 10+)
- **CMake** (version 3.10 or higher)
- **GLFW3** (version 3.3 or higher)
- **GLM** (OpenGL mathematics library)
- **OpenGL** (version 3.3 or higher)

#### Installing dependencies (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libglm-dev
```

#### Installing dependencies (Arch Linux)

```bash
sudo pacman -S base-devel cmake glfw-x11 glm
```

#### Installing dependencies (Fedora)

```bash
sudo dnf install gcc-c++ cmake glfw-devel glm-devel
```

### Building

1. Clone the repository:
```bash
git clone https://github.com/your-username/LinAim.git
cd LinAim
```

2. Create the build directory and compile:
```bash
mkdir -p build
cd build
cmake ..
make
```

3. Run the program:
```bash
./LinAim
```

### Usage

- Upon starting, you will see the main menu
- Use the mouse to navigate through menus
- Access settings to adjust sensitivity and resolution
- Start training mode to practice your aim
- Press ESC to pause or return to menu

---

## Project Structure

```
LinAim/
|-- CMakeLists.txt          # CMake configuration
|-- Makefile                # Alternative Makefile
|-- README.md               # Project documentation
|-- suppr.txt               # Suppression file (sanitizers)
|
|-- assets/                 # Game resources
|   |-- data/
|   |   |-- config.json     # User settings (sensitivity, resolution)
|   |-- fonts/              # Fonts used in the interface
|   |-- shaders/            # GLSL shaders
|   |   |-- ballShader.vs/fs    # Target (sphere) shader
|   |   |-- crosshair.vs/fs     # Crosshair shader
|   |   |-- gridShader.vs/fs    # Grid shader
|   |   |-- skyBox.vs/fs        # Skybox shader
|   |-- textures/
|       |-- skyCubeMap/     # Cubemap textures (px, nx, py, ny, pz, nz)
|
|-- include/                # Project headers
|   |-- core/               # Main classes
|   |   |-- camera.hpp      # FPS camera system
|   |   |-- configUI.hpp    # Configuration and UI structures
|   |   |-- game.hpp        # Master game class
|   |   |-- player.hpp      # Player class
|   |   |-- shader.hpp      # Shader management class
|   |-- entities/           # Game entities
|   |   |-- ball.hpp        # Spherical targets
|   |   |-- bullet.hpp      # Projectiles
|   |   |-- crosshair.hpp   # Crosshair
|   |   |-- grid.hpp        # Positioning grid
|   |   |-- skyBox.hpp      # Skybox cubemap
|   |-- states/             # Application states
|       |-- state.hpp       # Base state class
|       |-- stateDefinitions.hpp  # State definitions
|       |-- stateFps.hpp    # FPS training mode state
|       |-- stateSettings.hpp     # Settings state
|       |-- stateStartMenu.hpp    # Start menu state
|
|-- src/                    # Source code
|   |-- main.cpp            # Entry point
|   |-- glad.c              # GLAD implementation
|   |-- core/               # Core class implementations
|   |-- entities/           # Entity implementations
|   |-- states/             # State implementations
|
|-- vendor/                 # Third-party libraries
|   |-- glad/               # OpenGL loader
|   |-- imgui/              # Immediate mode GUI
|   |-- json/               # JSON parser (nlohmann)
|   |-- KHR/                # Khronos headers
|   |-- stb_image/          # Image loading
|
|-- build/                  # Build directory (generated)
```

### Module Descriptions

**Core:**
- `Game` - Master class that manages window, states, and settings
- `Camera` - First-person camera system with rotation control
- `Player` - Player control, movement, and interaction
- `Shader` - GLSL shader compilation and management

**Entities:**
- `Ball` - spherical targets
- `Bullet` - Projectile system
- `Crosshair` - Customizable crosshair
- `Grid` - Slot grid for target positioning
- `SkyBox` - Skybox cubemap rendering

**States:**
- `StateFps` - Main training mode logic
- `StateSettings` - Settings menu
- `StateStartMenu` - Start menu

---

## Technologies Used

| Technology | Version | Description |
|------------|---------|-------------|
| **C++** | C++20 | Main programming language |
| **OpenGL** | 3.3+ | Graphics API for rendering |
| **GLFW** | 3.3+ | Window and input management |
| **GLM** | - | Math library (vectors, matrices, transformations) |
| **CMake** | 3.10+ | Cross-platform build system |
| **GLAD** | - | Generated OpenGL loader for loading OpenGL functions |
| **Dear ImGui** | - | Immediate mode GUI library |
| **nlohmann/json** | - | Header-only JSON parser for C++ |
| **stb_image** | - | Header-only image loading library |

---

## Credits

### Third-party Libraries (vendor/)

- **GLAD** - OpenGL Loader Generator
  - Website: https://glad.dav1d.de/
  - License: MIT / Public Domain

- **Dear ImGui** - Immediate Mode Graphical User Interface
  - Author: Omar Cornut
  - Repository: https://github.com/ocornut/imgui
  - License: MIT

- **nlohmann/json** - JSON for Modern C++
  - Author: Niels Lohmann
  - Repository: https://github.com/nlohmann/json
  - License: MIT

- **stb_image** - Image loading library
  - Author: Sean Barrett
  - Repository: https://github.com/nothings/stb
  - License: MIT / Public Domain

- **KHR** - Khronos Platform Headers
  - Website: https://www.khronos.org/
  - License: MIT

### External Dependencies

- **GLFW** - Window and input library
  - Website: https://www.glfw.org/
  - License: zlib/libpng

- **GLM** - OpenGL Mathematics
  - Repository: https://github.com/g-truc/glm
  - License: MIT

### Assets

- **Skybox Cubemap** - Environment textures (skyCubeMap/)
  - Skybox textures are located in `assets/textures/skyCubeMap/`