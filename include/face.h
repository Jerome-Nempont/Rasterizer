#pragma once
#include "vector.h"
#include "color.h"

namespace aline {

class Face {
public:
    uint v0;
    uint v1;
    uint v2;
    minwin::Color color;

    Face(uint v1, uint v2, uint v3, const minwin::Color &color);
};

}