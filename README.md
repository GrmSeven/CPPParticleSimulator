# Particle Simulator using SFML

### Controls:
Space - Pause\
Middle Mouse Button - Drag the screen\
Scroll wheel - Zoom in/out\
Arrow keys - Move around\
Right click - Spawn single random particle at cursor\
Left click - Drag/Attract particles\
Q - Spawn 100 random particles at cursor\
W - Spawn 100 random particles random positions\
S - Despawn 100 random particles\
E - Increase particle type count\
D- Decrease particle type count\
R - Randomize type interaction matrix

### TODO:
Important optimizations:
- Multithreading
- Fix delta thing on drag by looking at pollEvent or adding multithreading
- Set all input variables to false when focus is lost

UI:
- Max Velocity
- Velocity check (sets velocity to zero AFTER it gets applied)
- Max radius, Min radius

Less optimizations:
- Optimizing calculations (using DistanceSquared, pointers)
- Quadtree
- Hashing of curves
- Particle simulation distance hashing
