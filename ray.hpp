//
// Created by Danny Le on 2022-09-17.
//

#ifndef UNTITLED_RAY_HPP
#define UNTITLED_RAY_HPP

#include <glm/glm.hpp>


class ray {
public:
    ray() {};
    ray(const glm::dvec3&origin, const glm::dvec3& direction, double time = 0.0): orig(origin), dir(direction), tm(time) {}

    glm::dvec3 origin() const { return orig; }
    glm::dvec3 direction() const { return dir; }
    double time() const { return tm; }

    glm::dvec3 at(double t) const {
        return (orig + dir*t);
    }

public:
    glm::dvec3 orig;
    glm::dvec3 dir;
    double tm;
};


#endif //UNTITLED_RAY_HPP
