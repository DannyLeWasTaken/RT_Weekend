#include <iostream>
#include <glm/glm.hpp>
#include <thread>
#include "ray.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "rtweekend.hpp"
#include "material.hpp"
#include "camera.hpp"
#include "glmutil.hpp"
#include "moving_sphere.hpp"


glm::dvec3 ray_color(const ray& r, const hittable_list& world, int depth) {
    hit_record rec;

    if (depth <= 0) {
        return glm::dvec3(0,0,0);
    }

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        glm::dvec3 attenuation;
        if (rec.mat_ptr->scatter(r,  rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return glm::dvec3{0,0,0};
    }
    glm::dvec3 unit_direction = glm::normalize(r.direction());
    auto t = 0.5f * (unit_direction.y + 1.0);
    return (1.0-t)*glm::dvec3(1.0,1.0,1.0) + t*glm::dvec3(0.5, 0.7, 1.0);
}

hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("C:/Users/Danny Le/CLionProjects/untitled/assets/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(glm::dvec3{0,0,0}, 2, earth_surface);

    return hittable_list(globe);
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(glm::dvec3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(glm::dvec3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(glm::dvec3(0.5,0.5,0.5));

    auto checker = make_shared<checker_texture>(glm::dvec3(0.2, 0.3, 0.1), glm::dvec3(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(glm::dvec3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            glm::dvec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if (glm::length(center - glm::dvec3(4, 0.2, 0)) > 0.9 ) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    glm::dvec3 albedo = randomVec3(0, 1) * randomVec3(0, 1);
                    sphere_material = make_shared<lambertian>(albedo);
                    glm::dvec3 center2 = center +  glm::dvec3(0, random_double(0.0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    glm::dvec3 albedo = randomVec3(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(glm::dvec3(0,1,0), 1.0 ,material1));

    auto material2 = make_shared<lambertian>(glm::dvec3(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(glm::dvec3(-4,1,0), 1.0, material2));

    auto material3 = make_shared<metal>(glm::dvec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(glm::dvec3(4,1,0), 1.0, material3));

    return world;
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(glm::dvec3(0.2, 0.3, 0.1), glm::dvec3(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(glm::dvec3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(glm::dvec3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 480;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 4;
    const int max_depth = 50;

    // World
    hittable_list world;

    glm::dvec3 lookFrom;
    glm::dvec3 lookAt = glm::dvec3(0,0,0);
    double vfov = 40.0;
    double aperture = 0.0;

    switch(4) {
        case 1:
            world = random_scene();
            lookFrom = glm::dvec3(13,2,3);
            lookAt = glm::dvec3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 2:
            world = two_spheres();
            lookFrom = glm::dvec3(13,2,3);
            lookAt = glm::dvec3(0,0,0);
            vfov = 20.0;
            break;
        case 3:
            world = two_perlin_spheres();
            lookFrom = glm::dvec3(13,2,3);
            lookAt = glm::dvec3(0,0,0);
            vfov = 20.0;
            break;
        case 4:
            world = earth();
            lookFrom = glm::dvec3{13,2,3};
            lookAt = glm::dvec3{0,0,0};
            vfov = 20.0;
            break;
        default:
            break;
    }

    // Camera
    glm::dvec3 vup = glm::dvec3(0,1,0);
    double dist_to_focus = 10.0;

    camera cam(lookFrom, lookAt, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    /*
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            glm::vec3 pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                std::thread rayWorker(ray_color, r, world, max_depth);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    */

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {

            glm::dvec3 pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                //std::thread rayWorker(ray_color, r, world, max_depth, (&pixel_color));
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}