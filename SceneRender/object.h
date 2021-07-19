#pragma once

enum object_type
{
  SPHERE = 0,
  RECT,
  PLANE,
  BOX,
  TRIANGLE,
  NUM_OF_OBJECT_TYPES
};

struct low_level_object
{
  object_type type;  // 4 byte
  double Arr[16];    // 8 * 16
};

class object
{
  private:
  protected:
    object();
public:
    virtual low_level_object gen_low_level_object(void);
};

#include "object_sphere.h"
