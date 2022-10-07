//
// Created by Danny Le on 2022-09-19.
//

#ifndef UNTITLED_CAMERA_HPP
#define UNTITLED_CAMERA_HPP

#include <glm/vec3.hpp>
#include "ray.hpp"

class camera {
public:
    camera(
            glm::dvec3 lookFrom,
            glm::dvec3 lookAt,
            glm::dvec3 vup,
            double vfov,
            double aspect_ratio,
            double aperture,
            double focus_dist,
            double _time0 = 0,
            double _time1 = 0
            ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = glm::normalize(lookFrom - lookAt);
        u = glm::normalize(glm::cross(vup, w));
        v = glm::cross(w, u);

        origin = lookFrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2.0 - vertical/2.0 - focus_dist*w;

        lens_radius = aperture / 2;
        time0 = _time0;
        time1 = _time1;
    }
    ray get_ray(double s, double t) const {
        glm::dvec3 rd = lens_radius * random_in_unit_disk();
        glm::dvec3 offset = u * rd.x + v * rd.y;

        return ray(origin + offset,
                   lower_left_corner + s*horizontal + t*vertical - origin - offset, random_double(time0, time1));
    }

public:
    glm::dvec3 origin;
    glm::dvec3 lower_left_corner;
    glm::dvec3 horizontal;
    glm::dvec3 vertical;
    glm::dvec3 u,v,w;
    double lens_radius;
    double time0, time1; // Shutter open/close times
};


#endif //UNTITLED_CAMERA_HPP
