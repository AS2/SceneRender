#pragma once
#include "object.h"

class object_sph : public object 
{
  public:
    vec3 Pos;
    double Radius;

    object_sph(vec3 Pos, double Radius)
    {
      this->Pos = Pos;
      this->Radius = Radius;
    }

    virtual low_level_object gen_low_level_object(void)
    {
      low_level_object tmp;
      tmp.type = SPHERE;
      tmp.Arr[0] = Pos.getX();
      tmp.Arr[1] = Pos.getY();
      tmp.Arr[2] = Pos.getZ();
      tmp.Arr[3] = Radius;
      return tmp;
    }
};
