﻿#pragma once

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"

class camera
{
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    
    void render(const hittable& world)
    {
        initialize();
        
        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;
            for (int i = 0; i < image_width; i++)
            {
                auto pixel_center = pixel00_loc_ + (i * pixel_delta_u_) + (j * pixel_delta_v_);
                auto ray_direction = pixel_center - camera_center_;
                ray r(camera_center_, ray_direction);

                color pixel_color = ray_color(r, world);
                write_color(std::cout, pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int image_height;
    point3 camera_center_;      // Camera centre
    point3 pixel00_loc_; // Location of pixel 0,0
    vec3 pixel_delta_u_; // Offset to the pixel to the right
    vec3 pixel_delta_v_; // Offset to the pixel below

    void initialize()
    {
        // Calculate the image height, and ensure that it's at least 1.
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
    
        // Camera
        constexpr double focal_length = 1.0;
        constexpr double viewport_height = 2.0;
        const double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
        camera_center_ = point3(0, 0, 0);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        const vec3 viewport_u = vec3(viewport_width, 0, 0);
        const vec3 viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u_ = viewport_u / image_width;
        pixel_delta_v_ = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        const vec3 viewport_upper_left = camera_center_
            - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);

    }

    static color ray_color(const ray& r, const hittable& world)
    {
        hit_record rec;

        if(world.hit(r, interval(0.0, infinity), rec))
        {
            return 0.5 * (rec.normal + color(1.0, 1.0, 1.0));
        }

        vec3 unit_direction = unit_vector(r.direction());
        const double a = 0.5 * (unit_direction.y() + 1.0);

        return (1.0 - a) * color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};
