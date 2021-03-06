#include "scene.h"
#include "interaction.h"

namespace pbrt {

bool Scene::Intersect(const Ray& ray, SurfaceInteraction* isect) const {
    return aggregate->Intersect(ray, isect);
}

bool Scene::IntersectP(const Ray& ray) const {
    return aggregate->IntersectP(ray);
}

}
