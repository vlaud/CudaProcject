#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius)
		: mCenter(center)
		, mRadius(radius)
	{

	}

	bool Hit(
		const Ray& ray,
		double rayTMin,
		double rayTMax,
		HitRecord& hitRecord
	) const override
	{
		Vec3 originToCenter = mCenter - ray.Origin();

		auto a = ray.Direction().LengthSquared();
		auto h = Dot(ray.Direction(), originToCenter);
		auto c = originToCenter.LengthSquared() - mRadius * mRadius;

		auto discriminant = h * h - a * c;
		if (discriminant < 0.0)
		{
			return false;
		}

		auto squareRootDiscriminant = std::sqrt(discriminant);

		// 범위 중 가장 가까운 루트 찾기 
		auto root = (h - squareRootDiscriminant) / a;
		if (root <= rayTMin || rayTMax <= root)
		{
			root = (h + squareRootDiscriminant) / a;
			if (root <= rayTMin || rayTMax <= root) return false;
		}

		hitRecord.T = root;
		hitRecord.P = ray.At(hitRecord.T);

		Vec3 outwardNormal = (hitRecord.P - mCenter) / mRadius;
		hitRecord.SetFaceNormal(ray, outwardNormal);

		return true;
	}
private:
	Point3 mCenter;
	double mRadius;
};
#endif