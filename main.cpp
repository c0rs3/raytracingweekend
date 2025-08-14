// #define USE_SIMD

#include "lib/rtweekend.h"

#include "lib/bvh.h"
#include "lib/camera.h"
#include "lib/hittable.h"
#include "lib/hittable_list.h"
#include "lib/sphere.h"
#include "lib/material.h"

#define _RENDER_TEST 0
#define _OTHER 0

int main() {
#if _RENDER_TEST
	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 800;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.vfov = 20;
	cam.lookfrom = point3(13, 2, 3);
	cam.lookat = point3(0, 0, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;

	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_float_xorshift();
			point3 center(a + 0.9 * random_float_xorshift(), 0.2, b + 0.9 * random_float_xorshift());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					auto center2 = center + vec3(0, random_float_xorshift(0, 0.5), 0);
					world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_float_xorshift(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	world = hittable_list(make_shared<bvh_node>(world));

	cam.threaded_render(world);
#elif !(_RENDER_TEST) && !(_OTHER)
	camera cam;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 800;
	cam.samples_per_pixel = 500;
	cam.max_depth = 50;

	cam.vfov = 20;
	cam.lookfrom = point3(13, 2, 3);
	cam.lookat = point3(0, 0, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0.6f;
	cam.focus_dist = 10.0f;

	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5f, 0.5f, 0.5f));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	auto material1 = make_shared<dielectric>(1.5f);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0f, material1));

	auto material2 = make_shared<lambertian>(color(0.4f, 0.2f, 0.1f));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0f, material2));

	auto material3 = make_shared<metal>(color(0.7f, 0.6f, 0.5f), 0.0f);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0f, material3));

	world = hittable_list(make_shared<bvh_node>(world));

	cam.render(world);
	cam.threaded_render(world);
#else
	// test_svec3();
#endif
}
