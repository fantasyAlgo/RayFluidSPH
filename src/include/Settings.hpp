#pragma once


namespace settings {

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int N_PARTICLES = 729;
constexpr float PARTICLE_RAD = 20;
constexpr float DAMPING = 0.9f;
constexpr int NChunksX = SCREEN_WIDTH/25;
constexpr int NChunksY = SCREEN_HEIGHT/25;

extern float SMOOTHING_RAD;
extern float VOLUME_SR;
extern float PARTICLE_MASS;
extern float TARGET_DENSITY;
extern float PRESSURE_MULT;


void init_settings();
void setWindowSettings(int width, int height);
void setBlockSettings(int block_x, int block_y);
  
}

