#include <iostream>

int main()
{
	int ImageWidth = 256;
	int ImageHeight = 256;

	std::cout << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n266\n";

	for (int j = 0; j < ImageHeight; j++)
	{
		std::clog << "\rSvanlines remaining: " << (ImageHeight - j) << ' ' << std::flush;
		for (int i = 0; i < ImageWidth; i++)
		{
			auto r = double(i) / (ImageWidth - 1);
			auto g = double(j) / (ImageHeight - 1);
			auto b = 0.0;

			int ir = int(255.999 * r);
			int ig = int(255.999 * g);
			int ib = int(255.999 * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}

	std::clog << "\rDone.                  \n";
	return 0;
}