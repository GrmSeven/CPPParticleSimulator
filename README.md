# Particle Simulator using SFML

### TODO:
Bugs:
- When warpping is enabled, sometimes particles on the far right on pre-previous cell, dont react to particles that are in the last cell (still on the right side)

Important optimizations:
- Multithreading

Unimportant optimizations:
- Optimizing calculations (using DistanceSquared, pointers)
- Quadtree
- Hashing of curves

Features:
- Draggable particles with mouse pointer
- Zoomable and draggable canvas
- Support multiple types of particles (store their data, matrix of particle to particle attraction, color)
- Different physics options (sin?, newton)


Other:
- Separate renderer.cpp into renderer.cpp and particle_simulator.cpp
- Rewrite everything to be more OOP, use getters and setters
- UI equivalent in console
- UI (later)