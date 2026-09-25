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

class Dielectric : public Material
{
public:
	explicit Dielectric(double refractionIndex)
		: mRefractionIndex(refractionIndex)
	{

	}

	bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation,
		Ray& scattered) const override
	{
		attenuation = ColorOne();

		const double refractionRatio =
			hitRecord.frontFace ? (1.0 / mRefractionIndex) : mRefractionIndex;

		const Vec3 unitDirection = UnitVector(rayIn.Direction());

		const double cosTheta =
			std::fmin(Dot(-unitDirection, hitRecord.Normal), 1.0);

		const double sinTheta =
			std::sqrt(1.0 - cosTheta * cosTheta);

		const bool cannotRefract =
			refractionRatio * sinTheta > 1.0;

		Vec3 direction;

		if (cannotRefract)
		{
			direction = Reflect(unitDirection, hitRecord.Normal);
		}
		else
		{
			direction = Refract(unitDirection, hitRecord.Normal, refractionRatio);
		}

		scattered = Ray(hitRecord.point, direction);

		return true;
	}

private:
	// 굴절 인덱스. 공기/진공 -> 물질로 들어가는 경우, 일반적 유리는 ~1.5
	double mRefractionIndex = 1.0;

	static double Reflectance(double cosine, double refractionIndex)
	{
		// Schlick 의 반사율 근사 사용
		auto r0 = (1.0 - refractionIndex) / (1.0 + refractionIndex);
		r0 = r0 * r0;
		return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5);
	}
};