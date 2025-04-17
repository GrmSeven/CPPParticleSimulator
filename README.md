# Particle Simulator using SFML

### TODO:
Important optimizations:
- Spatial Partitioning O(n^2) => O(n)
- Multithreading
- Using Struct-of-Arrays instead of AoS (replace particle class with long arrays of values)

Unimportant optimizations:
- Fixed Timestep Physics (?, maybe not needed, it kinda already does it)
- Optimizing calculations (using DistanceSquared, pointers, circle drawing)
- Delta time cap
- Rendering to texture buffer using shader or 8 sided polygons
- (Hard, unnecessary) could use glsl compute shaders to make it all faster

Features:
- Draggable particles with mouse pointer
- Zoomable and draggable canvas
- When using particle clamp(), forces should apply recursively on opposite sides of screen (if particles are on edge, they interact with particles that are on the other side of the screen, as if there is a portal)
- Support multiple types of particles (store their data, matrix of particle to particle attraction, color)
- Different physics options (sin?, newton)


Other:
- Separate renderer.cpp into renderer.cpp and particle_simulator.cpp
- Rewrite everything to be more OOP, use getters and setters
- UI equivalent in console
- UI (later)