#include "include/ParticleSystem.hpp"
#include "include/Settings.hpp"
#include <algorithm>
#include <cmath>
#include "raymath.h"
#include <functional>
#include <iostream>
#include "rlImGui.h"
#include "imgui.h"
#include <math.h>
#include <raylib.h>
#include <stdatomic.h>


float smoothingKernel(float dst){
  if (dst >= settings::SMOOTHING_RAD) return 0; float value = std::max(0.0f, settings::SMOOTHING_RAD- dst);
  return (value*value) / settings::VOLUME_SR;
}
float smoothingKernelDerivative(float dst){
  if (dst >= settings::SMOOTHING_RAD) return 0;
  float value = settings::SMOOTHING_RAD - dst;
  float scale = 12.0f/(pow(settings::SMOOTHING_RAD, 4)*M_PI);
  return -value * scale;
}
float smoothingNearKernel(float dst){
  if (dst >= settings::SMOOTHING_RAD) return 0;
  float value = std::max(0.0f, settings::SMOOTHING_RAD - dst);
  return (value*value*value) / settings::SpikyPow3ScalingFactor;
}
float smoothingNearKernelDer(float dst){
  if (dst >= settings::SMOOTHING_RAD) return 0;
  float value = std::max(0.0f, settings::SMOOTHING_RAD - dst);
  return -(value*value) / settings::SpikyPow3DerScalingFactor;
}

float viscosityKernel(float dst){
  if (dst >= settings::SMOOTHING_RAD) return 0;
  float value = std::max(0.0f, settings::SMOOTHING_RAD*settings::SMOOTHING_RAD - dst*dst);
  return (value*value*value)/settings::VOLUME_SV;
}

float density2Pressure(float density){
  float diff = density*10000.0f - settings::TARGET_DENSITY;
  return diff*settings::PRESSURE_MULT;
}
float nearDensity2Pressure(float density){
  return density*settings::NEAR_PRESSURE_MULT;
}



void ParticleSystem::init(){
  int sqrt_particles = std::sqrt(settings::N_PARTICLES);
  int center_x = (settings::SCREEN_WIDTH - (settings::PARTICLE_RAD+1.0f)*sqrt_particles)/2;
  int center_y = (settings::SCREEN_HEIGHT - (settings::PARTICLE_RAD+1.0f)*sqrt_particles)/2;
  int indx = 0;
  for (int i = 0; i < sqrt_particles; i++) {
    for (int j = 0; j < sqrt_particles; j++) {
      this->particles[indx].pos = {(float)(rand()%(settings::SCREEN_WIDTH- (int)settings::PARTICLE_RAD)), (float)(rand()%(settings::SCREEN_HEIGHT - (int)settings::PARTICLE_RAD))};
      //this->particles[indx].pos = {center_x+(settings::PARTICLE_RAD*1.0f+1.0f)*i, center_y + (settings::PARTICLE_RAD*1.0f + 1.0f)*j};
      this->particles[indx].predPos = this->particles[indx].pos;
      indx++;
    }
  }
  /*this->distanceF = [this](ParticleSystem* p, int indx, int indx_p){
    //p->distances[indx][indx_p] = Vector2Distance(p->particles[indx].predPos, p->particles[indx_p].predPos)*1.0f;
  };*/
  this->pressureF = [this](ParticleSystem* p, int i, int j){
    float slope, sharedPressure, nearSlope, nearSharedPressure;
    Vector2 dir;
    if (i == j) return;
    float distance = p->getDistance(i, j);
    dir = Vector2Normalize(Vector2Subtract(p->particles[i].predPos, p->particles[j].predPos));
    if (dir.x == 0 && dir.y == 0) dir = Vector2Normalize({(float)(rand()%100), (float)(rand()%100)});

    slope = smoothingKernelDerivative(distance);
    nearSlope = smoothingKernelDerivative(distance);

    sharedPressure = (density2Pressure(p->particles[i].density) + density2Pressure(p->particles[j].density))/2.0f;
    nearSharedPressure = (nearDensity2Pressure(p->particles[i].nearDensity) + nearDensity2Pressure(p->particles[j].nearDensity))/2.0f;
    p->particles[i].pressureForce = p->particles[i].pressureForce + 
                                      dir*(-(slope*settings::PARTICLE_MASS*sharedPressure)/(p->particles[i].density+0.001f));
    p->particles[i].pressureForce = p->particles[i].pressureForce + 
                                      dir*(-(nearSlope*settings::PARTICLE_MASS*nearSharedPressure)/(p->particles[i].nearDensity+0.001f));
    
    //std::cout << nearSharedPressure << " " << p->particles[i].pressureForce.x << std::endl;

  };
  this->densityF = [this](ParticleSystem* p, int i, int j){
    if (i == j) return;
    float distance = p->getDistance(i, j);
    p->particles[i].density += smoothingKernel(distance);
    p->particles[i].nearDensity += smoothingNearKernel(distance);
  };
  this->viscosityF = [this](ParticleSystem* p, int i, int j){
    float influence = viscosityKernel(p->getDistance(i, j));
    p->particles[i].viscosity += (p->particles[i].vel - p->particles[j].vel)*influence;
  };

}

