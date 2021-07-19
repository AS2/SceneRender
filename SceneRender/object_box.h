#pragma once
#include "object.h"

class object_box : public object
{
public:
  vec3 dot1, dot2, dot3, dot4;

  object_box(vec3 dot1, vec3 dot2, vec3 dot3, vec3 dot4)
  {
    this->dot1 = dot1;
    this->dot2 = dot2;
    this->dot3 = dot3;
    this->dot4 = dot4;
  }

  virtual low_level_object gen_low_level_object(void)
  {
    low_level_object tmp;
    tmp.type = BOX;

    tmp.Arr[0] = dot1.getX();
    tmp.Arr[1] = dot1.getY();
    tmp.Arr[2] = dot1.getZ();

    tmp.Arr[3] = dot2.getX();
    tmp.Arr[4] = dot2.getY();
    tmp.Arr[5] = dot2.getZ();

    tmp.Arr[6] = dot3.getX();
    tmp.Arr[7] = dot3.getY();
    tmp.Arr[8] = dot3.getZ();

    tmp.Arr[9] = dot4.getX();
    tmp.Arr[10] = dot4.getY();
    tmp.Arr[11] = dot4.getZ();

    return tmp;
  }
};