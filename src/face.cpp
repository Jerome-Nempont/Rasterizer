#include "face.h"

using namespace aline;

Face::Face(uint v0,uint v1,uint v2, const minwin::Color & color){
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
        this->color = color;
    }