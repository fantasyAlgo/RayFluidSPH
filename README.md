# Fluid simulation using SPH

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
Then, if you want a stable simulation, start with a low pressure, and increase it to the maximum after
## Input
- Q for pushing the particle inwards at the mouse coord 
- E for push the particle outwards at the mouse coord
- R to reset the velocities
- A to stop the simulation
Click to one particle to see it's position, viscosity, pressure force and velocity
