#pragma once

#include "Settings.hpp"
#include "raylib.h"
#include <functional>

typedef struct Particle {
  Vector2 pos;
  Vector2 predPos = {0.0f, 0.0f};
  Vector2 vel = {0.0f, 0.0f};
  Vector2 pressureForce;
  float density;
} Particle;

typedef struct Chunk {
  int len;
  int indxs[settings::N_PARTICLES];
} Chunk;

float smoothingKernel(float distance);

class ParticleSystem {
private:
  Particle particles[settings::N_PARTICLES];
  float distances[settings::N_PARTICLES][settings::N_PARTICLES];
  Chunk chunks[settings::NChunksX][settings::NChunksY];
  int mouseParticle = 0;
  bool isRepulsionOn = false;
public:
  void init();

  void update(const std::function<void(ParticleSystem&, int, int)>& updateF, int indx, int c_x, int c_y);
  void updateChunks();
  void updateDistances();
  void updateDensity();
  void updatePressure();
  void updateBase(float deltaTime);
  void update(float deltaTime);
  void inputHandling(float deltaTime);
  void render();
  void renderUI();
};
