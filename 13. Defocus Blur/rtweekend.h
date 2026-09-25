#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;

// Constants

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double Pi = 3.1415926535897932385;

// Utility Functions

inline double DegreesToRadians(double degrees)
{
	return degrees * Pi / 180.0;
}

inline double RandomDouble()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double RandomDouble(double minimum, double maximum)
{
	// 최소~최대 사이 값 반환
	return minimum + (maximum - minimum) * RandomDouble();
}

// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif // !RTWEEKEND_H
