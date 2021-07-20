#include "core.h"
#include "render_cpu_rt.h"

static double cosVec3(const vec3& v1, const vec3& v2)
{
  return (v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ()) / (v1.length() * v2.length());
}

vec3 render_cpu_rt::ray_norm_obj(object* obj, ray& ray, vec3& pos)
{
  switch (obj->gen_low_level_object().type) {
  case SPHERE:
    return ray_norm_sphere(dynamic_cast<object_sph*>(obj), ray, pos);
  case PLANE:
    return ray_norm_plane(dynamic_cast<object_plane*>(obj), ray, pos);
  case RECTANGLE:
    return ray_norm_rectangle(dynamic_cast<object_rectangle*>(obj), ray, pos);
  case BOX:
    return ray_norm_box(dynamic_cast<object_box*>(obj), ray, pos);
  default:
    return vec3(1, 0, 0);
  }
}

vec3 render_cpu_rt::ray_norm_sphere(object_sph* sph, ray& ray, vec3& pos)
{
  return (pos - sph->Pos).normal();
}

vec3 render_cpu_rt::ray_norm_plane(object_plane* plane, ray& ray, vec3& pos)
{
  if (cosVec3(plane->normal, ray.dir) > 0)
    return plane->normal.normal() * (-1);
  else
    return plane->normal.normal();
}

vec3 render_cpu_rt::ray_norm_rectangle(object_rectangle* rect, ray& ray, vec3& pos)
{
  if (cosVec3(rect->normal, ray.dir) > 0)
    return rect->normal.normal() * (-1);
  else
    return rect->normal.normal();
}

vec3 render_cpu_rt::ray_norm_box(object_box* box, ray& ray, vec3& pos)
{
  vec3 dot5 = box->dot4 + (box->dot2 - box->dot1),
       dot6 = box->dot4 + (box->dot3 - box->dot1),
       dot7 = dot6 + (box->dot2 - box->dot1), vecColl;

  object_rectangle p1 = object_rectangle(box->dot1, box->dot2, box->dot4),
                   p2 = object_rectangle(box->dot1, box->dot3, box->dot4),
                   p3 = object_rectangle(box->dot1, box->dot3, box->dot2),
                   p4 = object_rectangle(box->dot4, dot6, dot5),
                   p5 = object_rectangle(dot5, box->dot2, dot7),
                   p6 = object_rectangle(dot6, box->dot3, dot7),
                   nearestPlane = p1;

  double minDist = RAY_INF;
  vec3 colDot;

  if (ray_cast_rectangle(&p1, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestPlane = p1;
  }
  if (ray_cast_rectangle(&p2, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestPlane = p2;
  }
  if (ray_cast_rectangle(&p3, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestPlane = p3;
  }
  if (ray_cast_rectangle(&p4, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestPlane = p4;
  }
  if (ray_cast_rectangle(&p5, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestPlane = p5;
  }
  if (ray_cast_rectangle(&p6, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestPlane = p6;
  }

  if (minDist == RAY_INF)
    return vec3(1, 0, 0);
  else 
    return ray_norm_rectangle(&nearestPlane, ray, pos);
}
