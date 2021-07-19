#include "core.h"
#include "render_cpu_rt.h"

int main(void)
{

  core scene = core(new render_cpu_rt(300, 200));

  scene.objects.push_back(new object_sph(vec3(1,1,1), 1));

  scene.run();
/*
  
*/
}