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
Important:
- Fix delta thing on drag by looking at pollEvent (probably redundant after multithreading)
- Set all input variables to false when focus is lost
- ~~Separate view events and particle events~~ Create callback buffer for particle events
- Create separate settings class that gets updated during events, later when synced particle simulator will copy those over

- Multithreading
- UI

Less optimizations:
- Optimizing calculations (using DistanceSquared, pointers)
- Quadtree
- Hashing of curves
- Particle simulation distance hashing
