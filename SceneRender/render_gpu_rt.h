#pragma once
#include "lib/labengine/labengine.h"
#include "render_base.h"
#include "vec3.h"
#include <vector>
#include <algorithm>

class render_gpu_rt : public render_base
{
private:
  // TODO
  int W, H;
  std::vector<vec3> Pixels;

  const int SSAA_POW = 3, maxRefDepth = 5;
  vec3 screenCenter, cam;

  void load_shader(void);  
  void gen_screen_triangle(void);
  void send_objects(void);

  // important shit
  unsigned int vertexBuffer;
  unsigned int shaderProgramId;
public:
  friend void display(void);

  render_gpu_rt(int W, int H)
  {
    this->W = W;
    this->H = H;
    cam = vec3(5.5, 0, 0);
    screenCenter = vec3(0, 0, 0);
    Pixels.resize(W * H * SSAA_POW * SSAA_POW);
  }

  virtual void init(void);

  virtual void draw(void);

  virtual void term(void);
};