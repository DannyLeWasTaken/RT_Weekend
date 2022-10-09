//
// Created by Danny Le on 2022-10-08.
//

#ifndef UNTITLED_BOX_HPP
#define UNTITLED_BOX_HPP

#include "rtweekend.hpp"
#include "aarect.hpp"
#include "hittable_list.hpp"

class box : public hittable {
public:
    box() {}
    box(const glm::dvec3& p0, const glm::dvec3& p1, shared_ptr<material> ptr);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = aabb(box_min, box_max);
        return true;
    }

public:
    glm::dvec3 box_min;
    glm::dvec3 box_max;
    hittable_list sides;
};

box::box(const glm::dvec3& p0, const glm::dvec3& p1, shared_ptr<material>ptr) {
    box_max = p1;
    box_min = p0;

    sides.add(make_shared<xy_rect>(p0.x, p1.x, p0.y, p1.y, p1.z, ptr));
    sides.add(make_shared<xy_rect>(p0.x, p1.x, p0.y, p1.y, p0.z, ptr));

    sides.add(make_shared<xz_rect>(p0.x, p1.x, p0.z, p1.z, p1.y, ptr));
    sides.add(make_shared<xz_rect>(p0.x, p1.x, p0.z, p1.z, p0.y, ptr));

    sides.add(make_shared<yz_rect>(p0.y, p1.y, p0.z, p1.z, p1.x, ptr));
    sides.add(make_shared<yz_rect>(p0.y, p1.y, p0.z, p1.z, p0.x, ptr));
}

bool box::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    return sides.hit(r, t_min, t_max, rec);
}


#endif //UNTITLED_BOX_HPP
