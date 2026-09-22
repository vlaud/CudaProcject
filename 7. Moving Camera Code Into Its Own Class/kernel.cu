#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

Color RayColor(const Ray& ray, const Hittable& world)
{
	HitRecord hitRecord;
	if (world.Hit(ray, 0.0, Infinity, hitRecord))
	{
		return 0.5 * (hitRecord.Normal + Color(1.0, 1.0, 1.0));
	}

	Vector3 unitDirection = UnitVector(ray.Direction());
	auto a = 0.5 * (unitDirection.Y() + 1.0);

	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main()
{
	// 월드
	HittableList world;
	world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5));
	world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0));

	Camera camera;

	camera.aspectRatio = 16.0 / 9.0;
	camera.imageWidth = 400;

	camera.Render(world);
	return 0;
}