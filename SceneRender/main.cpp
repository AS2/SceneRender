#include "core.h"
#include "render_cpu_rt.h"

// TODO
// - all objs (sphere cast)
// - add maters
// - 

int main(void)
{
  core scene = core(new render_cpu_rt(300, 200));

  scene.add(new object_sph(vec3(-6,0,0), 1));

  scene.run();

  getchar();
/*
  
*/
}