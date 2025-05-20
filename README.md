# Particle Life Simulation

## Description
"Particle Life" was originally created by Jeffrey Ventrella (https://www.ventrella.com/Clusters/).
It is a simulation designed to generate emergent life-like behavior from particles following simple rules.
Since then, many similar applications and websites have been developed based on the same concept.

This project takes that idea and implements it in C++17 with use of [SFML 3.0](https://github.com/SFML/SFML).
The initial version was created as part of a course project to learn C++, 
and it was later fully rewritten for better optimization and additional improvements. 
All the code was written within one month. 
Can handle 100000+ particles under the right conditions. (CPU heavy)


![Cool image](https://i.imgur.com/4OmETSp.png)

## How to run (Windows)
There is already self-contained version that can be downloaded by clicking [here](https://github.com/GrmSeven/CPPParticleSimulator/blob/main/build/ParticleLifeSim.zip?raw=true) 
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

[Showcase](https://www.youtube.com/watch?v=YAzJucB3fZk&list=PLLKy2lT7F2I57ymEgP1YvlNCcKQiS2Wn1)


[![Showcase](https://i.imgur.com/OLqviS5.png)](https://www.youtube.com/watch?v=YAzJucB3fZk&list=PLLKy2lT7F2I57ymEgP1YvlNCcKQiS2Wn1)

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
| Ctrl | Show particle brush/drag radius |
| Left click | Drag / Attract particles |
| Right click | Draw / Delete particles |

UI:
| Key | Action |
| --- | --- |
| Left Click/Scroll up | Next value / Increase value |
| Right Click/Scroll down | Previous value / Decrease value |
| Left Click | Activate button / Toggle checkbox |
| Middle Click | Reset to default value |
| Hold Shift | Amplifies every action 10x while held |

Simulation:
| Key | Action |
| --- | --- |
| Space | Pause simulation |
| Q and N | Change particle count |
| W and S | Change particle type count |
| E and D | Choose matrix preset |
| R | Apply matrix preset |
| Hold Shift | Amplifies every action 10x while held |

Other:
| Key | Action |
| --- | --- |
| F11 | Toggle Fullscreen toggle |
| F1 | Hold to hide UI |
| Alt | Hold to show spacial partitioning grid |


## Links
Clusters: https://www.ventrella.com/Clusters/

SFML: https://github.com/SFML/SFML

Color palette: https://iquilezles.org/articles/palettes/

Original inspiration: https://www.youtube.com/watch?v=4vk7YvBYpOs

Formulas used for attraction: https://www.desmos.com/calculator/uimqle5zvt

