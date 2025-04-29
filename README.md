# Particle Simulator using SFML

### Controls:
Space - Pause
Middle Mouse Button - Drag the screen
Scroll wheel - Zoom in/out
Arrow keys - Move around
Right click - Spawn single random particle at cursor
Q - Spawn 100 random particles at cursor
W - Spawn 100 random particles random positions
S - Despawn 100 random particles

### TODO:
Bugs:
- Move camera events into camera.cpp from renderer.cpp
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

UI settings:
- Max Velocity
- Velocity check (sets velocity to zero AFTER it gets applied)
- Max radius, Min radius

Unimportant optimizations:
- Optimizing calculations (using DistanceSquared, pointers)
- Quadtree
- Hashing of curves