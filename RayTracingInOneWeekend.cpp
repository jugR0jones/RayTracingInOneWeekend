#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& center, const double radius, const ray& r)
{
    const vec3 oc = center - r.origin();
    const double a = r.direction().length_squared();
    const double h = dot(r.direction(), oc);
    const double c = oc.length_squared() - radius * radius;
    const double discriminant = h * h - a * c;

    if(discriminant < 0.0)
    {
        return -1.0;
    }
    
    return (h -sqrt(discriminant)) / a;
}

color ray_color(const ray& r)
{
    const double t = hit_sphere(point3(0.0, 0.0, -1.0), 0.5, r);
    if (t > 0.0)
    {
        const vec3 N = unit_vector(r.at(t) - vec3(0.0, 0.0, -1.0));

        return 0.5 * color(N.x() + 1.0, N.y() + 1.0, N.z() + 1.0);
    }

    const vec3 unit_direction = unit_vector(r.direction());
    const double a = 0.5 * (unit_direction.y() + 1.0);

    const double b = 1.0 - a;
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[])
{
    // Image
    constexpr double aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    constexpr double focal_length = 1.0;
    constexpr double viewport_height = 2.0;
    const double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    const point3 camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const vec3 viewport_u = vec3(viewport_width, 0, 0);
    const vec3 viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    const vec3 pixel_delta_u = viewport_u / image_width;
    const vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    const vec3 viewport_upper_left = camera_center
        - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    const vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";

    return 0;
}
