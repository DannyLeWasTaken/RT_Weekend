//
// Created by Danny Le on 2022-09-20.
//

#ifndef UNTITLED_MATERIAL_HPP
#define UNTITLED_MATERIAL_HPP

#include "rtweekend.hpp"
#include "ray.hpp"
#include "glmutil.hpp"
#include "hittable.hpp"
#include <glm/glm.hpp>
#include "texture.hpp"

class material {
public:
    virtual glm::dvec3 emitted(double u, double v, const glm::dvec3 &p) const {
        return glm::dvec3{0,0,0};
    }
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, glm::dvec3& attenuation, ray& scattered
    ) const = 0;
};


class lambertian: public material {
public:
    lambertian(const glm::dvec3 &a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, glm::dvec3 &attenuation, ray &scattered
            ) const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (near_zero(scatter_direction))
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    };
public:
    shared_ptr<texture> albedo;
};

class metal: public material {
public:
    metal(const glm::dvec3& a, double f): albedo(a), fuzz(f < 1 ? f : 1) {};

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, glm::dvec3& attenuation, ray& scattered
            ) const override {
        glm::dvec3 reflected = reflect(glm::normalize(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (glm::dot(scattered.direction(), rec.normal) > 0);
    }

public:
    glm::dvec3 albedo;
    double fuzz;
};

class dielectric: public material {
public:
    dielectric(double index_of_refraction): ir(index_of_refraction) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, glm::dvec3& attenuation, ray& scattered
            ) const override {
        attenuation = glm::dvec3(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        glm::dvec3 unit_direction = glm::normalize(r_in.direction());
        //glm::dvec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        glm::vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            // must reflect
            direction = reflect(unit_direction, rec.normal);
        } else {
            // can refract
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }
        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }

public:
    double ir;

private:
    static double reflectance(double consine, double ref_idx) {
        auto r0 = (1-ref_idx) / (1 + ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0) * pow((1 - consine), 5);
    }
};

class diffuse_light : public material {
public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {};
    diffuse_light(glm::dvec3 c) : emit(make_shared<solid_color>(c)) {}

    virtual bool scatter (
            const ray& r_in, const hit_record& rec, glm::dvec3& attenuation, ray& scattered
            ) const override {
        return false;
    }

    virtual glm::dvec3 emitted(double u, double v, const glm::dvec3 &p) const override {
        return emit->value(u,v,p);
    }

public:
    shared_ptr<texture> emit;
};

class isotropic : public material {
public:
    isotropic(glm::dvec3 c) : albedo(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, glm::dvec3& attenuation, ray& scattered
            ) const override {
        scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};

#endif //UNTITLED_MATERIAL_HPP
