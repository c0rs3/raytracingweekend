# raytracingweekend
- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html), [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html) implementation with multithreading && various optimizations in C++ with additional [benchmarking tools](https://github.com/c0rs3/benchmark) and .BMP image output
- Final test render in the ```image.png``` file

![](image.png)

## Huge thanks & credit to: [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) | [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
## How to Setup the Render
### 1. Initialize camera and it's properties
```
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
```
### 2. Initialize the sphere(s)
#### Metal
- ```auto object = make_shared<metal>(color(R, G, B), fuzzing);```
#### Matte
- ```auto object = make_shared<lambertian>(color(R, G, B));```
#### Dielectric
- ```auto object = make_shared<dielectric>(color(R, G, B), refraction_index);```
### 3. Add the sphere(s) with it's location vector and radius to a ```hittable_list``` to be rendered 
- ```hittable_list.add(make_shared<sphere>(point3(x, y, z), r, object));```
### 4. Render
```cam.render(hittable_list);```
- or render using the threaded and BVH version which provides __8X or much higher __ better rendering performance
```
world = hittable_list(make_shared<bvh_node>(world));
cam.threaded_render(hittable_list)
``` 
## How to Build & Run the Render
### Using CMake
#### Prerequisites
- ```cmake version 3.30.0``` 
- ```CMakeLists.txt```
#### Release Mode:
```
cmake -B build
cmake --build build --config release
build\Release\rtend.exe
```
#### Debug Mode:
```
cmake -B build
cmake --build build --config debug
build\Debug\rtend.exe
```
### Using MSVC
#### Prerequisites
- ```MSVC v142``` or higher
- ```Developer Command Prompt for 2022/2019``` for running the commands
```
cl /EHsc main.cpp
main.exe
```
### g++
- Currently cannot be compiled with g++
## TODO

