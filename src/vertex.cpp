#include "vertex.h"
using namespace aline;

Vertex::Vertex(const Vec3r &c, real h): vec(c),intensity(h){}
Vertex::Vertex(): vec({}), intensity(1) {}