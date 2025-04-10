#ifndef CAMERA_H
#define CAMERA_H

#define LOG_FILE_NAME "logged.txt"

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"
#include "../benchmark.h"
#include "bmp.h"
#include "../log.h"

const uint16_t THREAD_COUNT = std::thread::hardware_concurrency();

class camera {
public:
    float aspect_ratio = 1.0f;         // ratio of image width over height
    int    image_width = 100;         // rendered image width in pixel count
    int    samples_per_pixel = 10;     // count of random samples for each pixel
    int    max_depth = 10;     // maximum number of ray bounces into scene

    point3 lookfrom = point3(0, 0, 0);   // point camera is looking from
    point3 lookat = point3(0, 0, -1);  // point camera is looking at
    vec3   vup = vec3(0, 1, 0);     // camera-relative "up" direction

    float defocus_angle = 0;          // variation angle of rays through each pixel
    float focus_dist = 10;            // distance from camera lookfrom point to plane of perfect focus

    float vfov = 90;                  // vertical view angle (field of view)

    std::vector<std::vector<color>> image; // 2D Vector for storing pixel calculations of threads

    void render(const hittable& world) { // unthreaded render
        initialize(); // camera properties initialization
        image.resize(image_height, std::vector<color>(image_width));

        std::clog << "[" << return_current_time_and_date() << "] " << "Render started" << std::endl;
        {
            benchmark::Benchmark<float> timer;
            for (int j = 0; j < image_height; j++) {
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    image[j][i] = pixel_color * pixel_samples_scale;
                }
                std::string equal(int(j / 5), '=');
                std::string empty(int((image_height - j) / 5), '.');
                std::clog << "\r[" << equal << empty << "] " << "Scanlines remaining: " << image_height - j << " " << std::flush;
            }
        }
        write_bmp("image.bmp", image, image_width, image_height);
        std::clog << "[" << return_current_time_and_date() << "] " << "Render finished \n";
    }

    void threaded_render(const hittable& world) {
        Logger logger(LOG_FILE_NAME);
        initialize(); // camera properties initialization

        image.resize(image_height, std::vector<color>(image_width));

        std::vector<std::thread> threads;

        int remainder_rows_amount = (image_height % THREAD_COUNT) > 0 ? image_height % THREAD_COUNT : 0;
        int rows_per_thread = (image_height - remainder_rows_amount) / THREAD_COUNT;
        logger.log("===============TEST RUN===============\n TESTID:", return_current_time_and_date());
        logger.log("[", return_current_time_and_date(), "] ", "Threaded render started");
        for (unsigned int t = 0; t < THREAD_COUNT; t++) {
            int start_row = t * rows_per_thread;
            int end_row = (t + 1) * rows_per_thread - 1;

            if (t == THREAD_COUNT - 1) {
                end_row = image_height - 1;
                threads.emplace_back(this->threaded_render_rows(world, start_row, end_row)); // remainder rows are asssigned to the last thread
                break;
            }
            threads.push_back(this->threaded_render_rows(world, start_row, end_row));
        }

        threads_remaining = threads.size();

        {
            benchmark::Benchmark<float> timer;
            for (auto& thread : threads) {
                thread.join();
            }
        }
        std::clog << "[" << return_current_time_and_date() << "] " << "All thread workers finished \n";

        write_bmp("image.bmp", image, image_width, image_height);
        logger.log("[", return_current_time_and_date(), "] ", "Render finished \n");
        std::clog << std::endl;
        std::clog << "[" << return_current_time_and_date() << "] " << "Render finished \n";
    }

    void render_rows(const hittable& world, int start_row, int end_row) {
        for (int j = start_row; j <= end_row; j++) {
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                image[j][i] = pixel_color * pixel_samples_scale;
            }
        }
        threads_remaining--;

        std::string equal((THREAD_COUNT - threads_remaining) * 2, '=');
        std::string empty(threads_remaining * 2, ' ');

        std::clog << "\r[" << equal << empty << "] " << threads_remaining << " worker(s) left" << std::flush;
    }

    std::thread threaded_render_rows(const hittable& world, const int start_row, const int end_row) {
        return std::thread([this, &world, start_row, end_row] { this->render_rows(world, start_row, end_row); });
    }

    void render_row(const hittable& world, int i) {
        for (int j = 0; j < image_width; j++) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                ray r = get_ray(j, i);
                pixel_color += ray_color(r, max_depth, world);
            }
            image[i][j] = pixel_samples_scale * pixel_color;
        }
    }

    std::thread threaded_render_row(const hittable& world, int i) {
        return std::thread([this, &world, i] { this->render_row(world, i); });
    }

private:
    int    image_height;         // rendered image height
    float pixel_samples_scale;  // color scale factor for a sum of pixel samples
    point3 center;               // camera center
    point3 pixel00_loc;          // location of pixel 0, 0
    vec3   pixel_delta_u;        // offset to pixel to the right
    vec3   pixel_delta_v;        // offset to pixel below
    vec3   u, v, w;              // camera frame basis vectors

    vec3   defocus_disk_u;       // defocus disk horizontal radius
    vec3   defocus_disk_v;       // defocus disk vertical radius
    size_t threads_remaining = 0;

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0f / samples_per_pixel;

        center = lookfrom;

        // determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (float(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // vector down viewport vertical edge

        // calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

        // calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        // construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
            + ((i + offset.x()) * pixel_delta_u)
            + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_float_xorshift();

        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 sample_square() const {
        // returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_float_xorshift() - 0.5f, random_float_xorshift() - 0.5f, 0);
    }

    point3 defocus_disk_sample() const {
        // returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        // if we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0 && max_depth != 0)
            return color(0, 0, 0);

        hit_record rec;

        if (world.hit(r, interval(0.001f, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.5f, 0.7f, 1.0f);
    }
};

#endif
