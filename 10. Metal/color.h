#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using Color = Vec3;

inline double LinearToGamma(double linearComponent)
{
	if (linearComponent > 0.0)
	{
		return std::sqrt(linearComponent);
	}

	return 0.0;
}

void WriteColor(std::ostream& out, const Color& pixelColor)
{
	auto r = pixelColor.X();
	auto g = pixelColor.Y();
	auto b = pixelColor.Z();

	// 감마 2.0에 대한 선형-감마 변환을 적용
	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	// [0,1] 범위의 컴포넌트 값을 바이트 범위[0,255]로 변환
	static const interval intensity(0.000, 0.999);

	int rByte = static_cast<int>(256.0 * intensity.clamp(r));
	int gByte = static_cast<int>(256.0 * intensity.clamp(g));
	int bByte = static_cast<int>(256.0 * intensity.clamp(b));

	// 픽셀 색상 컴포넌트 출력
	out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif