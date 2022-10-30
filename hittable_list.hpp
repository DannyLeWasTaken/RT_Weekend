//
// Created by Danny Le on 2022-09-17.
//

#ifndef UNTITLED_HITTABLE_LIST_HPP
#define UNTITLED_HITTABLE_LIST_HPP

#include "hittable.hpp"
#include "aabb.hpp"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list: public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    double pdf_value(const glm::dvec3& o, const glm::dvec3& v) const;
    glm::dvec3 random(const glm::dvec3& o) const;

public:
    std::vector<shared_ptr<hittable>> objects;
};

double hittable_list::pdf_value(const glm::dvec3 &o, const glm::dvec3 &v) const {
    auto weight = 1.0/objects.size();
    auto sum = 0.0;

    for (const auto& object: objects)
        sum += weight * object->pdf_value(o ,v);

    return sum;
}

glm::dvec3 hittable_list::random(const glm::dvec3 &o) const {
    auto int_size = static_cast<int>(objects.size());
    return objects[random_int(0, int_size - 1)]->random(o);
}

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object: objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb &output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object: objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }
    return true;
}

#endif //UNTITLED_HITTABLE_LIST_HPP
