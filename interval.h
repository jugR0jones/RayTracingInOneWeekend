#pragma once

#include "rtweekend.h"

class interval
{
public:
    double min;
    double max;

    interval() : min(+infinity), max(-infinity)
    {
        // Default interval is empty
    }

    interval(const double min, const double max): min(min), max(max)
    {
    }

    double size() const
    {
        return max - min;
    }

    bool contains(const double x) const
    {
        return min <= x && x <= max;
    }

    bool surrounds(const double x) const
    {
        return min < x && x < max;
    }

    static const interval empty;
    static const interval universe;
};