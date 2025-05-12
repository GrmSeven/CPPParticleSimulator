# Particle Simulator using SFML

### Controls:
Space - Pause\
Middle Mouse Button - Drag the screen\
Scroll wheel - Zoom in/out\
Arrow keys - Move around\
Left click - Drag/Attract particles\

### TODO:
Threads:
- Events + Camera updating + UI updating
- Particle rendering + UI Rendering
- Sync from UI -> Particle simulation

Smaller improvements:
- Move particle rendering into particle simulator itself
- Add visible border to the simulator
- Shift + scroll = resize brush
- Shift to speed up scrolling.

Smaller optimizations:
- Profiler: https://www.jetbrains.com/help/clion/cpu-profiler.html#perf-output
- Optimizing calculations (using DistanceSquared, pointers)
- Hashing of curves
- Particle simulation distance hashing