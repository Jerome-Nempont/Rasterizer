#pragma once
#include "vector.h"
#include "object.h"

#define ALPHA 60
#define PI 3.141592

namespace aline {

class Frustum {
private:
    Object clippingCut(const Object &object, const Vec4r &plan);
    Vec4r intersection(const Vec4r &point, const Vec4r &direction, const Vec4r &plan);
public:
    Vec4r near;
    Vec4r far;
    Vec4r left;
    Vec4r right;
    Vec4r bottom;
    Vec4r top;

    real dist_near;
    real dist_far;

    Frustum(real dist_near, real dist_far, real a, real e);
    void update(real a, real e);
    Object clip(const Object &object);
};

}