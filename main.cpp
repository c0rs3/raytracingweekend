#include "lib/rtweekend.h"

#include "lib/camera.h"
#include "lib/hittable.h"
#include "lib/hittable_list.h"
#include "lib/sphere.h"
#include "lib/material.h"

int main() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1000;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 10;

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,0);
    cam.lookat   = point3(1,1,1);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);
}