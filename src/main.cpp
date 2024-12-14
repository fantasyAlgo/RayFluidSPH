#include "include/ParticleSystem.hpp"
#include "include/Settings.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include <iostream>


int main(void){
  settings::init();
  settings::update();
  InitWindow(settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT, "raylib [core] example - basic window");
  rlImGuiSetup(true);

  SetTargetFPS(144);
  ParticleSystem particleSystem;
  particleSystem.init();

  float deltaTime;
  float fixedDeltaTime = 1.0f/160.0f;
  fixedDeltaTime*=2.0f;
  bool runFrame = false;
  particleSystem.updateChunks();
  particleSystem.updateDistances();
  particleSystem.updateDensity();
  particleSystem.updatePressure();

  while (!WindowShouldClose()) {

    deltaTime = GetFrameTime();
    particleSystem.updateChunks();
    particleSystem.updateBase(fixedDeltaTime);
    particleSystem.inputHandling(fixedDeltaTime);

    particleSystem.updateDistances();
    particleSystem.updateDensity();
    particleSystem.updateViscosity();
    particleSystem.updatePressure();

    if (runFrame)
      particleSystem.update(fixedDeltaTime);
    //runFrame = false;
    if (IsKeyPressed(KEY_A)) runFrame = !runFrame;
    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawFPS(10, 10);
      particleSystem.render();
      rlImGuiBegin();
        particleSystem.renderUI();
      rlImGuiEnd();
    EndDrawing();
  }

  CloseWindow();        // Close window and OpenGL context

  return 0;
}
