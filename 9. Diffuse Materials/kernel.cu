#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main()
{
	// 월드
	HittableList world;
	world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5));
	world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0));

	Camera camera;

	camera.aspectRatio = 16.0 / 9.0;
	camera.imageWidth = 400;
	camera.samplesPerPixel = 100;
	camera.maxDepth = 50;

	camera.Render(world);
	return 0;
}