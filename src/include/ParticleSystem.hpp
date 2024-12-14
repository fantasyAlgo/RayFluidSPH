#pragma once

#include "Settings.hpp"
#include "raylib.h"
#include <functional>

typedef struct Particle {
  Vector2 pos;
  Vector2 predPos = {0.0f, 0.0f};
  Vector2 vel = {0.0f, 0.0f};
  Vector2 pressureForce;
  Vector2 viscosity;
  float density = 0;
  float nearDensity = 0;
} Particle;

typedef struct Chunk {
  short len = 0;
  short indxs[settings::N_PARTICLES/10];
} Chunk;

float smoothingKernel(float dst);
float viscosityKernel(float dst);

float smoothingNearKernel(float dst);
float smoothingNearKernelDer(float dst);

class ParticleSystem {
private:
  Particle particles[settings::N_PARTICLES];
  //float distances[settings::N_PARTICLES][settings::N_PARTICLES];
  Chunk chunks[settings::NChunksX][settings::NChunksY];
  int mouseParticle = 0;
  bool isRepulsionOn = false;
  bool isAntiRepulsion = false;
  std::function<void(ParticleSystem*, int, int)> distanceF;
  std::function<void(ParticleSystem*, int, int)> pressureF;
  std::function<void(ParticleSystem*, int, int)> densityF;
  std::function<void(ParticleSystem*, int, int)> viscosityF;
  float getDistance(int i, int j);
public:
  void init();

  void update(const std::function<void(ParticleSystem*, int, int)>& updateF, int indx, int c_x, int c_y);
  void updateChunks();
  void updateDistances();
  void updateDensity();
  void updatePressure();
  void updateViscosity();
  void updateBase(float deltaTime);
  void update(float deltaTime);
  void inputHandling(float deltaTime);
  void render();
  void renderUI();

};
