# Snake Game

A simple Snake game built using **C++**, **CMake**, and **SFML 2.6**. This game is a classic implementation of the Snake game, where the player controls a snake to collect food while avoiding obstacles and the snake's own tail. The game includes various controls for movement and interaction.

## Table of Contents
- [Features](#features)
- [Controls](#controls)
- [Requirements](#requirements)
- [Build Instructions](#build-instructions)
- [How to Run](#how-to-run)
- [Contributing](#contributing)
- [License](#license)

## Features
- **Classic Snake Game**: Grow the snake by eating food, avoid collisions with walls or the snake's body.
- **Customizable Controls**: Modify the controls using the `snake.cpp`.
- **Cross-Platform**: Can be built and run on multiple platforms using **CMake** and **SFML**.

## Controls

The game uses simple keyboard controls:

| Control  | Action           |
|----------|------------------|
| Arrow Up | Move Up          |
| Arrow Down | Move Down      |
| Arrow Left | Move Left      |
| Arrow Right | Move Right    |

You can customize the controls in `main`.

## Requirements

- **SFML 2.6**: Ensure you have SFML 2.6 installed on your system.
- **CMake**: CMake is used to build the project.
- **C++ Compiler**: A C++14 or greater compatible compiler (such as g++, clang++, or MSVC).

## Build Instructions

1. **Clone the Repository**:
    ```bash
    git clone https://github.com/AshiShukla/snakeGame.git
    cd snakeGame
    ```

2. **Install SFML**:
    - **Windows**: Download and install SFML 2.6 from the [official website](https://www.sfml-dev.org/download.php).
    - **Linux/macOS**: You can install SFML using package managers like `apt`, `brew`, or `dnf`. For example, on Ubuntu:
      ```bash
      sudo apt-get install libsfml-dev
      ```

3. **Configure with CMake**:
    ```bash
    mkdir build
    cd build
    cmake ../
    ```

    Make sure to set the path to SFML during the configuration if it's not found automatically:
    ```bash
    cmake .. -DSFML_DIR=/path/to/SFML
    ```

5. **Build the Project**:
    ```bash
    cmake --build .
    ```

    This will generate the executable for the Snake game.

## How to Run

After the build completes, you can run the game with:

```bash
./SnakeGame   # On Linux/macOS
SnakeGame.exe # On Windows
```
## Author
**Ashish Shukla**
