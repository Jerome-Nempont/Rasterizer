#pragma once
#include "vector.h"
#include "matrix.h"
#include "object.h"
#include "frustum.h"
#include "quaternion.h"

namespace aline {

class Camera {
private:
    const Vec3r LOOK_IN = {0, 0, -1};
    Mat44r matrixRotationQuaternion() const;
    Mat44r matrixTranslation() const;
    void updateRotation();
    void updateTranslation();
    bool sees(const Vertex& v1, const Vertex& v2, const Vertex& v3);

public:
    real aspect_ratio;
    real focal_dist;
    real distanceMax;
    Vec3r position;
    Vec3r rotation;
    real move_speed;
    real rot_speed;
    real zoom_speed;
    Frustum fov;
    Quaternion qRotation;
    Vec3r vector_movement[3];
    Vec3r vector_rotation[3];

    Camera(real aspect_ratio, real focal_dist = 2, real distanceMax = 100, real move_speed = 0.5, real rot_speed = 0.5, real zoom_speed = 0.0625);

    Mat44r transform() const;
    void move(int idAxis, const Vec3r &axis);
    void rotate(int idAxis, const Vec3r &axis);
    void stop_movement(int axis);
    void stop_rotation(int axis);
    void update();
    Object cull(const Object &object);
};

}