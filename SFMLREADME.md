# Particle Simulator using SFML

### TODO:
Optimisation:
- Spatial Partitioning O(n^2) => O(n)
- Multithreading
- Optimizing calculations (using DistanceSquared, pointers, circle drawing)
- Delta time cap
- Rendering to texture buffer using shader or 8 sided polygons

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