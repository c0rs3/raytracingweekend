# raytracingweekend
- "Ray Tracing in One Weekend" implementation with multithreading in C++ & CMake with additional [_benchmarking tools_](https://github.com/c0rs3/benchmark) and better output format
- Final test render in the ```image.png``` or ```image.bmp``` file

![](image.png)

## Huge thanks & credit to: [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
## How to Setup the Render
1. Initialize the sphere(s) to be rendered on the screen specifying the material, attributes of the material
- ```auto object = make_shared<material>(color(R, G, B));```
2. Add the sphere(s) with it's location vector and size to ```hittable_list``` to 
be rendered 
```hittable_list.add(make_shared<sphere>(point3(0,-1000,0), 1000, object));```
3. Initialize camera and it's properties
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
4. Render
```cam.render(hittable_list);```
- or render using the threaded version 
```cam.threaded_render(hittable_list)``` which provides upto 4-8x better rendering performance
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
## Current known problems
- Colors appear darker due to .bmp headers values or gamma correction issues