#include "core.h"
#include "render_cpu_rt.h"

static double cosVec3(const vec3& v1, const vec3& v2)
{
  return (v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ()) / (v1.length() * v2.length());
}

int render_cpu_rt::ray_cast_obj(object* obj, ray& ray, vec3& pos)
{
  switch (obj->gen_low_level_object().type) {
  case SPHERE:
    return ray_cast_sphere(dynamic_cast<object_sph*>(obj), ray, pos);
  case PLANE:
    return ray_cast_plane(dynamic_cast<object_plane*>(obj), ray, pos);
  case RECT:
    return ray_cast_rectangle(dynamic_cast<object_rectangle*>(obj), ray, pos);
  case BOX:
    return ray_cast_box(dynamic_cast<object_box*>(obj), ray, pos);
  default:
    return 0;
  }
}

int render_cpu_rt::ray_cast_sphere(object_sph* sph, ray& ray, vec3& pos)
{
  vec3 l = sph->Pos - ray.pos;
  double t1 = l * ray.dir;

  if (t1 <= 0)
    return 0;
  else {
    double d = l * l - t1 * t1;
    if (d > sph->Radius * sph->Radius)
      return 0;

    double t2 = sqrt(sph->Radius * sph->Radius - d), t = t1 - t2;
    if (t > RAY_INF)
      return 0;

    vec3 cD = ray.pos + ray.dir * t;
    pos = sph->Pos + (cD - sph->Pos) * (1 + RAY_SHIFT);
    return 1;
  }
}

int render_cpu_rt::ray_cast_plane(object_plane* plane, ray& ray, vec3& pos)
{
  if (ray.dir * plane->normal == 0)
    return 0;

  double d = (-1) * (plane->normal * plane->center),
    t = -(ray.pos * plane->normal + d) / (ray.dir * plane->normal);

  if (t <= 0 || t > RAY_INF)
    return 0;
  pos = ray.pos + ray.dir * (t - RAY_SHIFT);
  return 1;
}

int render_cpu_rt::ray_cast_rectangle(object_rectangle* rect, ray& ray, vec3& pos)
{
  // find collision dot with plane
  if (ray.dir * rect->normal == 0)
    return 0;

  double d = (-1) * (rect->normal * rect->LDcorn),
    t = -(ray.pos * rect->normal + d) / (ray.dir * rect->normal);

  if (t <= 0 || t > RAY_INF)
    return 0;

  // check if we are in plane piece
  vec3 dotOnPlane = ray.pos + ray.dir * t,
    newVec = (dotOnPlane - rect->LDcorn), v1 = (rect->RDcorn - rect->LDcorn), v2 = (rect->LUcorn - rect->LDcorn),
    xPart = v2.normal() * newVec.length() * cosVec3(newVec, v2),
    yPart = v1.normal() * newVec.length() * cosVec3(newVec, v1);

  double alpha, beta;

  if (v2.getX() != 0)
    alpha = xPart.getX() / v2.getX();
  else if (v2.getY() != 0)
    alpha = xPart.getY() / v2.getY();
  else
    alpha = xPart.getZ() / v2.getZ();

  if (v1.getX() != 0)
    beta = yPart.getX() / v1.getX();
  else if (v1.getY() != 0)
    beta = yPart.getY() / v1.getY();
  else
    beta = yPart.getZ() / v1.getZ();

  if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1) {
    pos = ray.pos + ray.dir * (t - RAY_SHIFT);
    if ((pos - rect->LDcorn) * (ray.pos - rect->LDcorn) < 0)
      pos = ray.pos + ray.dir * (t + RAY_SHIFT);
    return 1;
  }
  else
    return 0;
}

int render_cpu_rt::ray_cast_box(object_box* box, ray& ray, vec3& pos)
{
  vec3 dot5 = box->dot4 + (box->dot2 - box->dot1),
    dot6 = box->dot4 + (box->dot3 - box->dot1),
    dot7 = dot6 + (box->dot2 - box->dot1), vecColl;

  object_rectangle p1 = object_rectangle(box->dot1, box->dot2, box->dot4),
    p2 = object_rectangle(box->dot1, box->dot3, box->dot4),
    p3 = object_rectangle(box->dot1, box->dot3, box->dot2),
    p4 = object_rectangle(box->dot4, dot6, dot5),
    p5 = object_rectangle(dot5, box->dot2, dot7),
    p6 = object_rectangle(dot6, box->dot3, dot7);

  double minDist = RAY_INF;
  vec3 colDot, nearestColDot;

  if (ray_cast_rectangle(&p1, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestColDot = colDot;
  }
  if (ray_cast_rectangle(&p2, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestColDot = colDot;
  }
  if (ray_cast_rectangle(&p3, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestColDot = colDot;
  }
  if (ray_cast_rectangle(&p4, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestColDot = colDot;
  }
  if (ray_cast_rectangle(&p5, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestColDot = colDot;
  }
  if (ray_cast_rectangle(&p6, ray, colDot) == 1 && (colDot - ray.pos).length() < minDist) {
    minDist = (colDot - ray.pos).length();
    nearestColDot = colDot;
  }

  if (minDist == RAY_INF)
    return 0;
  else {
    pos = nearestColDot;
    return 1;
  }
}