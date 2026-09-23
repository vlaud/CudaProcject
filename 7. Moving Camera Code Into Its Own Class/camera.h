#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera
{
public:
	double aspectRatio = 1.0;	// 종횡비 
	int imageWidth = 100;

	void Render(const Hittable& world)
	{
		Initialize();

		std::cout << "P3\n" << imageWidth << ' ' << mImageHeight << "\n255\n";

		for (int scanlineIndex = 0; scanlineIndex < mImageHeight; scanlineIndex++)
		{
			std::clog
				<< "\rSvanlines remaining: "
				<< (mImageHeight - scanlineIndex)
				<< ' '
				<< std::flush;

			for (int pixelIndex = 0; pixelIndex < imageWidth; pixelIndex++)
			{
				auto pixelCenter =
					mPixel00Location
					+ (pixelIndex * mPixelDeltaU)
					+ (scanlineIndex * mPixelDeltaV);

				auto rayDirection = pixelCenter - mCenter;
				Ray r(mCenter, rayDirection);

				Color PixelColor = RayColor(r, world);
				WriteColor(std::cout, PixelColor);
			}
		}

		std::clog << "\rDone.                  \n";
	}

private:
	void Initialize()
	{
		mImageHeight = static_cast<int>(imageWidth / aspectRatio);
		mImageHeight = (mImageHeight < 1) ? 1 : mImageHeight;

		mCenter = Point3(0.0, 0.0, 0.0);

		// 뷰포트 크기 설정
		auto focalLength = 1.0;
		auto viewportHeight = 2.0;
		auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / mImageHeight);

		// 뷰포트 가로 및 세로 벡터 계산
		auto viewportU = Vec3(viewportWidth, 0.0, 0.0);
		auto viewportV = Vec3(0.0, -viewportHeight, 0.0);

		// 픽셀 간 수평 및 수직 델타 벡터 계산
		mPixelDeltaU = viewportU / imageWidth;
		mPixelDeltaV = viewportV / mImageHeight;

		// 왼쪽 위 픽셀의 위치 계산
		auto viewportUpperLeft =
			mCenter
			- Vec3(0.0, 0.0, focalLength)
			- viewportU / 2.0
			- viewportV / 2.0;

		mPixel00Location = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);
	}

	Color RayColor(const Ray& ray, const Hittable& world) const
	{
		HitRecord hitRecord;
		if (world.Hit(ray, 0.0, infinity, hitRecord))
		{
			return 0.5 * (hitRecord.Normal + Color(1.0, 1.0, 1.0));
		}

		Vec3 unitDirection = UnitVector(ray.Direction());
		auto a = 0.5 * (unitDirection.Y() + 1.0);

		return (1.0 - a) * Color(1.0, 1.0, 1.0)
			+ a * Color(0.5, 0.7, 1.0);
	}

private:
	int mImageHeight = 0;		// 렌더 이미지 높이
	Point3 mCenter;				// 카메라 중앙
	Point3 mPixel00Location;	// 픽셀 0,0 위치
	Vec3 mPixelDeltaU;			// 오른쪽 픽셀 오프셋
	Vec3 mPixelDeltaV;			// 아래 픽셀 오프셋
};

#endif // !CAMERA_H
