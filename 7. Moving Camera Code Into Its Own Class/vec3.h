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

	double mElements[3];
};

typedef Vec3 Vec3;

// Point3 = Vector3 별칭, 코드 기하학적 명확성 위해 유용함
using Point3 = Vec3;


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

#endif