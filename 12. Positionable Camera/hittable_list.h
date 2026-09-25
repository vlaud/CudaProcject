#pragma once
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable
{
public:
	HittableList() = default;

	explicit HittableList(const std::shared_ptr<Hittable>& object)
	{
		Add(object);
	}

	void Clear()
	{
		mObjects.clear();
	}

	void Add(const std::shared_ptr<Hittable>& object)
	{
		mObjects.push_back(object);
	}

	bool Hit(const Ray& ray, interval ray_t, HitRecord& hitRecord) const override
	{
		HitRecord temporaryHitRecord;
		bool bHitAnything = false;
		auto closestSofar = ray_t.max;

		for (const auto& object : mObjects)
		{
			if (object->Hit(ray, interval(ray_t.min, closestSofar), temporaryHitRecord))
			{
				bHitAnything = true;
				closestSofar = temporaryHitRecord.t;
				hitRecord = temporaryHitRecord;
			}
		}

		return bHitAnything;
	}
private:
	std::vector<std::shared_ptr<Hittable>> mObjects;
};


#endif // !HITTABLE_LIST_H
