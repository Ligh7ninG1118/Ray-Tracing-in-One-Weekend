#include "Header/rtweekend.h"

#include "Header/color.h"
#include "Header/hittable_list.h"
#include "Header/sphere.h"
#include "Header/camera.h"

#include <iostream>
#include <chrono>

color ray_color(const ray &r, const hittable &world)
{
    hit_record rec;

    if (world.hit(r, 0, inf, rec))
    {
        return 0.5 * color(rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(void)
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;

    // Render
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    auto start = std::chrono::system_clock::now();
    for (int i = image_height - 1; i >= 0; i--)
    {
        std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
        for (int j = image_width - 1; j >= 0; j--)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                auto u = (j + random_double()) / (image_width - 1);
                auto v = (i + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> timecost = end - start;

    std::cerr << "\nDone. Total time cost: " << timecost.count() << "s"
              << "\n";
}