#include "include/Settings.hpp"
#include <math.h>
#include <cmath>

namespace settings {
float SMOOTHING_RAD;
float VOLUME_SR;
float VOLUME_SV;
float PARTICLE_MASS;
float TARGET_DENSITY;
float PRESSURE_MULT;
float VISCOSITY_STRENGTH;
float NEAR_PRESSURE_MULT;
float SpikyPow3ScalingFactor;
float SpikyPow3DerScalingFactor;

void init(){
  VISCOSITY_STRENGTH = 100.0;
  SMOOTHING_RAD = 19.0f;
  PARTICLE_MASS = 1.0f;
  TARGET_DENSITY = 3.2f;
  PRESSURE_MULT = 2.0f;
  VOLUME_SR = (M_PI*std::pow(settings::SMOOTHING_RAD, 4))/6.0f;
  VOLUME_SV = M_PI*std::pow(SMOOTHING_RAD, 8)/4.0f;
  NEAR_PRESSURE_MULT = 1000.0f;
}
void update(){
  VOLUME_SR = (M_PI*std::pow(settings::SMOOTHING_RAD, 4))/6.0f;
  VOLUME_SV = (M_PI*std::pow(SMOOTHING_RAD, 8))/4.0f;
  SpikyPow3ScalingFactor = (M_PI*std::pow(SMOOTHING_RAD, 5))/10.0f;
  SpikyPow3DerScalingFactor = (M_PI*std::pow(SMOOTHING_RAD, 5))/30.0f;
}

}
