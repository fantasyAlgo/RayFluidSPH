#pragma once


namespace settings {

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 800;
constexpr int N_PARTICLES = 1400;
constexpr float PARTICLE_RAD = 20;
constexpr float DAMPING = 0.9f;

constexpr int RADIUS_CHUNK = 40;
constexpr int NChunksX = SCREEN_WIDTH/RADIUS_CHUNK;
constexpr int NChunksY = SCREEN_HEIGHT/RADIUS_CHUNK;

extern float SMOOTHING_RAD;
extern float VOLUME_SR;
extern float VOLUME_SV;
extern float PARTICLE_MASS;
extern float TARGET_DENSITY;
extern float PRESSURE_MULT;
extern float VISCOSITY_STRENGTH;


void init();
void update();
void setWindowSettings(int width, int height);
void setBlockSettings(int block_x, int block_y);
  
}

