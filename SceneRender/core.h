#pragma once
#include <vector>
#include "render_base.h"
#include "object.h"

class core
{
  private:
    render_base *current_renderer;
    // input
    // timer

  public:
    std::vector<object *> objects;

    core(render_base * renderer)
    {
      current_renderer = renderer;
      renderer->SetCore(this);
      current_renderer->init();
    }

    void add(object *Obj) 
    {
      objects.push_back(Obj);
    }

    void run(void)
    {
      current_renderer->draw();
    }
};