#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class Camera
{
public:
	double aspectRatio = 1.0;					// 이미지 너비 대 높이 비율 
	int imageWidth = 100;						// 렌더링된 이미지 너비(픽셀 단외)
	int samplesPerPixel = 10;					// 각 픽셀당 랜덤 샘플 수
	int maxDepth = 10;							// 장면으로의 최대 광선 반사 개수

	double vfov = 90;							// 수직 시야각(시야)
	Point3 lookfrom = PointZero();				// 카메라 바라보는 위치
	Point3 lookat = Point3(0.0, 0.0, -1.0);		// 카레라 바라보는 점
	Vec3 vup = Vec3(0.0, 1.0, 0.0);				// 카메라 위쪽 방향

	double defocus_angle = 0;	// 각 픽셀을 통과하는 광선의 변화 각도
	double focus_dist = 10;		// 카메라 시점 위치(lookfrom)에서 완전한 초점이 맺히는 평면까지 거리

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

		mCenter = lookfrom;

		// 뷰포트 크기 설정
		auto focalLength = (lookfrom - lookat).Length();
		auto theta = DegreesToRadians(vfov);
		auto h = std::tan(theta / 2);
		auto viewportHeight = 2.0 * h * focalLength;
		auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / mImageHeight);

		// 카메라 좌표 프레임에 대한 u,v,w 단위 기저 벡터 계산
		w = UnitVector(lookfrom - lookat);
		u = UnitVector(Cross(vup, w));
		v = Cross(w, u);

		// 뷰포트 가로 및 세로 벡터 계산
		auto viewportU = viewportWidth * u;		// 뷰포트 수평 가장자리를 가로지르는 벡터
		auto viewportV = viewportHeight * -v;	// 뷰포트 수직 가장자리를 따라 내려가는 벡터

		// 픽셀 간 수평 및 수직 델타 벡터 계산
		mPixelDeltaU = viewportU / imageWidth;
		mPixelDeltaV = viewportV / mImageHeight;

		// 왼쪽 위 픽셀의 위치 계산
		auto viewportUpperLeft =
			mCenter
			- (focalLength * w)
			- viewportU / 2.0
			- viewportV / 2.0;

		mPixel00Location = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);

		const double defocusRadius =
			focus_dist * std::tan(DegreesToRadians(defocus_angle * 0.5));

		mDefocustDiskU = u * defocusRadius;
		mDefocustDiskV = v * defocusRadius;
	}

	Ray GetRay(int pixelIndex, int scanlineIndex) const
	{
		// 원점에서 시작 -> 픽셀 위치(pixelIndex, scanlineIndex) 주변의 무작위로 샘플링된 지점을 향하는 카메라 광선

		auto offset = sample_square();

		auto pixel_sample =
			mPixel00Location
			+ ((pixelIndex + offset.X()) * mPixelDeltaU)
			+ ((scanlineIndex + offset.Y()) * mPixelDeltaV);

		auto rayOrigin = (defocus_angle <= 0.0) ? mCenter : DefocusDiskSample();
		auto rayDirection = pixel_sample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 sample_square() const
	{
		return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
	}

	Point3 DefocusDiskSample() const
	{
		const Vec3 point = random_in_unit_disk();
		return mCenter + (point.X() * mDefocustDiskU) + (point.Y() * mDefocustDiskV);
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
			Ray scattered;
			Color attenuation;

			if (hitRecord.material->Scatter(ray, hitRecord, attenuation, scattered))
			{
				return attenuation * RayColor(scattered, depth - 1, world);
			}

			return Color(0.0, 0.0, 0.0);
		}

		Vec3 unitDirection = UnitVector(ray.Direction());
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
	Vec3 u, v, w;						// 카메라 프레임 기저 벡터

	Vec3 mDefocustDiskU;				// 초점 흐리기 수평 반경
	Vec3 mDefocustDiskV;				// 초점 흐리기 수직 반경
};

#endif // !CAMERA_H
