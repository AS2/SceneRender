#pragma once
#include <vector>
#include "render_base.h"

#include "object.h"
#include "material.h"
#include "light.h"

class core
{
  private:
    render_base *current_renderer;
    // input
    // timer

  public:
    std::vector<object *> objects;
    std::vector<material> materials;
    std::vector<light *> lights;

    core(render_base * renderer)
    {
      current_renderer = renderer;
      renderer->SetCore(this);
      current_renderer->init();
    }

    void addObj(object *Obj, material &newMater) 
    {
      objects.push_back(Obj);
      materials.push_back(newMater);
    }

    void addLight(light *newLight)
    {
      lights.push_back(newLight);
    }

    void run(void)
    {
      current_renderer->draw();
    }
};