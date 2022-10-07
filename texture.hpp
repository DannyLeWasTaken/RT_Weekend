//
// Created by Danny Le on 2022-10-04.
//

#ifndef UNTITLED_TEXTURE_HPP
#define UNTITLED_TEXTURE_HPP

#include <glm/glm.hpp>
#include "rtweekend.hpp"
#include "perlin.hpp"

class texture {
public:
    virtual glm::dvec3 value(double u, double v, const glm::dvec3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color() {}
    solid_color(glm::dvec3 c) : color_value(c) {}

    solid_color(double red, double green, double blue) : solid_color(glm::dvec3(red,green,blue)) {};

    virtual glm::dvec3 value(double u, double v, const glm::dvec3& p) const override {
        return color_value;
    }

private:
    glm::dvec3 color_value;
};

class checker_texture : public texture {
public:
    checker_texture() {}

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd) : even(_even), odd(_odd) {}

    checker_texture(glm::dvec3 c1, glm::dvec3 c2) : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

    virtual glm::dvec3 value(double u, double v, const glm::dvec3& p) const override {
        auto sines = sin(10*p.x)*sin(10*p.y)*sin(10*p.z);
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};

class noise_texture: public texture {
public:
    noise_texture() {}
    noise_texture(double sc) : scale(sc) {}

    virtual glm::dvec3 value(double u, double v, const glm::dvec3& p) const override {
        //return glm::dvec3(1, 1, 1) * noise.turb(scale * p);
        return glm::dvec3(1) * 0.5 * (1 + sin(scale*p.z + 10 * noise.turb(p)));
    }

public:
    perlin noise;
    double scale;
};

#endif //UNTITLED_TEXTURE_HPP
