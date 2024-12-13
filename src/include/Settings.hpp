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
extern float SpikyPow3ScalingFactor;
extern float SpikyPow3DerScalingFactor;

extern float PARTICLE_MASS;
extern float TARGET_DENSITY;
extern float PRESSURE_MULT;
extern float NEAR_PRESSURE_MULT;
extern float VISCOSITY_STRENGTH;


void init();
void update();
void setWindowSettings(int width, int height);
void setBlockSettings(int block_x, int block_y);
  
}

/*
         compute.SetFloat("deltaTime", deltaTime);
        compute.SetFloat("gravity", gravity);
        compute.SetFloat("collisionDamping", collisionDamping);
        compute.SetFloat("smoothingRadius", smoothingRadius);
        compute.SetFloat("targetDensity", targetDensity);
        compute.SetFloat("pressureMultiplier", pressureMultiplier);
        compute.SetFloat("nearPressureMultiplier", nearPressureMultiplier);
        compute.SetFloat("viscosityStrength", viscosityStrength);
        compute.SetVector("boundsSize", boundsSize);
        compute.SetVector("obstacleSize", obstacleSize);
        compute.SetVector("obstacleCentre", obstacleCentre);

        compute.SetFloat("Poly6ScalingFactor", 4 / (Mathf.PI * Mathf.Pow(smoothingRadius, 8)));
        compute.SetFloat("SpikyPow3ScalingFactor", 10 / (Mathf.PI * Mathf.Pow(smoothingRadius, 5)));
        compute.SetFloat("SpikyPow2ScalingFactor", 6 / (Mathf.PI * Mathf.Pow(smoothingRadius, 4)));
        compute.SetFloat("SpikyPow3DerivativeScalingFactor", 30 / (Mathf.Pow(smoothingRadius, 5) * Mathf.PI));
        compute.SetFloat("SpikyPow2DerivativeScalingFactor", 12 / (Mathf.Pow(smoothingRadius, 4) * Mathf.PI));
*/
