#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

Color RayColor(const Ray& ray, const Hittable& world)
{
	HitRecord hitRecord;
	if (world.Hit(ray, interval(0.0, infinity), hitRecord))
	{
		return 0.5 * (hitRecord.Normal + Color(1.0, 1.0, 1.0));
	}

	Vec3 unitDirection = UnitVector(ray.Direction());
	auto a = 0.5 * (unitDirection.Y() + 1.0);

	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main()
{
	// 이미지
	auto aspectRatio = 16.0 / 9.0;
	int imageWidth = 400;

	// 이미지 높이 계산 -> 최소 1 되도록
	int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	// 월드

	HittableList world;
	world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5));
	world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0));


	// 카메라
	auto focalLength = 1.0;
	auto viewportHeight = 2.0;
	auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
	auto cameraCenter = Point3(0, 0, 0);

	// 뷰포트의 수평 및 수직 가장자리를 가로지르는 벡터 계산
	auto viewportU = Vec3(viewportWidth, 0, 0);
	auto viewportV = Vec3(0, -viewportHeight, 0);

	// 픽셀 간 수평 및 수직 델타 벡터 계산
	auto pixelDeltaU = viewportU / imageWidth;
	auto pixelDeltaV = viewportV / imageHeight;

	// 왼쪽 위 픽셀의 위치 계산
	auto viewportUpperLeft =
		cameraCenter
		- Vec3(0, 0, focalLength)
		- viewportU / 2
		- viewportV / 2;

	auto pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

	// 렌더

	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	for (int scanlineIndex = 0; scanlineIndex < imageHeight; scanlineIndex++)
	{
		std::clog
			<< "\rSvanlines remaining: "
			<< (imageHeight - scanlineIndex)
			<< ' '
			<< std::flush;

		for (int pixelIndex = 0; pixelIndex < imageWidth; pixelIndex++)
		{
			auto pixelCenter =
				pixel00Location
				+ (pixelIndex * pixelDeltaU)
				+ (scanlineIndex * pixelDeltaV);

			auto rayDirection = pixelCenter - cameraCenter;
			Ray r(cameraCenter, rayDirection);

			Color PixelColor = RayColor(r, world);
			WriteColor(std::cout, PixelColor);
		}
	}

	std::clog << "\rDone.                  \n";
	return 0;
}