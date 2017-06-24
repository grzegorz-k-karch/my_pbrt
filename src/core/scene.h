#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "light.h"
#include "primitive.h"
#include "interaction.h"

namespace pbrt {

class Scene {
public:
    // <scene public methods>
    Scene(std::shared_ptr<Primitive> aggregate,
          const std::vector<std::shared_ptr<Light>>& lights)
    : lights(lights), aggregate(aggregate) {
        // <scene constructor implementation>
      worldBound = aggregate->WorldBound();
        for (const auto &light : lights) {
          light->Preprocess(*this);
        }
    }

    bool Intersect(const Ray& ray, SurfaceInteraction* isect) const;
    bool IntersectP(const Ray& ray) const;

    const Bounds3f& WorldBound() const { return worldBound; }
    // <scene public data>
    std::vector<std::shared_ptr<Light>> lights;
private:
    // <scene private data>
    std::shared_ptr<Primitive> aggregate;
    Bounds3f worldBound;
};

}

#endif//SCENE_H
