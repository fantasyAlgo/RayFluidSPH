#include "include/ParticleSystem.hpp"
#include "include/Settings.hpp"
#include <algorithm>
#include <cmath>
#include "raymath.h"
#include <iostream>
#include "rlImGui.h"
#include "imgui.h"
#include <math.h>
#include <raylib.h>


float smoothingKernel(float dst){
  if (dst >= settings::SMOOTHING_RAD) return 0;
  float value = std::max(0.0f, settings::SMOOTHING_RAD*settings::SMOOTHING_RAD - dst*dst);
  return (value*value*value) / settings::VOLUME_SR;
}
float smoothingKernelDerivative(float dst){
  float value = std::max(0.0f, settings::SMOOTHING_RAD*settings::SMOOTHING_RAD - dst*dst);
  return -6*dst*value*value/settings::VOLUME_SR;
}
float density2Pressure(float density){
  return (settings::TARGET_DENSITY - density*10000.0f)*settings::PRESSURE_MULT;
}



void ParticleSystem::init(){
  int sqrt_particles = std::sqrt(settings::N_PARTICLES);
  int center_x = (settings::SCREEN_WIDTH - (settings::PARTICLE_RAD+1.0f)*sqrt_particles)/2;
  int center_y = (settings::SCREEN_HEIGHT - (settings::PARTICLE_RAD+1.0f)*sqrt_particles)/2;
  int indx = 0;
  for (int i = 0; i < sqrt_particles; i++) {
    for (int j = 0; j < sqrt_particles; j++) {
      this->particles[indx].pos = {
        (float)(rand()%(settings::SCREEN_WIDTH- (int)settings::PARTICLE_RAD)), (float)(rand()%(settings::SCREEN_WIDTH- (int)settings::PARTICLE_RAD))
      };
      //this->particles[indx].pos = {center_x+(settings::PARTICLE_RAD*1.0f+1.0f)*i, center_y + (settings::PARTICLE_RAD*1.0f + 1.0f)*j};
      indx++;
    }
  }
}
void ParticleSystem::updateDistances(){
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    for (int j = 0; j < settings::N_PARTICLES; j++) {
      this->distances[i][j] = Vector2Distance(particles[i].pos, particles[j].pos)*1.0f;
      //std::cout << particles[i].pos.x << " " << particles[j].pos.x << std::endl;
      //std::cout << this->distances[i][j] << " " << particles[i].pos.x - particles[j].pos.x << std::endl;
    }
  }
}
void ParticleSystem::updatePressure(){
  float slope;
  Vector2 dir;
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    particles[i].pressureForce = Vector2Zero();
    for (int j = 0; j < settings::N_PARTICLES; j++) {
      dir = Vector2Normalize(Vector2Subtract(particles[i].pos, particles[j].pos));
      slope = smoothingKernelDerivative(distances[i][j]);
      if (particles[i].density == 0) particles[i].pressureForce = {0.0001f, 0.0f};
      else 
        particles[i].pressureForce = Vector2Add(particles[i].pressureForce, 
                                                Vector2Scale(dir, (slope*settings::PARTICLE_MASS*density2Pressure(particles[i].density))/particles[i].density));
      //std::cout << "slope: " << slope << " " << distances[i][j] << " " << smoothingKernel(distances[i][j]) << " " << particles[i].pressureForce.x << " " << particles[i].density << std::endl;
    }
  }
  
}
void ParticleSystem::updateDensity(){
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    particles[i].density = 0;
    for (int j = 0; j < settings::N_PARTICLES; j++) {
      if (i == j) continue;
      particles[i].density += smoothingKernel(distances[i][j]);
    }
  }
}
void ParticleSystem::update(float deltaTime){
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    particles[i].vel = Vector2Add(particles[i].vel, Vector2Scale(particles[i].pressureForce, deltaTime));
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
void ParticleSystem::render(){
  unsigned char trans_value;
  DrawCircleV(particles[66].pos, settings::SMOOTHING_RAD, RED);
  for (int i = 0; i < settings::N_PARTICLES; i++) {
    trans_value = settings::PRESSURE_MULT*(settings::TARGET_DENSITY-particles[i].density*10000.0f) + 125;
    DrawCircleV(particles[i].pos, particles[i].density*10000.0f, {0, 0, 255, 125}); 
    if (trans_value > 0)
      DrawCircleV(particles[i].pos, settings::PARTICLE_RAD/4, {trans_value, 0 ,trans_value, 255});
    else DrawCircleV(particles[i].pos, settings::PARTICLE_RAD/4, {0, 255,0, 255});
    //DrawLineV(particles[i].pos, Vector2Add(particles[i].pos, Vector2Scale(particles[i].pressureForce, 50)), BLACK);
  }
}

void ParticleSystem::renderUI(){
  ImGui::Begin("Parameter Settings"); // Begin window
  ImGui::Text("Adjust the parameters below:");
  ImGui::SliderFloat("smoothing rad:", &settings::SMOOTHING_RAD, 0.0f, 500.0f, "%.1f");
  ImGui::SliderFloat("Pressure mult:", &settings::PRESSURE_MULT, 0.0f, 20.0f, "%.1f");
  ImGui::SliderFloat("Target density:", &settings::TARGET_DENSITY, 0.0f, 10.0f, "%.2f");
  settings::VOLUME_SR = ((float)M_PI*std::pow(settings::SMOOTHING_RAD, 8))/4.0f;
  ImGui::Text("Density at 66: %.6f", particles[66].density);
  ImGui::Text("Pressure x at 66: %.3f %.3f", particles[66].pressureForce.x, particles[66].pressureForce.y);
  ImGui::Text("Position x at 66: %.3f %.3f", particles[66].pos.x, particles[66].pos.y);

  ImGui::End(); // End window
}
