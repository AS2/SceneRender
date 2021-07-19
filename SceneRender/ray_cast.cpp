#include <cstdio>

#include "object.h"
#include "core.h"
#include "render_cpu_rt.h"

vec3 render_cpu_rt::get_refl(vec3& vec,   vec3& normal) 
{
  return normal * (2 * vec.vecMul(normal)) - vec;
}

vec3 render_cpu_rt::ray_cast(ray& rayToCast, int currentDepth)
{
  vec3 pixelColor = backgroundColor, collPnt, tmpCollPnt;
  material currentMtr;
  object* collObj = nullptr;
  int collObjIndex = 0, objectsCnt = our_core->objects.size(), nearestCollObj = -1;

  double nearestLength = RAY_INF;

  // check reflection
  for (int collObjIndex = 0; collObjIndex < objectsCnt; collObjIndex++)
    if (ray_cast_obj(our_core->objects[collObjIndex], rayToCast, tmpCollPnt) == 1) {
      if ((tmpCollPnt - rayToCast.pos).length() < nearestLength) {
        nearestLength = (tmpCollPnt - rayToCast.pos).length();
        collObj = our_core->objects[collObjIndex];
        nearestCollObj = collObjIndex;
        collPnt = tmpCollPnt;
      }
    }

  // if reflection -> build secondary ray
  if (collObj != nullptr) {
    currentMtr = our_core->materials[nearestCollObj];
    pixelColor = currentMtr.color * currentMtr.kAmb;

    ray rayToLight;
    if (currentMtr.kRefl != 1) {
      // build sec ray to each light sources
      for (auto& oneLight : our_core->lights) {
        rayToLight = ray(collPnt, (oneLight->pos - collPnt).normal());
        double lightDist = oneLight->DistToLightSource(collPnt);

        // check collisions while ray go to the light source
        nearestLength = RAY_INF;
        vec3 collLightPnt;
        
        for (auto& oneObj : our_core->objects)
          if (ray_cast_obj(oneObj, rayToLight, collLightPnt) == 1 &&
            (collLightPnt - collPnt).length() < nearestLength)
            nearestLength = (collLightPnt - collPnt).length();

        if (nearestLength < lightDist)
          continue;
        // if go to light source -> add to pixel color
        else {
          vec3 lightDir = rayToLight.dir,
            normal = ray_norm_obj(collObj, rayToCast, collPnt),
            Rm = get_refl(lightDir, normal);

          vec3 Idif = (oneLight->color * (currentMtr.kDif * std::max((double)0, lightDir.vecMul(normal)))),
            Ispec = (oneLight->color * (currentMtr.kSpec * pow(std::max((double)0, Rm.vecMul(rayToCast.dir * (-1))), currentMtr.alpha)));

          pixelColor = pixelColor + Idif + Ispec;
        }
      }
    }

    // add reflection component
    if (currentMtr.kRefl != 0 && currentDepth < maxRefDepth) {
      vec3 normToPnt = ray_norm_obj(collObj, rayToCast, collPnt);
      vec3 newRayDir = get_refl(rayToCast.dir, normToPnt) * (-1);
      ray newRay = ray(collPnt, newRayDir);

      pixelColor = pixelColor + ray_cast(newRay, currentDepth + 1) * currentMtr.kRefl;
    }
  }

  return pixelColor;
}

void render_cpu_rt::render_async_cast(ray Ray, int x, int y) 
{
}