void ParticleSystem::updateChunks(){
  for (int i = 0; i < settings::NChunksX; i++)
    for (int j = 0; j < settings::NChunksY; j++) 
      chunks[i][j].len = 0;
  int x, y;
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    x = (particles[i].pos.x/(float)settings::SCREEN_WIDTH)*settings::NChunksX;
    y = (particles[i].pos.y/(float)settings::SCREEN_HEIGHT)*settings::NChunksY;
    x = std::max(0, std::min(x, settings::NChunksX-1));
    y = std::max(0, std::min(y, settings::NChunksY-1));
    chunks[x][y].indxs[chunks[x][y].len++] = i;
  }

}

void ParticleSystem::update(const std::function<void(ParticleSystem*, int, int)>& updateF, int indx, int c_x, int c_y){
  c_x = std::max(0, std::min(c_x, settings::NChunksX-1));
  c_y = std::max(0, std::min(c_y, settings::NChunksY-1));
  int indx_p;
  for (int i = -1; i < 2; i++) {
    if (c_x-i < 0 || c_x-i > settings::NChunksX-1) continue;
    for (int j = -1; j < 2; j++) {
      if (c_y-j < 0 || c_y-j > settings::NChunksY-1) continue;
      //std::cout << c_x-i << " " << c_y-j << std::endl;
      for (int k = 0; k < chunks[c_x-i][c_y-j].len; k++) {
        indx_p =  chunks[c_x-i][c_y-j].indxs[k];
        updateF(this, indx, indx_p);
      }
    }
  }
}

void ParticleSystem::updateDistances(){
  return;
  int x, y;
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    x = (particles[i].pos.x/(float)settings::SCREEN_WIDTH)*settings::NChunksX;
    y = (particles[i].pos.y/(float)settings::SCREEN_HEIGHT)*settings::NChunksY;
    this->update(this->distanceF, i, x, y);
  }
}
void ParticleSystem::updatePressure(){
  int x, y;
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    particles[i].pressureForce = Vector2Zero();
    x = (particles[i].pos.x/(float)settings::SCREEN_WIDTH)*settings::NChunksX;
    y = (particles[i].pos.y/(float)settings::SCREEN_HEIGHT)*settings::NChunksY;
    this->update(this->pressureF, i, x, y);

    particles[i].pressureForce += particles[i].viscosity;
    // Gravity
    particles[i].pressureForce.y += 100.0f;

    Vector2 mouse_pos = GetMousePosition();
    if ((isRepulsionOn || isAntiRepulsion) && Vector2Distance(particles[i].pos, mouse_pos) < settings::SMOOTHING_RAD*3)
      particles[i].pressureForce += Vector2Scale(Vector2Normalize(Vector2Subtract(particles[i].pos, GetMousePosition())), -2000.0f)*(isAntiRepulsion ? -1.0f : 1.0f);
  }
  
}
void ParticleSystem::updateDensity(){
  float x, y;
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    particles[i].density = 0;
    particles[i].nearDensity = 0;
    x = (particles[i].pos.x/(float)settings::SCREEN_WIDTH)*settings::NChunksX;
    y = (particles[i].pos.y/(float)settings::SCREEN_HEIGHT)*settings::NChunksY;
    this->update(this->densityF, i, x, y);
  }
}
void ParticleSystem::updateViscosity(){
  float x, y;
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    particles[i].viscosity = Vector2Zero();
    x = (particles[i].pos.x/(float)settings::SCREEN_WIDTH)*settings::NChunksX;
    y = (particles[i].pos.y/(float)settings::SCREEN_HEIGHT)*settings::NChunksY;
    this->update(this->viscosityF, i, x, y);
    this->particles[i].viscosity *= settings::VISCOSITY_STRENGTH;
  }

}

