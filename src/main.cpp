#include "include/ParticleSystem.hpp"
#include "include/Settings.hpp"
#include "raylib.h"
#include "rlImGui.h"


int main(void){
  settings::init_settings();
  InitWindow(settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT, "raylib [core] example - basic window");
  rlImGuiSetup(true);

  SetTargetFPS(144);               // Set our game to run at 60 frames-per-second
  ParticleSystem particleSystem;
  particleSystem.init();

  float deltaTime;
  float fixedDeltaTime = 1.0f/144.0f;
  fixedDeltaTime*=2.0f;
  bool runFrame = false;
  particleSystem.updateDistances();
  particleSystem.updateDensity();
  particleSystem.updatePressure();

  while (!WindowShouldClose()) {

    deltaTime = GetFrameTime();
    particleSystem.updateBase(fixedDeltaTime);
    particleSystem.inputHandling(fixedDeltaTime);

    particleSystem.updateDistances();
    particleSystem.updateDensity();
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
