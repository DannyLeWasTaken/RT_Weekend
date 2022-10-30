//
// Created by Danny Le on 2022-10-28.
//

#ifndef UNTITLED_ONB_HPP
#define UNTITLED_ONB_HPP

#include <glm/glm.hpp>

class onb {
public:
    onb() {}

    inline glm::dvec3 operator[](int i) const {return axis[i];}

    glm::dvec3 u() const { return axis[0]; }
    glm::dvec3 v() const { return axis[1]; }
    glm::dvec3 w() const { return axis[2]; }

    glm::dvec3 local(double a, double b, double c) const {
        return a*u() + b*v() + c*w();
    }

    glm::dvec3 local(const glm::dvec3& a) const {
        return a.x*u() + a.y*v() + a.z*w();
    }

    void build_from_w(const glm::dvec3&);

public:
    glm::dvec3 axis[3];
};

void onb::build_from_w(const glm::dvec3 &n) {
    axis[2] = glm::normalize(n);
    glm::dvec3 a = (fabs(w().x) > 0.9) ? glm::dvec3(0, 1, 0) : glm::dvec3(1, 0, 0);
    axis[1] = glm::normalize(glm::cross(w(), a));
    axis[0] = glm::cross(w(), v());
}


#endif //UNTITLED_ONB_HPP
