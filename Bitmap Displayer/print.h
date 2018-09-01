#ifndef PRINT_H
#define PRINT_H

namespace print
{
	void setup();
	void bitmap(const bmp::image &image, bmp::pixeldata &data);
	void bitmap(const bmp::image &image, bmp::i1pixeldata &i1data);
	//void bitmap(const bmp::image &image, bmp::i8pixeldata &i8data);
}


#endif // !PRINT_H
