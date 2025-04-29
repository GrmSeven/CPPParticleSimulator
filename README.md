# Particle Simulator using SFML

### TODO:
Bugs:
- When warpping is enabled, sometimes particles on the far right on pre-previous cell, dont react to particles that are in the last cell (still on the right side)

Important optimizations:
- Multithreading

Features:
- Pause
- Draggable particles with mouse pointer
- Draggable canvas
- Support multiple types of particles (store their data, matrix of particle to particle attraction, color)
- Different physics options (sin?, newton)

UI and final features:
- Rewrite everything to be more OOP, use getters and setters
- UI equivalent in console
- UI (later)

Unimportant optimizations:
- Optimizing calculations (using DistanceSquared, pointers)
- Quadtree
- Hashing of curves