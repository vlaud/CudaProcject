#pragma once

#include "hittable.h"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord,
		Color& attenuation, Ray& scattered) const
	{
		return false;
	}
};

class Lambertian : public Material
{
public:
	explicit Lambertian(const Color& albedo)
		: mAlbedo(albedo)
	{
	}

	bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation,
		Ray& scattered) const override
	{
		Vec3 scatterDirection = hitRecord.Normal + RandomUnitVector();

		if (scatterDirection.NearZero())
		{
			scatterDirection = hitRecord.Normal;
		}

		scattered = Ray(hitRecord.point, scatterDirection);
		attenuation = mAlbedo;

		return true;
	}
private:
	Color mAlbedo;
};

class Metal : public Material
{
public:
	Metal(const Color& albedo, double fuzz)
		: mAlbedo(albedo)
		, mFuzz(fuzz < 1 ? fuzz : 1)
	{
	}

	bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation,
		Ray& scattered) const override
	{
		Vec3 reflected = Reflect(rayIn.Direction(), hitRecord.Normal);
		reflected = UnitVector(reflected) + (mFuzz * RandomUnitVector());
		scattered = Ray(hitRecord.point, reflected);
		attenuation = mAlbedo;

		return (Dot(scattered.Direction(), hitRecord.Normal) > 0);
	}
private:
	Color mAlbedo;
	double mFuzz;
};