void ParticleSystem::updateBase(float deltaTime){
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    particles[i].predPos = Vector2Add(particles[i].pos, Vector2Scale(particles[i].vel, deltaTime));
  }
}
void ParticleSystem::update(float deltaTime){
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    particles[i].vel = Vector2Add(particles[i].vel, Vector2Scale(particles[i].pressureForce, deltaTime));
    /*if (i == mouseParticle){
      std::cout << particles[i].pressureForce.x << " " << particles[i].pressureForce.y << std::endl;
      std::cout << "Then velocity: " << particles[i].vel.x << " " << particles[i].vel.y << std::endl;
    }*/

    //particles[i].vel = Vector2ClampValue(particles[i].vel, -100, 100);
    //std::cout << particles[i].vel.x << std::endl;
    particles[i].pos = Vector2Add(particles[i].pos, Vector2Scale(particles[i].vel, deltaTime));
    if (particles[i].pos.y < 0 || particles[i].pos.y > settings::SCREEN_HEIGHT){
      particles[i].vel.y *= -settings::DAMPING;
      particles[i].pos.y = particles[i].pos.y < 0 ? 0 : settings::SCREEN_HEIGHT;
    }
    if (particles[i].pos.x < 0 || particles[i].pos.x > settings::SCREEN_WIDTH){
      particles[i].vel.x *= -settings::DAMPING;
      particles[i].pos.x = particles[i].pos.x < 0 ? 0 : settings::SCREEN_WIDTH;
    }
  }

}

void ParticleSystem::inputHandling(float deltaTime){
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
    float min_dist = 1000000.0f;
    int indx = 0;
    Vector2 mouse_pos = GetMousePosition();
    for (int i = 0; i < settings::N_PARTICLES; i++) {
      if (min_dist > Vector2Distance(particles[i].pos, mouse_pos)){
        indx = i;
        min_dist = Vector2Distance(particles[i].pos, mouse_pos);
      }
    }
    this->mouseParticle = indx;
  }
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
    this->particles[mouseParticle].pos = GetMousePosition();
  }
  if (IsKeyDown(KEY_R)){
    for (int i = 0; i < settings::N_PARTICLES; i++) 
      particles[i].vel = Vector2Zero();
  }

  if (IsKeyDown(KEY_Q)) isRepulsionOn = true;
  else if (IsKeyDown(KEY_E)) isAntiRepulsion = true;
  else {
    isRepulsionOn = false;
    isAntiRepulsion = false;
  }
}
void ParticleSystem::render(){
  unsigned char trans_value;
  float pressure;
  for (int i = 0; i < settings::SCREEN_WIDTH; i+=settings::RADIUS_CHUNK) 
    DrawLine(i, 0, i, settings::SCREEN_HEIGHT, BLACK);
  for (int i = 0; i < settings::SCREEN_HEIGHT; i+=settings::RADIUS_CHUNK) 
    DrawLine(0, i, settings::SCREEN_WIDTH, i, BLACK);

  DrawCircleV(particles[this->mouseParticle].pos, settings::SMOOTHING_RAD, RED);
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    pressure = settings::PRESSURE_MULT*(settings::TARGET_DENSITY-particles[i].density*10000.0f);
    trans_value = Vector2Length(particles[i].vel)*5.0f;
    //DrawCircleV(particles[i].pos, particles[i].density*10000.0f, {0, 0, 255, 125}); 
    DrawCircleV(particles[i].pos, settings::SMOOTHING_RAD/3, {(unsigned char)trans_value, 0, (unsigned char)(255-trans_value), 255});
    /*if (pressure > 0)
      DrawCircleV(particles[i].pos, settings::PARTICLE_RAD/4, {trans_value, 0 ,trans_value, 255});
    else if (pressure < 0) DrawCircleV(particles[i].pos, settings::PARTICLE_RAD/4, {0, trans_value, 0, 255});
    else DrawCircleV(particles[i].pos, settings::PARTICLE_RAD/4, {0, 0, 0, 255});*/
    //DrawLineV(particles[i].pos, Vector2Add(particles[i].pos, Vector2Scale(particles[i].pressureForce, 50)), BLACK);
  }
}

