#pragma once
#include "render_base.h"
#include <vector>

class render_cpu_rt : public render_base
{
  private:
    // TODO
    int W, H;
    std::vector<int > Pixels;
  public:
    render_cpu_rt(int W, int H)
    {
      this->W = W;
      this->H = H;
      Pixels.resize(W * H);
    }
    
    virtual void init(void)
    {
      // init labengine
    }

    //vec3 RayCast(ray Ray, int depth);

    virtual void draw(void)
    {      
      for (int y = 0; y < H; y++)
      {
        for (int x = 0; x < W; x++)
        {
          // our_core->objects
         // in pixel do Raycast
        }
      }

      // Draw in LabEngine

    }
};