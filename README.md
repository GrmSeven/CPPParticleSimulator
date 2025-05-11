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
Multithreading preparation:
- Create UI that stores data, later particle simulator will sync

Threads:
- Events + Camera updating + UI updating
- Particle rendering + UI Rendering
- Sync from UI -> Particle simulation

Smaller improvements:
- Move particle rendering into particle simulator itself
- Add visible border to the simulator
- Shift + scroll = resize brush
- Middle click on ui - reset to default

Smaller optimizations:
- Use profiler: https://www.jetbrains.com/help/clion/cpu-profiler.html#perf-output
- Optimizing calculations (using DistanceSquared, pointers)
- Hashing of curves
- Particle simulation distance hashing