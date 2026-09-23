#ifndef HITTABLE_H
#define HITTABLE_H

class Material;

class HitRecord
{
public:
	Point3 point;
	Vec3 Normal;
	std::shared_ptr<Material> material;
	double t = 0.0;
	bool bFrontFace = false;

	void SetFaceNormal(const Ray& r, const Vec3& outwardNormal)
	{
		// 히트 레코드 법선 벡터 설정
		// 참고: 매개변수 'outwardNormal'은 단위 길이를 가진다고 가정

		bFrontFace = Dot(r.Direction(), outwardNormal) < 0.0;
		Normal = bFrontFace ? outwardNormal : -outwardNormal;
	}
	
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, interval ray_t, HitRecord& rec) const = 0;
};

#endif