#pragma once

#include "vec3.h"

struct low_level_material
{
  float Arr[8] = { 0 };    // 8 * 16
};

class material 
{
  public:
    vec3 color;
    double kAmb, kSpec, kDif, alpha, kRefl;

    material() 
    {
      this->color = vec3(0, 0, 0);
      this->kAmb = 0;
      this->kSpec = 0;
      this->kDif = 0;
      this->alpha = 0;
      this->kRefl = 0;
    }

    material(const vec3& color, const double& kAmb, const double& kSpec, const double& kDif, const double& alpha, const double& kRefl)
    {
      this->color = color;
      this->kAmb = kAmb;
      this->kSpec = kSpec;
      this->kDif = kDif;
      this->alpha = alpha;
      this->kRefl = kRefl;
    }

    low_level_material gen_low_level_material(void) { 
      low_level_material toReturn;

      toReturn.Arr[0] = color.getX();
      toReturn.Arr[1] = color.getY();
      toReturn.Arr[2] = color.getZ();
      toReturn.Arr[3] = kAmb;
      toReturn.Arr[4] = kSpec;
      toReturn.Arr[5] = kDif;
      toReturn.Arr[6] = alpha;
      toReturn.Arr[7] = kRefl;

      return toReturn; 
    };
};
