#ifndef CORE_MEDIUM_H
#define CORE_MEDIUM_H

namespace pbrt {

class Medium {

};

struct MediumInterface {
  MediumInterface()
  : inside(nullptr), outside(nullptr) {}
  const Medium *inside, *outside;
};

} // namespace pbrt

#endif//CORE_MEDIUM_H
