# Particle Simulator using SFML

### TODO:
**Async Multithreading?**

**Important:**
1. Bug: When interaction radius is very large, particles tend to move top-left. Probably related to uneven space warping at the edges (right and bottom)
2. Better thread pooling (don't create new one every single time) + separate from rendering
3. Single vector for grid

**Smaller features:**
- Particle spawning (needs buffering to work with threads)
- Shift + scroll = resize brush (add to controls hint)
- Shift to speed up scrolling (add to controls hint)
- Logarithmic range scaling
- Sliders instead of range
- Visualize velocity as slider
- Spatial grid ratio
