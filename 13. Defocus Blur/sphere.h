#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius, const std::shared_ptr<Material>& material)
		: mCenter(center)
		, mRadius(std::fmax(0.0, radius))
		, mMaterial(material)
	{

	}

	bool Hit(const Ray& ray, interval ray_t, HitRecord& hitRecord) const override
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
		if (!ray_t.surrounds(root))
		{
			root = (h + squareRootDiscriminant) / a;
			if (!ray_t.surrounds(root)) return false;
		}

		hitRecord.t = root;
		hitRecord.point = ray.At(hitRecord.t);

		Vec3 outwardNormal = (hitRecord.point - mCenter) / mRadius;
		hitRecord.SetFaceNormal(ray, outwardNormal);

		hitRecord.material = mMaterial;

		return true;
	}
private:
	Point3 mCenter;
	double mRadius;
	std::shared_ptr<Material> mMaterial;
};

#endif