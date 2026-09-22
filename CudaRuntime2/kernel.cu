#include "color.h"
#include "ray.h"
#include <iostream>

Color RayColor(const Ray& r)
{
	Vec3 unitDirection = UnitVector(r.Direction());
	auto a = 0.5 * (unitDirection.Y() + 1.0);

	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main()
{
	// 이미지
	auto aspectRatio = 16.0 / 9.0;
	int imageWidth = 400;

	// 이미지 높이 계산 -> 최소 1 되도록
	int imageHeight = int(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	// 카메라
	auto focalLength = 1.0;
	auto viewportHeight = 2.0;
	auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
	auto cameraCenter = Point3(0, 0, 0);

	// 뷰포트의 수평 및 수직 가장자리를 가로지르는 벡터 계산
	auto viewportU = Vec3(viewportWidth, 0, 0);
	auto viewportV = Vec3(0, -viewportHeight, 0);

	// 픽셀 간 수평 및 수직 델타 벡터 계산
	auto pixelDeltaU = viewportU / imageWidth;
	auto pixelDeltaV = viewportV / imageHeight;

	// 왼쪽 위 픽셀의 위치 계산
	auto viewportUpperLeft = cameraCenter - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
	auto pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

	// 렌더
	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n266\n";

	for (int j = 0; j < imageHeight; j++)
	{
		std::clog << "\rSvanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
		for (int i = 0; i < imageWidth; i++)
		{
			auto pixelCenter = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
			auto rayDirection = pixelCenter - cameraCenter;
			Ray r(cameraCenter, rayDirection);

			Color PixelColor = RayColor(r);
			WriteColor(std::cout, PixelColor);
		}
	}

	std::clog << "\rDone.                  \n";
	return 0;
}