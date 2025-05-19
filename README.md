# Particle Life Simulation

## Desctiption
"Particle Life" was originally created by Jeffrey Ventrella (https://www.ventrella.com/Clusters/).
It is a simulation designed to generate emergent life-like behavior from particles following simple rules.
Since then, many similar applications and websites have been developed based on the same concept.

This project takes that idea and implements it in C++17 with use of [SFML 3.0](https://github.com/SFML/SFML).
The initial version was created as part of a course project to learn C++, 
and it was later fully rewritten for better optimization and additional improvements.
Can handle []+ particles under the right conditions. (CPU heavy, tested on -[])

## How to run (Windows)
There is already self-contained version that can be downloaded by clicking [here](https://github.com/GrmSeven/CPPParticleSimulator/blob/development/build/ParticleLifeSim.zip?raw=true) 
or downloading file in `build` folder.
Unzip `ParticlelifeSim.zip` and run `ParticlelifeSim.exe` file.

To compile the project manually, it requires:
- C++ 17 compatible compiler
- CMake version 3.28 or higher https://cmake.org

## Features
- Particle life simulation
- Camera movement, particle dragging, warping and other simulation controls
- Simple custom UI system
- Synchronous Multithreading
- Spatial partitioning optimisation
- Structure of Arrays

## Showcase

## Controls
Camera:
| Key | Action |
| --- | --- |
| Middle mouse button | Drag the screen |
| Scroll wheel  | Zoom in/out |
| Arrow keys | Move around |

Canvas:
| Key | Action |
| --- | --- |
| Shift + Scroll | Resize brush/drag |
| Ctrl | Show particle drag/brush radius |
| Left click | Drag/Attract particles |
| Right click | Draw/delete particles |

UI:
| Key | Action |
| --- | --- |
| Hold Shift | amplifies every action 10x while held |
| Left Click/Scroll up | Next value / increase value |
| Right Click/Scroll down | Previous value / decrease value |
| Left Click | Activate button / Toggle checkbox |
| Middle Click | Reset to default value |

Simulation:
| Key | Action |
| --- | --- |
| Space | Pause simulation |
| Q and N | Change particle count |
| W and S | Change particle type count |
| E and D | Choose matrix preset |
| R | Apply matrix preset |

Other:
| Key | Action |
| --- | --- |
| F11 | Fullscreen toggle |
| F1 | Hold to hide UI |
| Alt | Hold to show spacial partitioning grid |