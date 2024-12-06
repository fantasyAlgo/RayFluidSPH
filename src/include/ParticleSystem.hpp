#pragma once

#include "Settings.hpp"
#include "raylib.h"

typedef struct Particle {
  Vector2 pos;
  Vector2 predPos = {0.0f, 0.0f};
  Vector2 vel = {0.0f, 0.0f};
  Vector2 pressureForce;
  float density;
} Particle ;

float smoothingKernel(float distance);

class ParticleSystem {
private:
  Particle particles[settings::N_PARTICLES];
  float distances[settings::N_PARTICLES][settings::N_PARTICLES];
  int mouseParticle = 0;
  bool isRepulsionOn = false;
public:
  void init();
  void updateDistances();
  void updateDensity();
  void updatePressure();
  void updateBase(float deltaTime);
  void update(float deltaTime);
  void inputHandling(float deltaTime);
  void render();
  void renderUI();
};
