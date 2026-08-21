# Particle System

## Overview
A particle simulation engine that uses gravitational dynamics, spring forces, and collision detection using spatial partitioning for efficient computation. The system currently models **100–1000 particles** interacting on a 2D grid with energy conservation validation and adaptive timestep processing.

## Physics Model

### Forces and Acceleration

**Gravitational Force: for particles in freefall or flying upwards**:
```
a = -g = -9.8 m/s²
```

**Spring Force (when y ≤ spring height)**):
```
F_spring = -k(y - h)
a_total = -(k/m)(y - h) - g
acceleration is the force of the spring/mass minus gravity
```

where `k` is dynamically calculated based on particle count and assumed compression (0.2 m), scaled by factor of 4 for springiness:
```
k = ( 9.8 × m / 0.2) × 4
Multiplied by 4 for strength and springiness
```

### Numerical Integration

**Euclidean Approximation** with fixed timestep `dt = 0.1 s`:
```
v(t+dt) = v(t) + a(t) × dt
x(t+dt) = x(t) + v(t) × dt
```

Updates apply at (`0.01 s` intervals) within each frame for stability.

## Program Architecture

### Spatial Partitioning

The simulation uses a dynamically sized grid(sqrt(num of particles/10)+2) of equal dimentions to reduce collision detection from O(n²) to O(n) for typical particle distributions:
- **Grid cell width**: 800 / cells per row
- **Collision checks**: Only particles in the same grid cell are tested
- **Grid update**: partial rebuild if any particles move cells via `clearAndFix()` to update the grid and manage resources effectively.

This optimization scales efficiently to 1000 particles without performance degradation.

### Collision Detection

#### Particle–Particle Collisions
- **Trigger**: Euclidean distance `absDx * absDx + absDy * absDy < 100` units (r^2)
- **Axis determination**: Compare `|dx|` vs. `|dy|` to resolve collision along correct axis
- **Response**:
  - **Y-axis collision**: Swap vy for energy conservation
  - **X-axis collision**: Swap vx for energy conservation
  - **If X and Y axis are equal**: apply Y-axis collision conditions.

#### Wall Collisions
- **Bounds**: `[1, 799]` × `[1, 799]`
- **Response**: Reverse normal velocity component, clamp position to boundary

### Frame Timing

Uses `std::chrono::high_resolution_clock` to decouple frame rate from simulation timestep:
```
accumulator += frameDuration.count()
while (accumulator >= dt) {
    update()
    accumulator -= dt
}
```

Ensures consistent physics independent of frame rate or system load.

## Code Structure

| File | Purpose |
|------|---------|
| `particleSim2.0.cpp` | Initialization, user input validation, grid setup, main loop |
| `Particles.cpp` | Physics update, collision detection, grid management |
| `Particles.h` | Class definitions (`Particles`, `Spring`), function declarations |

### Key Functions

| Function | Signature | Purpose |
|----------|-----------|---------|
| `updatePos()` | `void(vector<Particles>&, Spring&)` | Apply forces, update velocity and position |
| `wallCollis()` | `void(vector<Particles>&)` | Handle boundary collisions |
| `particleCollis()` | `void(vector<Particles*>&)` | Detect and resolve particle–particle collisions within grid cells with more than 1 particle |
| `clearAndFix()` | `void(vector<Particles>&, vector<vector<vector<Particles*>>>&, int width)` | Clears and puts any particle that moves cells in accordance to it's current position|
| `csvDump()` | `void csvDump(std::vector<Particles>&, std::fstream&, float)` | Adds data to csv file for further analysis|
---

**Date**: August 2026  
**License**: MIT
