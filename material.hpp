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
#include "onb.hpp"
#include "pdf.hpp"

struct scatter_record {
    ray specular_ray;
    bool is_specular;
    glm::dvec3 attenuation;
    shared_ptr<pdf> pdf_ptr;
};

class material {
public:
    virtual glm::dvec3 emitted(
            const ray& r_in, const hit_record& rec, double u, double v, const glm::dvec3 &p
            ) const {
        return glm::dvec3{0,0,0};
    }
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const {
        return false;
    };
    virtual double scattering_pdf(
            const ray& r_in, const hit_record& rec, const ray& scattered
            ) const {
        return 0;
    }
};


class lambertian: public material {
public:
    lambertian(const glm::dvec3 &a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
            ) const override {
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
        return true;
    };
    double scattering_pdf(
            const ray& r_in, const hit_record&  rec, const ray& scattered
            ) const override {
        auto cosine = glm::dot(rec.normal, glm::normalize(scattered.direction()));
        return cosine < 0 ? 0 : cosine / pi;
    }

public:
    shared_ptr<texture> albedo;
};

class metal: public material {
public:
    metal(const glm::dvec3& a, double f): albedo(a), fuzz(f < 1 ? f : 1) {};

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
            ) const override {
        glm::dvec3 reflected = reflect(glm::normalize(r_in.direction()), rec.normal);
        srec.specular_ray = ray(rec.p, reflected+fuzz*random_in_unit_sphere());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = 0;
        return true;
    }

public:
    glm::dvec3 albedo;
    double fuzz;
};

class dielectric: public material {
public:
    dielectric(double index_of_refraction): ir(index_of_refraction) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
            ) const override {
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        srec.attenuation = glm::dvec3{1.0, 1.0, 1.0};
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
        srec.specular_ray = ray(rec.p, direction, r_in.time());
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
            const ray& r_in, const hit_record& rec, scatter_record& srec
            ) const override {
        return false;
    }

    virtual glm::dvec3 emitted(const ray& r_in, const hit_record& rec, double u, double v, const glm::dvec3 &p) const override {

        if (rec.front_face)
            return emit->value(u,v,p);
        else
            return glm::dvec3(0,0,0);
    }

public:
    shared_ptr<texture> emit;
};

class isotropic : public material {
public:
    isotropic(glm::dvec3 c) : albedo(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
            ) const override{
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.is_specular = false;
        srec.pdf_ptr = nullptr;
        //scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
        //attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};
#endif //UNTITLED_MATERIAL_HPP
