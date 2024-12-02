#include "include/ParticleSystem.hpp"
#include "include/Settings.hpp"
#include "raylib.h"
#include "rlImGui.h"
int main(void)
{
  settings::init_settings();
  InitWindow(settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT, "raylib [core] example - basic window");
  rlImGuiSetup(true);

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
  ParticleSystem particleSystem;
  particleSystem.init();

  float deltaTime;
  while (!WindowShouldClose()) {
    deltaTime = GetFrameTime();

    particleSystem.updateDistances();
    particleSystem.updateDensity();
    particleSystem.updatePressure();
    particleSystem.update(deltaTime);
    BeginDrawing();
      ClearBackground(RAYWHITE);
      particleSystem.render();
      rlImGuiBegin();
        particleSystem.renderUI();
      rlImGuiEnd();
    EndDrawing();
  }

  CloseWindow();        // Close window and OpenGL context

  return 0;
}
