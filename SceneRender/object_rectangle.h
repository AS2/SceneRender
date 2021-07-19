#pragma once
#include "object.h"

static vec3 crosMulVec3(vec3& v1, vec3& v2)
{
  return vec3((v1.getY() * v2.getZ() - v2.getY() * v1.getZ()),
              (v1.getX() * v2.getZ() - v2.getX() * v1.getZ()) * (-1),
              (v1.getX() * v2.getY() - v2.getX() * v1.getY()));
}

class object_rectangle : public object
{
public:
  vec3 LDcorn, RDcorn, LUcorn, normal;

  object_rectangle(vec3 LDcorn, vec3 RDcorn, vec3 LUcorn)
  {
    this->LDcorn = LDcorn;
    this->LUcorn = LUcorn;
    this->RDcorn = RDcorn;

    vec3 e1 = RDcorn - LDcorn, e2 = LUcorn - LDcorn;

    normal = crosMulVec3(e2, e1).normal();
  }

  virtual low_level_object gen_low_level_object(void)
  {
    low_level_object tmp;
    tmp.type = RECT;

    tmp.Arr[0]  = LDcorn.getX();
    tmp.Arr[1]  = LDcorn.getY();
    tmp.Arr[2]  = LDcorn.getZ();

    tmp.Arr[3]  = RDcorn.getX();
    tmp.Arr[4]  = RDcorn.getY();
    tmp.Arr[5]  = RDcorn.getZ();

    tmp.Arr[6]  = LUcorn.getX();
    tmp.Arr[7]  = LUcorn.getY();
    tmp.Arr[8]  = LUcorn.getZ();

    tmp.Arr[9]  = normal.getX();
    tmp.Arr[10] = normal.getY();
    tmp.Arr[11] = normal.getZ();

    return tmp;
  }
};