#pragma once

#include "vec3.h"

class light {
public:
  vec3 pos, color;

  light(const vec3& pos, const vec3& color)
  {
    this->pos = pos;
    this->color = color;
  }

  double DistToLightSource(const vec3& pos) 
  {
    return (this->pos - pos).length();
  }
};