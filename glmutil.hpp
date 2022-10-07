//
// Created by Danny Le on 2022-09-19.
//

#ifndef UNTITLED_GLMUTIL_HPP
#define UNTITLED_GLMUTIL_HPP

#include <glm/glm.hpp>
#include "rtweekend.hpp"

inline static glm::dvec3 randomVec3()
{
    return glm::dvec3(random_double(), random_double(), random_double());
}

inline static glm::dvec3 randomVec3(double min, double max)
{
    return glm::dvec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline glm::dvec3 random_in_unit_sphere() {
    while (true) {
        auto p = randomVec3(-1, 1);
        if (glm::length(p) * glm::length(p) >= 1) continue;
        return p;
    }
}

inline glm::dvec3 random_unit_vector() {
    return glm::normalize(random_in_unit_sphere());
}

inline glm::dvec3 random_in_hemisphere(const glm::dvec3& normal) {
    glm::dvec3 in_unit_sphere = random_in_unit_sphere();
    if (glm::dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline bool near_zero(glm::dvec3 value) {
    const auto s = 1e-8;
    return (fabs(value.x) < s) && (fabs(value.y) < s) && (fabs(value.z) < s);
}

inline glm::dvec3 reflect(const glm::dvec3& v, const glm::dvec3& n) {
    return v - 2*glm::dot(v,n)*n;
}

inline glm::dvec3 refract(const glm::dvec3 uv, const glm::dvec3 n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    glm::dvec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    glm::dvec3 r_out_parallel = -sqrt(fabs(1.0 - (glm::length(r_out_perp) * glm::length(r_out_perp)))) * n;
    return r_out_perp + r_out_parallel;
}

inline glm::dvec3 random_in_unit_disk() {
    while (true) {
        auto p = glm::dvec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (glm::length(p) * glm::length(p) >= 1) continue;
        return p;
    }
}

#endif //UNTITLED_GLMUTIL_HPP
