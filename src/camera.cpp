#include "camera.h"

using namespace aline;

Camera::Camera(real aspect_ratio, real focal_dist, real distanceMax, real move_speed, real rot_speed, real zoom_speed):
    aspect_ratio(aspect_ratio), focal_dist(focal_dist), distanceMax(distanceMax),
    position({}),
    move_speed(move_speed), rot_speed(rot_speed), zoom_speed(zoom_speed),
    fov(focal_dist, distanceMax, aspect_ratio, focal_dist), qRotation(1, {}) { }

//Fusion du front et backward
void Camera::move(int idAxis, const Vec3r &axis) {
    Vec3r isAxisActive = vector_movement[idAxis];
    if(isAxisActive == Vec3r{}) {
        vector_movement[idAxis] = axis;
    }
}

//rotation clockwise et anticlockwise
void Camera::rotate(int idAxis, const Vec3r &axis) {
    Vec3r isAxisActive = vector_rotation[idAxis];
    if(isAxisActive == Vec3r{}) {
        vector_rotation[idAxis] = axis;
    }
}


void Camera::stop_movement(int idAxis) {
    vector_movement[idAxis] = {};
}

void Camera::stop_rotation(int idAxis) {
    vector_rotation[idAxis] = {};
}

void Camera::update() {
    updateRotation();
    updateTranslation();
}

void Camera::updateRotation() {
    real delta = (rot_speed/2)*PI /180;
    real cos = std::cos(delta);
    real sin = std::sin(delta);

    if(vector_rotation[0] != Vec3r{}) {
        Vec3r img = vector_rotation[0] * Vec3r{sin, 0, 0};
        Quaternion r(cos, img);
        qRotation = qRotation.mult(r);
    }

    if(vector_rotation[1] != Vec3r{}) {
        Vec3r img = vector_rotation[1] * Vec3r{0, sin, 0};
        Quaternion r(cos, img);
        qRotation = qRotation.mult(r);
    }

    if(vector_rotation[2] != Vec3r{}) {
        Vec3r img = vector_rotation[2] * Vec3r{0, 0, sin};
        Quaternion r(cos, img);
        qRotation = qRotation.mult(r);
    }
}

void Camera::updateTranslation() {
    real s = qRotation.s;
    Vec3r v = qRotation.v;
    real n = sq_norm(v);
    Vec3r depl = vector_movement[0] + vector_movement[1] + vector_movement[2];
    real dote = dot(v, depl);
    Vec3r d = ( std::pow(s, 2) - n ) * depl + cross( (2*s*v) , depl) + (2*dote)*v;
    Vec3r t = move_speed * d;
    position += t;
}

Mat44r Camera::transform() const {
    Mat44r matrixR = matrixRotationQuaternion();
    Mat44r matrixTransl = matrixTranslation();
    return matrixR * matrixTransl;
}

Mat44r Camera::matrixTranslation() const {
    return {
        {1, 0, 0, -position[0]},
        {0, 1, 0, -position[1]},
        {0, 0, 1, -position[2]},
        {0, 0, 0, 1}
    };
}

Mat44r Camera::matrixRotationQuaternion() const{
    Quaternion q(qRotation.s, qRotation.v);
    q.v = -q.v;
    return q.transformToMatrix();
}


bool Camera::sees(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    Vec3r p0 = v1.vec;
    Vec3r p1 = v2.vec;
    Vec3r p2 = v3.vec;
    Vec3r e01 = p1 - p0;
    Vec3r e02 = p2 - p0;
    Vec3r normal =  cross(e01, e02) / norm(e01-e02);
    real d = dot(LOOK_IN, normal);
    return d >= 0;
}

Object Camera::cull(const Object &object) {
    std::vector<Vertex> vertices = object.get_vertices();
    std::vector<Face> faces;
    for(const auto face: object.get_faces()) {
        Vertex v0 = vertices[face.v0];
        Vertex v1 = vertices[face.v1];
        Vertex v2 = vertices[face.v2];
        if(sees(v0, v1, v2)) {
            faces.push_back(face);
        }
    }

    Shape shape = Shape(object.shape.get_name(), vertices, faces);
    return Object(shape);
}