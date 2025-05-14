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
**Smaller features:**
- Particle spawning (needs buffering to work with threads)
- Shift + scroll = resize brush (add to controls hint)
- Shift to speed up scrolling (add to controls hint)

\
**Smaller optimizations:**
- Optimizing calculations (using DistanceSquared, pointers)
- Hashing of curves
- Particle simulation distance hashing

\
**Bugs:**
- When interaction radius is very large, particles tend to move top-left. Probably related to uneven space warping