#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera
{
public:
	double aspectRatio = 1.0;	// 종횡비 
	int imageWidth = 100;
	int samplesPerPixel = 10;			// 각 픽셀 랜덤 샘플 개수
	int maxDepth = 10;	// 최대 레이 바운스 수

	void Render(const Hittable& world)
	{
		Initialize();

		std::cout << "P3\n" << imageWidth << ' ' << mImageHeight << "\n255\n";

		for (int scanlineIndex = 0; scanlineIndex < mImageHeight; scanlineIndex++)
		{
			std::clog
				<< "\rScanlines remaining: "
				<< (mImageHeight - scanlineIndex)
				<< ' '
				<< std::flush;

			for (int pixelIndex = 0; pixelIndex < imageWidth; pixelIndex++)
			{
				Color pixelColor(0.0, 0.0, 0.0);

				for (int sampleIndex = 0; sampleIndex < samplesPerPixel; sampleIndex++)
				{
					Ray ray = GetRay(pixelIndex, scanlineIndex);
					pixelColor += RayColor(ray, maxDepth, world);
				}

				WriteColor(std::cout, mPixelSamplesScale * pixelColor);
			}
		}

		std::clog << "\rDone.                  \n";
	}

private:
	void Initialize()
	{
		mImageHeight = static_cast<int>(imageWidth / aspectRatio);
		mImageHeight = (mImageHeight < 1) ? 1 : mImageHeight;

		mPixelSamplesScale = 1.0 / static_cast<double>(samplesPerPixel);

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

	Ray GetRay(int pixelIndex, int scanlineIndex) const
	{
		// 원점에서 시작 -> 픽셀 위치(pixelIndex, scanlineIndex) 주변의 무작위로 샘플링된 지점을 향하는 카메라 광선

		auto offset = sample_square();
		auto pixel_sample =
			mPixel00Location
			+ ((pixelIndex + offset.X()) * mPixelDeltaU)
			+ ((scanlineIndex + offset.Y()) * mPixelDeltaV);

		auto rayOrigin = mCenter;
		auto rayDirection = pixel_sample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 sample_square() const
	{
		return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
	}

	Color RayColor(const Ray& ray, int depth, const Hittable& world) const
	{
		// 레이 바운스 한계를 넘으면 빛이 더 이상 없게 설정
		if (depth <= 0)
		{
			return Color(0.0, 0.0, 0.0);
		}

		HitRecord hitRecord;

		if (world.Hit(ray, interval(0.001, infinity), hitRecord))
		{
			Vec3 direction = hitRecord.Normal + RandomUnitVector();
			return 0.1 * RayColor(Ray(hitRecord.P, direction), depth - 1, world);
		}

		Vector3 unitDirection = UnitVector(ray.Direction());
		auto a = 0.5 * (unitDirection.Y() + 1.0);

		return (1.0 - a) * Color(1.0, 1.0, 1.0)
			+ a * Color(0.5, 0.7, 1.0);
	}

private:
	int mImageHeight = 0;				// 렌더 이미지 높이
	double mPixelSamplesScale = 1.0;	// 픽셀 샘플 합의 색 스케일 팩터

	Point3 mCenter;						// 카메라 중앙
	Point3 mPixel00Location;			// 픽셀 0,0 위치
	Vec3 mPixelDeltaU;					// 오른쪽 픽셀 오프셋
	Vec3 mPixelDeltaV;					// 아래 픽셀 오프셋
};

#endif // !CAMERA_H
