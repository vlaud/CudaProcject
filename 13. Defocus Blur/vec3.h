#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

struct Vec3
{
	Vec3() : mElements{ 0,0,0 } {}
	Vec3(double e0, double e1, double e2) : mElements{ e0,e1,e2 } {}

	double X() const { return mElements[0]; }
	double Y() const { return mElements[1]; }
	double Z() const { return mElements[2]; }

	Vec3 operator-() const { return Vec3(-mElements[0], -mElements[1], -mElements[2]); }
	double operator[](int i) const { return mElements[i]; }
	double& operator[](int i) { return mElements[i]; }

	Vec3& operator+=(const Vec3& v)
	{
		mElements[0] += v.mElements[0];
		mElements[1] += v.mElements[1];
		mElements[2] += v.mElements[2];
		return *this;
	}

	Vec3& operator*=(double t)
	{
		mElements[0] *= t;
		mElements[1] *= t;
		mElements[2] *= t;
		return *this;
	}

	Vec3& operator/=(double t)
	{
		return *this *= 1 / t;
	}

	double Length() const
	{
		return std::sqrt(LengthSquared());
	}

	double LengthSquared() const
	{
		return mElements[0] * mElements[0] + mElements[1] * mElements[1] + mElements[2] * mElements[2];
	}

	bool NearZero() const
	{
		auto threshold = 1e-8;

		return (std::fabs(mElements[0]) < threshold)
			&& (std::fabs(mElements[1]) < threshold)
			&& (std::fabs(mElements[2]) < threshold);
	}

	static Vec3 Random()
	{
		return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
	}

	static Vec3 Random(double min, double max)
	{
		return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
	}

	double mElements[3] = {};
};

// Point3 = Vector3 별칭, 코드 기하학적 명확성 위해 유용함
using Point3 = Vec3;

inline Point3 PointZero()
{
	return Point3(0.0, 0.0, 0.0);
}

// 벡터 유틸리티 함수들

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.mElements[0] << ' ' << v.mElements[1] << ' ' << v.mElements[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.mElements[0] + v.mElements[0], u.mElements[1] + v.mElements[1], u.mElements[2] + v.mElements[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.mElements[0] - v.mElements[0], u.mElements[1] - v.mElements[1], u.mElements[2] - v.mElements[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.mElements[0] * v.mElements[0], u.mElements[1] * v.mElements[1], u.mElements[2] * v.mElements[2]);
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return Vec3(t * v.mElements[0], t * v.mElements[1], t * v.mElements[2]);
}

inline Vec3 operator*(const Vec3& v, double t)
{
	return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
	return (1 / t) * v;
}

inline double Dot(const Vec3& u, const Vec3& v)
{
	return u.mElements[0] * v.mElements[0]
		+ u.mElements[1] * v.mElements[1]
		+ u.mElements[2] * v.mElements[2];
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u.mElements[1] * v.mElements[2] - u.mElements[2] * v.mElements[1],
		u.mElements[2] * v.mElements[0] - u.mElements[0] * v.mElements[2],
		u.mElements[0] * v.mElements[1] - u.mElements[1] * v.mElements[0]);
}

inline Vec3 UnitVector(const Vec3& v)
{
	return v / v.Length();
}

inline Vec3 random_in_unit_disk()
{
	while (true)
	{
		auto p = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
		if (p.LengthSquared() < 1)
			return p;
	}
}

inline Vec3 RandomUnitVector()
{
	while (true)
	{
		auto p = Vec3::Random(-1.0, 1.0);
		auto lengthSquared = p.LengthSquared();

		if (1e-160 < lengthSquared && lengthSquared <= 1.0)
		{
			return p / std::sqrt(lengthSquared);
		}
	}
}

inline Vec3 RandomOnHemisphere(const Vec3& normal)
{
	Vec3 unitSphereDirection = RandomUnitVector();

	// 표면 법선과 무작위 벡터 내적 구함
	// 내적이 양수면 벡터가 올바른 반구에 있음
	if (Dot(unitSphereDirection, normal) > 0.0)
	{
		return unitSphereDirection;
	}

	// 내적이 음수면 벡터 반전
	return -unitSphereDirection;
}

inline Vec3 Reflect(const Vec3& v, const Vec3& n)
{
	return v - 2.0 * Dot(v, n) * n;
}

inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etaInOverEtaOut)
{
	const double cosTheta = std::fmin(Dot(-uv, n), 1.0);

	const Vec3 refractPerpendicular = etaInOverEtaOut * (uv + cosTheta * n);
	const Vec3 refractParallel =
		-std::sqrt(std::fabs(1.0 - refractPerpendicular.LengthSquared())) * n;

	return refractPerpendicular + refractParallel;
}
#endif