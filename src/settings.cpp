#include "include/Settings.hpp"
#include <math.h>
#include <cmath>

namespace settings {
float SMOOTHING_RAD;
float VOLUME_SR;
float PARTICLE_MASS;
float TARGET_DENSITY;
float PRESSURE_MULT;
void init_settings(){
  SMOOTHING_RAD = 5;
  PARTICLE_MASS = 1.0f;
  TARGET_DENSITY = 6;
  PRESSURE_MULT = 0.6;
  VOLUME_SR = ((float)M_PI*std::pow(settings::SMOOTHING_RAD, 8))/4.0f;

}

}