void ParticleSystem::renderUI(){
  ImGui::Begin("Parameter Settings"); // Begin window
  ImGui::Text("Adjust the parameters below:");
  ImGui::SliderFloat("smoothing rad:", &settings::SMOOTHING_RAD, 0.0f, 80.0f, "%.1f");
  ImGui::SliderFloat("Pressure mult:", &settings::PRESSURE_MULT, 0.0f, 10000.0f, "%.1f");
  ImGui::SliderFloat("Target density:", &settings::TARGET_DENSITY, 0.0f, 5.0f, "%.4f");
  ImGui::SliderFloat("VISCOSITY_STRENGTH:", &settings::VISCOSITY_STRENGTH, -100.0f, 100.0f, "%.4f");
  ImGui::SliderFloat("Near pressure mult:", &settings::NEAR_PRESSURE_MULT, -10000.0f, 10000.0f, "%.4f");
  settings::update();


  ImGui::Text("Density at 66: %.6f", particles[this->mouseParticle].density*10000.0f);
  ImGui::Text("Near density at 66: %.6f", particles[this->mouseParticle].nearDensity*10000.0f);
  ImGui::Text("Pressure x at 66: %.3f %.3f", particles[mouseParticle].pressureForce.x, particles[mouseParticle].pressureForce.y);
  ImGui::Text("viscosity x at 66: %.3f %.3f", particles[mouseParticle].viscosity.x, particles[mouseParticle].viscosity.y);
  ImGui::Text("Velocity at 66: %.3f %.3f", particles[mouseParticle].vel.x, particles[mouseParticle].vel.y);
  ImGui::Text("Position x at 66: %.6f %.6f", particles[mouseParticle].pos.x, particles[mouseParticle].pos.y);
  /*int x = (particles[mouseParticle].pos.x/(float)settings::SCREEN_WIDTH)*settings::NChunksX;
  int y = (particles[mouseParticle].pos.y/(float)settings::SCREEN_HEIGHT)*settings::NChunksY;
  x = std::max(0, std::min(x, settings::NChunksX-1));
  y = std::max(0, std::min(y, settings::NChunksY-1));
  ImGui::Text("Miah: pos %.1f %.1f: %.1f", (float)x, (float)y, (float)this->chunks[x][y].len);*/


  ImGui::End(); // End window
}

float ParticleSystem::getDistance(int i, int j){
  if ((i >= 0 && i < settings::N_PARTICLES) && (j >= 0 && j < settings::N_PARTICLES))
    return Vector2Distance(this->particles[i].predPos, this->particles[j].predPos)*1.0f;
  return 1;
}
