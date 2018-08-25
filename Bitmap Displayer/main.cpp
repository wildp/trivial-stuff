#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bitmap.h"
#include "print.h"

// Read binary data and write to an empty bin
int read(const std::string &filename, bin &data)
{
	std::ifstream image;
	image.open(filename, std::ios::in | std::ios::binary);

	if (image.is_open())
	{
		image.seekg(0, std::ios::end);
		std::streamoff length = image.tellg();
		image.seekg(0, std::ios::beg);

		//std::cout << "Image " << filename << " has a size of " << length << " bytes\n";//

		char * buffer = new char[length];
		image.read(buffer, length);
		image.close();

		for (unsigned int i{ 0 }; i < length; ++i)
		{
			data.push_back(static_cast<uint8_t>(buffer[i]));
		}

		delete[] buffer;
		return 0;
	}
	else
	{
		std::cerr << "Error: Image " << filename << " not found\n";
		return 1;
	}
}

void view(std::string &filename)
{
	bin data;
	int status = read(filename, data);
	if (status == 0)
	{
		bmp::image image;
		bmp::extract(data, image);

		//std::cout << image << '\n';//


		bmp::render(image);
	}
	return;
}

int main(int argc, char *argv[])
{
	print::setup();
	std::string filename{ "Test.bmp" };

	if (argc = 2)
	{
		filename = argv[1];
		view(filename);
	}
	else
	{
		std::cerr << "Error: No file specified\n";
	}

	system("pause");
	return 0;
}