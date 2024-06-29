#pragma once

#include "hittable.h"
#include "interval.h"
#include "ray.h"

class sphere : public hittable
{
public:
    sphere(const point3& center, const double radius) : center_(center), radius_(fmax(0.0, radius))
    {
    }

    bool hit(const ray& r, const interval ray_t, hit_record& rec) const override
    {
        const vec3 oc = center_ - r.origin();
        const double a = r.direction().length_squared();
        const double h = dot(r.direction(), oc);
        const double c = oc.length_squared() - radius_*radius_;

        const double discriminant = h*h - a*c;
        if(discriminant < 0.0)
        {
            return false;
        }

        const double sqrtd = sqrt(discriminant);
        
        // Find the nearest root that lies in the acceptable range
        double root = (h - sqrtd) / a;
        if(!ray_t.surrounds(root))
        {
            root = (h + sqrtd / a);
            if(!ray_t.surrounds(root))
            {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        const vec3 outward_normal = (rec.p - center_) / radius_;

        rec.set_face_normal(r, outward_normal);
        
        return true;
    }

private:
    point3 center_;
    double radius_;
};
