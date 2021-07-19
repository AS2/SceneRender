#pragma once
#include "object.h"

class object_plane : public object
{
public:
  vec3 normal, center;

  object_plane(vec3 normal, vec3 center)
  {
    this->normal = normal;
    this->center = center;
  }

  virtual low_level_object gen_low_level_object(void)
  {
    low_level_object tmp;
    tmp.type = PLANE;
    tmp.Arr[0] = normal.getX();
    tmp.Arr[1] = normal.getY();
    tmp.Arr[2] = normal.getZ();
    tmp.Arr[3] = center.getX();
    tmp.Arr[4] = center.getY();
    tmp.Arr[5] = center.getZ();
    return tmp;
  }
};