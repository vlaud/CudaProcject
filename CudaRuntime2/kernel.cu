#include "color.h"
#include <iostream>

int main()
{
	// 이미지
	int ImageWidth = 256;
	int ImageHeight = 256;

	// 렌더
	std::cout << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n266\n";

	for (int j = 0; j < ImageHeight; j++)
	{
		std::clog << "\rSvanlines remaining: " << (ImageHeight - j) << ' ' << std::flush;
		for (int i = 0; i < ImageWidth; i++)
		{
			auto PixelColor = Color(double(i) / (ImageWidth - 1), double(j) / (ImageHeight - 1), 0);
			WriteColor(std::cout, PixelColor);
		}
	}

	std::clog << "\rDone.                  \n";
	return 0;
}