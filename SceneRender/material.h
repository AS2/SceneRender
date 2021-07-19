#pragma once

#include "vec3.h"

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
};