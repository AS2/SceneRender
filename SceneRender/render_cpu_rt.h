#pragma once
#include "lib/labengine/labengine.h"
#include "render_base.h"
#include "vec3.h"
#include <vector>

#define RAY_INF 1000
#define RAY_SHIFT 0.001

const vec3 backgroundColor = vec3(0.3, 0.5, 0.7);

class ray {
public:
  vec3 pos, dir;

  ray() {
    pos = vec3(0, 0, 0);
    dir = vec3(0, 0, 0);
  }
  ray(const vec3& startPos, const vec3& dir) {
    this->pos = startPos;
    this->dir = dir;
  }
};

class render_cpu_rt : public render_base
{
  private:
    // TODO
    int W, H;
    std::vector<vec3> Pixels;

    const int SSAA_POW = 3, maxRefDepth = 5;
    vec3 screenCenter, cam;


    int ray_cast_obj(object* obj, ray& ray, vec3& pos);

    int ray_cast_sphere(object_sph* sph, ray& ray, vec3& pos);
    int ray_cast_plane(object_plane* plane, ray& ray, vec3& pos);
    int ray_cast_rectangle(object_rectangle* rect, ray& ray, vec3& pos);
    int ray_cast_box(object_box* box, ray& ray, vec3& pos);


    vec3 ray_norm_obj(object* obj, ray& ray, vec3& pos);

    vec3 ray_norm_sphere(object_sph* sph, ray& ray, vec3& pos);
    vec3 ray_norm_plane(object_plane* plane, ray& ray, vec3& pos);
    vec3 ray_norm_rectangle(object_rectangle* rect, ray& ray, vec3& pos);
    vec3 ray_norm_box(object_box* box, ray& ray, vec3& pos);


    vec3 get_refl(  vec3& vec,   vec3& normal)  ;
    vec3 ray_cast(ray& ray, int currentDepth)  ;
    void render_async_cast(ray Ray, int x, int y);
  public:
    render_cpu_rt(int W, int H)
    {
      this->W = W;
      this->H = H;
      cam = vec3(5.5, 0, 0);
      screenCenter = vec3(0, 0, 0);
      Pixels.resize(W * H * SSAA_POW * SSAA_POW);
    }
    
    virtual void init(void)
    {
      // init labengine
      labparams_t params = { (unsigned int)W, (unsigned int)H, 1 };
      if (LabInitWith(&params))
        return;
      else {

        exit(0);
      }
    }

    virtual void draw(void)
    {
      double scrCoordW = (screenCenter - cam).length(),
             scrCoordH = (screenCenter - cam).length() * H / W,
             pixelSize = scrCoordW / (W * SSAA_POW);

      for (int y = 0; y < H * SSAA_POW; y++)
      {
        for (int x = 0; x < W * SSAA_POW; x++)
        {
          double xCoord = screenCenter.getX(), // screenCenter.getZ() - scrCoordZ / 2 + x * (pixelSize + 1.0 / 2);
                 yCoord = screenCenter.getY() - scrCoordW / 2 + pixelSize * (x + 1.0 / 2),
                 zCoord = screenCenter.getZ() + scrCoordH / 2 - pixelSize * (y + 1.0 / 2);

          vec3 screenPointCrd = vec3(xCoord, yCoord, zCoord);
          ray screenRay = ray(screenPointCrd, (screenPointCrd - cam).normal());

          Pixels[y * (W * SSAA_POW) + x] = ray_cast(screenRay, 0);
          // our_core->objects
         // in pixel do Raycast
        }
      }

      // Draw in LabEngine
      vec3 pixOnScr;
      for (int y = 0; y < H; y++)
      {
        for (int x = 0; x < W; x++)
        {
          pixOnScr = vec3(0, 0, 0);
          for (int ay = 0; ay < SSAA_POW; ay++)
            for (int ax = 0; ax < SSAA_POW; ax++)
              pixOnScr = pixOnScr + Pixels[(y * SSAA_POW + ay) * (W * SSAA_POW) + (x * SSAA_POW + ax)];

          pixOnScr = pixOnScr * (1.0f / (SSAA_POW * SSAA_POW));

          LabSetColorRGB(std::min(255, (int)(pixOnScr.getX() * 255)),
                         std::min(255, (int)(pixOnScr.getY() * 255)),
                         std::min(255, (int)(pixOnScr.getZ() * 255)));
          LabDrawPoint(x, y);
          LabDrawFlush();
        }
      }
    }

    virtual void term(void) {
      LabTerm();
    }
};