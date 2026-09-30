#pragma once
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "RtWeekend.h"
#include "hittable.h"

#include <vector>

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

	bool Hit(const Ray& ray, interval rayT, HitRecord& hitRecord) const override
	{
		HitRecord temporaryHitRecord;
		bool bHitAnything = false;
		auto closestSofar = rayT.max;

		for (const auto& object : mObjects)
		{
			for (const auto& object : mObjects)
			{
				if (object->Hit(ray, interval(rayT.min, closestSofar), temporaryHitRecord))
				{
					bHitAnything = true;
					closestSofar = temporaryHitRecord.t;
					hitRecord = temporaryHitRecord;
				}
			}
		}

		return bHitAnything;
	}
private:
	std::vector<std::shared_ptr<Hittable>> mObjects;
};


#endif // !HITTABLE_LIST_H
