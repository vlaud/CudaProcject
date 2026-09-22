#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

struct Vector3
{
	Vector3() : E{ 0,0,0 } {}
	Vector3(double e0, double e1, double e2) : E{ e0,e1,e2 } {}

	double X() const { return E[0]; }
	double Y() const { return E[1]; }
	double Z() const { return E[2]; }

	Vector3 operator-() const { return Vector3(-E[0], -E[1], -E[2]); }
	double operator[](int i) const { return E[i]; }
	double& operator[](int i) { return E[i]; }

	Vector3& operator+=(const Vector3& v)
	{
		E[0] += v.E[0];
		E[1] += v.E[1];
		E[2] += v.E[2];
		return *this;
	}

	Vector3& operator*=(double t)
	{
		E[0] *= t;
		E[1] *= t;
		E[2] *= t;
		return *this;
	}

	Vector3& operator/=(double t)
	{
		return *this *= 1 / t;
	}

	double Length() const
	{
		return std::sqrt(LengthSquared());
	}

	double LengthSquared() const
	{
		return E[0] * E[0] + E[1] * E[1] + E[2] * E[2];
	}

	static Vector3 Random()
	{
		return Vector3(RandomDouble(), RandomDouble(), RandomDouble());
	}

	static Vector3 Random(double min, double max)
	{
		return Vector3(RandomDouble(min,max), RandomDouble(min, max), RandomDouble(min, max));
	}
	
	double E[3];
};

typedef Vector3 Vec3;

// Point3 = Vector3 별칭, 코드 기하학적 명확성 위해 유용함
using Point3 = Vector3;


// 벡터 유틸리티 함수들

inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
	return out << v.E[0] << ' ' << v.E[1] << ' ' << v.E[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v)
{
	return Vector3(u.E[0] + v.E[0], u.E[1] + v.E[1], u.E[2] + v.E[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v)
{
	return Vector3(u.E[0] - v.E[0], u.E[1] - v.E[1], u.E[2] - v.E[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
	return Vector3(u.E[0] * v.E[0], u.E[1] * v.E[1], u.E[2] * v.E[2]);
}

inline Vector3 operator*(double t, const Vector3& v)
{
	return Vector3(t * v.E[0], t * v.E[1], t * v.E[2]);
}

inline Vector3 operator*(const Vector3& v, double t)
{
	return t * v;
}

inline Vector3 operator/(const Vector3& v, double t)
{
	return (1 / t) * v;
}

inline double Dot(const Vector3& u, const Vector3& v)
{
	return u.E[0] * v.E[0]
		+ u.E[1] * v.E[1]
		+ u.E[2] * v.E[2];
}

inline Vector3 Cross(const Vector3& u, const Vector3& v)
{
	return Vector3(u.E[1] * v.E[2] - u.E[2] * v.E[1],
		u.E[2] * v.E[0] - u.E[0] * v.E[2],
		u.E[0] * v.E[1] - u.E[1] * v.E[0]);
}

inline Vector3 UnitVector(const Vector3& v)
{
	return v / v.Length();
}

inline Vector3 RandomUnitVector()
{
	while (true)
	{
		auto p = Vector3::Random(-1.0, 1.0);
		auto lengthSquared = p.LengthSquared();

		if (1e-160 < lengthSquared && lengthSquared <= 1.0)
		{
			return p / std::sqrt(lengthSquared);
		}
	}
}

inline Vector3 RandomOnHemisphere(const Vector3& normal)
{
	Vector3 unitSphereDirection = RandomUnitVector();

	// 표면 법선과 무작위 벡터 내적 구함
	// 내적이 양수면 벡터가 올바른 반구에 있음
	if (Dot(unitSphereDirection, normal) > 0.0)
	{
		return unitSphereDirection;
	}

	// 내적이 음수면 벡터 반전
	return -unitSphereDirection;
}
#endif