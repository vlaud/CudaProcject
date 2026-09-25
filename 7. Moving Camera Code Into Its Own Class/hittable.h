#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class HitRecord
{
public:
	void SetFaceNormal(const Ray& r, const Vec3& outwardNormal)
	{
		// 히트 레코드 법선 벡터 설정
		// 참고: 매개변수 'outwardNormal'은 단위 길이를 가진다고 가정

		frontFace = Dot(r.Direction(), outwardNormal) < 0;
		Normal = frontFace ? outwardNormal : -outwardNormal;
	}
	Point3 point;
	Vec3 Normal;
	double t;
	bool frontFace;
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, double rayTMin, double rayTMax, HitRecord& rec) const = 0;
};

#endif