#pragma once
#include "object.h"

struct vec3
{
  double x, y, z;
};

class object_sph : object 
{
  private:
    vec3 Pos;
    double Radius;
  public:
    object_sph(vec3 Pos, double Radius)
    {
      this->Pos = Pos;
      this->Radius = Radius;
    }

    virtual low_level_object gen_low_level_object(void)
    {
      low_level_object tmp;
      tmp.type = SPHERE;
      tmp.Arr[0] = Pos.x;
      tmp.Arr[1] = Pos.y;
      tmp.Arr[2] = Pos.z;
      tmp.Arr[3] = Radius;
    }
};
