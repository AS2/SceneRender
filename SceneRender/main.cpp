#include "core.h"
#include "render_cpu_rt.h"

// TODO
// - add lights

material materials[] = {
  material(vec3(0, 1, 0), 0.3, 0.9, 0.4, 127, 0.1),                   // green glass - 0
  material(vec3(1, 1, 0), 0.35, 0.5, 0.1, 128, 0.25),                 // gold - 1
  material(vec3(0.25, 0.25, 1), 0.5, 0.1, 0.5, 5, 0),                 // blue plastic - 2
  material(vec3(1, 0.25, 0.25), 0.5, 0.1, 0.5, 5, 0),                 // red plastic - 3
  material(vec3(1, 1, 1), 0.05, 0.5, 0, 128, 0.8),                    // mirror - 4
  material(vec3(0.9, 0.9, 0.9), 0.5, 0.2, 0.3, 128, 0.13),            // ceramic - 5
  material(vec3(0.6, 0.6, 0.6), 0.05, 0.5, 0, 128, 0.7),              // dark mirror - 6
  material(vec3(0.98, 0.988, 0.651), 0.5, 0.1, 0.4, 128, 0.34),       // yellow wood - 7
  material(vec3(0.345, 0.196, 0.035), 0.5, 0.1, 0.4, 128, 0.34),      // dark wood - 8
  material(vec3(0.78, 0.78, 0.78), 0.5, 0.1, 0.5, 5, 0),              // grey plastic - 9
  material(vec3(0, 1, 1), 0.3, 0.9, 0.4, 127, 0.2),                   // 10
  material(vec3(1, 0, 1), 0.3, 0.9, 0.4, 127, 0.3),                   // 11
  material(vec3(0.2, 0.2, 0.2), 0.5, 0.5, 0.6, 127, 0.1),             // 12

};

int main(void)
{
  core scene = core(new render_cpu_rt(640, 320));

  // OBJS
  scene.addObj(new object_sph(vec3(-6,0,0), 1), materials[0]);
  scene.addObj(new object_sph(vec3(-8, -1, 1.2), 0.9), materials[10]);
  scene.addObj(new object_sph(vec3(-9, 1, 1.2), 0.9), materials[6]);
  scene.addObj(new object_sph(vec3(-6, 1, -0.5), 0.6), materials[11]);
  scene.addObj(new object_sph(vec3(-3, -2, 1.4), 0.3), materials[12]);

  // left wall
  scene.addObj(new object_plane(vec3(0, 1, 0).normal(), vec3(0, -10, 0)), materials[2]);
  // right wall
  scene.addObj(new object_plane(vec3(0, -1, 0).normal(), vec3(0, 10, 0)), materials[3]);
  // celling
  scene.addObj(new object_plane(vec3(0, 0, -1).normal(), vec3(-6, 0, 4)), materials[5]);
  // front mirror
  scene.addObj(new object_plane(vec3(1, 0, 0).normal(), vec3(-18, 0, 0)), materials[6]);
  // back mirror
  scene.addObj(new object_plane(vec3(1, 0, 0).normal(), vec3(2, 0, 0)), materials[6]);
  // floor
  scene.addObj(new object_rectangle(vec3(2, -10, -2.25), vec3(2, 10, -2.25), vec3(-18, -10, -2.25)), materials[5]);

  scene.addObj(new object_box(vec3(-7, -3, -2.25), vec3(-6, -4, -2.25), vec3(-6, -2, -2.25), vec3(-7, -3, -0.8357)), materials[1]);

  // LIGHTS
  scene.addLight(new light(vec3(-3, -4, 3), vec3(1, 1, 1)));

  scene.addLight(new light(vec3(-1, 0, 5), vec3(1, 1, 1)));

  scene.addLight(new light(vec3(-3, 3, 2), vec3(0.4, 0.4, 1)));

  scene.addLight(new light(vec3(1.8, -6, 6), vec3(0.8, 0.3, 0.5)));

  scene.run();

  getchar();
}