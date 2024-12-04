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
  SMOOTHING_RAD = 34.4f;
  PARTICLE_MASS = 1.0f;
  TARGET_DENSITY = 0.1f;
  PRESSURE_MULT = 2.0f;
  VOLUME_SR = (M_PI*std::pow(settings::SMOOTHING_RAD, 4))/6.0f;

}

}
