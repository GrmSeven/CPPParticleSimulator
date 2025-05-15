# Particle Simulator using SFML

### TODO:
Profiler: https://www.jetbrains.com/help/clion/cpu-profiler.html#perf-output

**Multithreading:**

Sync version (easier) DONE
- Events -> [Multiple threads for particles] -> Rendering -> Repeat

Async version (harder, slightly better)
- Events + Camera updating + UI updating
- Particle rendering + UI Rendering
- Particle simulation

\
**Important optimizations:**
1. Single instruction, multiple data optimization
2. Simpler calculations for particle stuff (update_particle_velocity and handle_particle_velocity specifically) + (?get_particles_in_cell)
3. Allow resizing of spatial partitioning grid (and better formula for best performance)
4. Better thread pooling (don't create new one every single time) + separate from rendering
5. Particle update batching (?) single SoA vector with pointers to them? (probably won't help)

\
**Smaller features:**
- Particle spawning (needs buffering to work with threads)
- Shift + scroll = resize brush (add to controls hint)
- Shift to speed up scrolling (add to controls hint)
- Logarithmic range scaling
- Sliders instead of range
- Visualize velocity as slider

\
**Smaller optimizations:**
- Optimizing calculations (using DistanceSquared, pointers)
- Hashing of curves
- Particle simulation distance hashing

\
**Bugs:**
- When interaction radius is very large, particles tend to move top-left. Probably related to uneven space warping at the edges (right and bottom)