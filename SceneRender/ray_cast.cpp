#include "object.h"
#include "core.h"
#include "render_cpu_rt.h"

int render_cpu_rt::ray_cast_obj(object* obj, ray& ray, vec3& pos) {
  switch(obj->gen_low_level_object().type) {
  case SPHERE:
    return ray_cast_sphere(dynamic_cast<object_sph *>(obj), ray, pos);
  default:
    return 0;
  }
}

int render_cpu_rt::ray_cast_sphere(object_sph* sph, ray& ray, vec3& pos) {
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

vec3 render_cpu_rt::get_refl(vec3& vec,   vec3& normal) {
  return normal * (2 * vec.vecMul(normal)) - vec;
}

vec3 render_cpu_rt::ray_cast(ray& rayToCast, int currentDepth)   {
  vec3 pixelColor = backgroundColor, collPnt;
  //material currentMtr;
  object* collObj = nullptr;

  double nearestLength = RAY_INF;

  // check reflection
  for (auto& oneObj : our_core->objects)
    if (ray_cast_obj(oneObj, rayToCast, collPnt) == 1) {
      if ((collPnt - rayToCast.pos).length() < nearestLength)
        collObj = oneObj;
    }

  // if reflection -> build secondary ray
  if (collObj != nullptr) {
    //currentMtr = collObj->GetMaterial(collPnt, rayToCast.GetDir(), rayToCast);
    //pixelColor = currentMtr.GetColor() * currentMtr.GetKoefA();
    pixelColor = vec3(1, 0, 0);
    ray rayToLight;

    //if (currentMtr.GetKoefR() != 1) {
    //  // build sec ray to each light sources
    //  for (auto& oneLight : lights) {
    //    rayToLight = ray(collPnt, (oneLight->GetPos() - collPnt).normal());
    //    double lightDist = oneLight->DistToLightSource(collPnt);

    //    vec3 collLightPnt;
    //    object* collLightObj = nullptr;
    //    std::vector<object*> collLightObjs(objects.size(), nullptr);
    //    std::vector<vec3> collLightPnts(objects.size(), vec3());
    //    currentIndex = 0;

    //    // check collisions while ray go to the light source
    //    for (auto& oneObj : objects)
    //      if (oneObj->CheckCollision(rayToLight, collLightPnt, 1) == 1) {
    //        collLightObjs[currentIndex] = oneObj;
    //        collLightPnts[currentIndex++] = collLightPnt;
    //      }

    //    // find nearest obj
    //    nearestLength = RAY_INF;
    //    collisionsCnt = currentIndex;
    //    for (int i = 0; i < collisionsCnt; i++)
    //      if ((collLightPnts[i] - collPnt).length() < nearestLength)
    //        nearestLength = (collLightPnts[i] - collPnt).length();

    //    if (nearestLength < lightDist)
    //      continue;
    //    // if go to light source -> add to pixel color
    //    else {
    //      vec3 lightDir = rayToLight.GetDir(),
    //        normal = collObj->GetNormalToPoint(collPnt, rayToCast.GetDir(), rayToCast),
    //        Rm = GetRefl(lightDir, normal);

    //      vec3 Idif = (oneLight->GetColor() * (currentMtr.GetKoefD() * std::max((double)0, lightDir.vecMul(normal)))),
    //        Ispec = (oneLight->GetColor() * (currentMtr.GetKoefS() * pow(std::max((double)0, Rm.vecMul(rayToCast.GetDir() * (-1))), currentMtr.GetKoefAlpha())));

    //      pixelColor = pixelColor + Idif + Ispec;
    //    }
    //  }
    //}

    //// add reflection component
    //if (currentMtr.GetKoefR() != 0 && currentDepth < maxRefDepth) {
    //  vec3 newRayDir = GetRefl(rayToCast.GetDir(), collObj->GetNormalToPoint(collPnt, rayToCast.GetDir(), rayToCast)) * (-1);
    //  ray newRay = ray(collPnt, newRayDir);

    //  pixelColor = pixelColor + RayCast(newRay, currentDepth + 1) * currentMtr.GetKoefR();
    //}
  }

  return pixelColor;
}

void render_cpu_rt::render_async_cast(ray Ray, int x, int y) {
}