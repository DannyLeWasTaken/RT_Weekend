//
// Created by Danny Le on 2022-09-17.
//

#ifndef UNTITLED_HITTABLE_HPP
#define UNTITLED_HITTABLE_HPP

//#include <glm/glm.hpp>
#include "ray.hpp"
#include "rtweekend.hpp"
#include "aabb.hpp"

class material;

struct hit_record {
    glm::dvec3 p;
    glm::dvec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    double u;
    double v;
    bool front_face;

    inline void set_face_normal(const ray& r, const glm::dvec3& outward_normal) {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};


#endif //UNTITLED_HITTABLE_HPP
