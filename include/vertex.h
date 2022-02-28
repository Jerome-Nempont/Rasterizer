#pragma once
#include "vector.h"

namespace aline {

class Vertex {
public:
    Vec3r vec;
    real intensity;
    Vertex();
    Vertex(const Vec3r &c, real h);
};
}