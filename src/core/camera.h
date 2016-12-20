#ifndef CAMERA_H
#define CAMERA_H

#include "film.h"

namespace pbrt {

class Camera {

public:
    Film *film;
};

}

#endif//CAMERA_H
