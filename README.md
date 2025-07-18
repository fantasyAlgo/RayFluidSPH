# Fluid simulation using SPH
![fluidSimulation](https://github.com/user-attachments/assets/7f56a920-585c-4ce9-8889-2b1f4a9f4c3b)
## Prerequisites
- CMake
- Raylib, Cmake will automatically install it if you don't have it locally
## How to run
This is a cmake project, so as always
```bash
git clone https://github.com/fantasyAlgo/fluidSimulationExp.git
mkdir build
cmake ..
cd build
build
./FluidSim
```
Then, if you want a stable simulation, increase the pressure multiplier, and return it back to normal when the simulation is settled
## Input
- Q for pushing the particle inwards at the mouse coord 
- E for push the particle outwards at the mouse coord
- R to reset the velocities
- A to stop the simulation

Click to one particle to see it's position, density, near-density, viscosity, pressure force and velocity
## Features
It is essentially a worse copy of the beutiful Sebastian Lague [video](https://www.youtube.com/watch?v=rSKMYc1CQHE&t=2288s) on fluid simulation but in raylib and c++ (2d), it is also extremely prone to explode.
