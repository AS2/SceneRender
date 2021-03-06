#pragma once

#include "vec3.h"

enum object_type
{
  SPHERE = 0,
  RECTANGLE,
  PLANE,
  BOX,
  TRIANGLE,
  NUM_OF_OBJECT_TYPES
};

struct low_level_object
{
  float Arr[15]= {0};    // 8 * 16
  int type = NUM_OF_OBJECT_TYPES;  // 4 byte
};

class object
{
  private:
  protected:
    object() {}
  public:
    virtual low_level_object gen_low_level_object(void) { return low_level_object();};
};

#include "object_sphere.h"
#include "object_plane.h"
#include "object_box.h"
#include "object_rectangle.h"
