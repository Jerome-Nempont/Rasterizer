#include <cmath>
#include "frustum.h"

using namespace aline;

Frustum::Frustum(real dist_near, real dist_far, real a, real e):dist_near(dist_near), dist_far(dist_far)  {
    near = {0, 0, -1, -dist_near};
    far = {0, 0, 1, dist_far};
    left = unit_vector(Vec4r {e, 0, -1, 0});
    right = unit_vector(Vec4r {-e, 0, -1, 0});
    bottom = unit_vector(Vec4r {0, e, -a, 0});
    top = unit_vector(Vec4r {0, -e, -a, 0});
}

void Frustum::update(real a, real e) {
    near = {0, 0, -1, -dist_near};
    far = {0, 0, 1, dist_far};
    left = unit_vector(Vec4r {e, 0, -1, 0});
    right = unit_vector(Vec4r {-e, 0, -1, 0});
    bottom = unit_vector(Vec4r {0, e, -a, 0});
    top = unit_vector(Vec4r {0, -e, -a, 0});
}

Vec4r Frustum::intersection(const Vec4r &point, const Vec4r &direction, const Vec4r &plan) {
    real iV = dot(plan, direction);
    real iS = dot(plan, point);

    real t = -(iS/iV);

    return point + t * direction;
}

Object Frustum::clippingCut(const Object &object, const Vec4r &plan) {
    std::vector<Face> newFaces;
    std::vector<Vertex> newVertices(object.get_vertices());

    for(const auto face: object.get_faces()) {
        Vertex v1 = newVertices[face.v0];
        Vertex v2 = newVertices[face.v1];
        Vertex v3 = newVertices[face.v2];

        Vec3r p1 = v1.vec;
        Vec3r p2 = v2.vec;
        Vec3r p3 = v3.vec;

        real distP1 = dot(p1, {plan[0], plan[1], plan[2]}) + plan[3];
        real distP2 = dot(p2, {plan[0], plan[1], plan[2]}) + plan[3];
        real distP3 = dot(p3, {plan[0], plan[1], plan[2]}) + plan[3];

        int isIn[3] = {0, 0, 0};

        if(distP1 >= 0) isIn[0] = 1;
        if(distP2 >= 0) isIn[1] = 1;
        if(distP3 >= 0) isIn[2] = 1;

        int sum = isIn[0] + isIn[1] + isIn[2];

        if(sum == 3) newFaces.push_back(face);
        else if(sum == 0) continue;
        else {
            Vec3r a = p1;
            Vec3r b = p2;
            Vec3r c = p3;

            if(sum == 1) {
                if(isIn[0] == 1) {
                    Vec4r prime1 = intersection({a[0], a[1], a[2], 1}, {b[0], b[1], b[2], 0}, plan);
                    Vec4r prime2 = intersection({a[0], a[1], a[2], 1}, {c[0], c[1], c[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v2.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v3.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(face.v0, size-2, size-1, face.color));
                }

                else if(isIn[1] == 1) {
                    Vec4r prime1 = intersection({b[0], b[1], b[2], 1}, {a[0], a[1], a[2], 0}, plan);
                    Vec4r prime2 = intersection({b[0], b[1], b[2], 1}, {c[0], c[1], c[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v1.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v3.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(size-2, face.v1, size-1, face.color));
                }

                else {
                    Vec4r prime1 = intersection({c[0], c[1], c[2], 1}, {a[0], a[1], a[2], 0}, plan);
                    Vec4r prime2 = intersection({c[0], c[1], c[2], 1}, {b[0], b[1], b[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v1.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v2.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(size-2, size-1, face.v2, face.color));
                }
            }

            else {
                if(isIn[0] == 0) {
                    Vec4r prime1 = intersection({b[0], b[1], b[2], 1}, {a[0], a[1], a[2], 0}, plan);
                    Vec4r prime2 = intersection({c[0], c[1], c[2], 1}, {a[0], a[1], a[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v1.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v1.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(face.v1, face.v2, size-2, face.color));
                    newFaces.push_back(Face(size-2, face.v2, size-1, face.color));
                }

                else if(isIn[1] == 0) {
                    Vec4r prime1 = intersection({a[0], a[1], a[2], 1}, {b[0], b[1], b[2], 0}, plan);
                    Vec4r prime2 = intersection({c[0], c[1], c[2], 1}, {b[0], b[1], b[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v2.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v2.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(face.v0, face.v2, size-2, face.color));
                    newFaces.push_back(Face(size-2, face.v2, size-1, face.color));
                }

                else {
                    Vec4r prime1 = intersection({a[0], a[1], a[2], 1}, {c[0], c[1], c[2], 0}, plan);
                    Vec4r prime2 = intersection({b[0], b[1], b[2], 1}, {c[0], c[1], c[2], 0}, plan);

                    newVertices.push_back(Vertex({prime1[0], prime1[1], prime1[2]}, v3.intensity));
                    newVertices.push_back(Vertex({prime2[0], prime2[1], prime2[2]}, v3.intensity));

                    uint size = newVertices.size();

                    newFaces.push_back(Face(face.v0, face.v1, size-2, face.color));
                    newFaces.push_back(Face(size-2, face.v1, size-1, face.color));
                }
            }
        }
    }


    Shape s(object.shape.get_name(), newVertices, newFaces);

    return Object(s);
}

Object Frustum::clip(const Object& object) {
    real rayon = object.rayon;
    Vec3r centre = object.barycentre;

    real dist_Near = dot(centre, {near[0], near[1], near[2]}) + near[3];
    real dist_Far = dot(centre, {far[0], far[1], far[2]}) + far[3];
    real dist_left = dot(centre, {left[0], left[1], left[2]}) + left[3];
    real dist_Right = dot(centre, {right[0], right[1], right[2]}) + right[3];
    real dist_Bot = dot(centre, {bottom[0], bottom[1], bottom[2]}) + bottom[3];
    real dist_Top = dot(centre, {top[0], top[1], top[2]}) + top[3];


    if(dist_Near < -rayon) return Object(Shape("", {}, {}));
    if(dist_Far < -rayon) return Object(Shape("", {}, {}));
    if(dist_left < -rayon) return Object(Shape("", {}, {}));
    if(dist_Right < -rayon) return Object(Shape("", {}, {}));
    if(dist_Top < -rayon) return Object(Shape("", {}, {}));
    if(dist_Bot < -rayon) return Object(Shape("", {}, {}));

    Object o(object);

    if(dist_Near < rayon) o = clippingCut(o, near);
    if(dist_Far < rayon) o = clippingCut(o, far);
    if(dist_left < rayon) o = clippingCut(o, left);
    if(dist_Right < rayon) o = clippingCut(o, right);
    if(dist_Top < rayon) o = clippingCut(o, top);
    if(dist_Bot < rayon) o = clippingCut(o, bottom);

    return o;
}