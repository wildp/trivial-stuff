#ifndef BITMAP_H
#define BITMAP_H

#include <cstdint>
#include <iostream>
#include <vector>

typedef std::vector<uint8_t> bin;

namespace bmp
{
	typedef struct colorBGRA
	{
		uint8_t blue;              // Blue value  (0-255)
		uint8_t green;             // Green value (0-255)
		uint8_t red;               // Red value   (0-255)
		uint8_t alpha;             // Alpha value (0-255)

	} pixel ;

	typedef std::vector<colorBGRA> pixeldata, colortable;

	typedef std::vector<uint8_t> indexedpixeldata, i8pixeldata;
	typedef std::vector<bool> i1pixeldata;

	struct character
	{
		colorBGRA top;
		colorBGRA bot;
	};

	typedef struct indexedcharacter
	{
		colorBGRA top;
		colorBGRA bot;
	} icharacter;

	typedef std::vector<character> characterdata, chardata;
	typedef std::vector<indexedcharacter> indexedcharacterdata, ichardata;

	struct fileheader               // 14 bytes
	{
		char	 file_type[2];      // Should be the uppercase characters BM
		uint32_t file_size;         // The file size of the image in bytes
		uint16_t reserved1;         // One of two unneeded "reserved values" 
		uint16_t reserved2;         // One of two unneeded "reserved values" 
		uint32_t image_offset;      // The offset to the beginning of image data
	};

	struct infoheader               // 40 bytes
	{
		uint32_t header_size;       // Size of the information header in bytes (40)
		uint32_t width;             // The width of the image
		uint32_t height;            // The height of the image
		uint16_t color_planes;      // Number of color planes
		uint16_t bits_per_pixel;    // The number of bits per pixel
		uint32_t compression;       // Compression in bytes (0)
		uint32_t image_size;        // Size of the image in bytes
		uint32_t x_resolution;      // X-resolution in pixels per metre
		uint32_t y_resolution;      // Y-resolution in pixels per metre
		uint32_t no_colors;         // Number of colors
		uint32_t impt_colors;       // Important colors (??)
	};

	struct image
	{
		fileheader file;
		infoheader info;
		colortable ctab;
		bin data;
	};

	int extract(const bin &data, bmp::image &image);

	void generate24(const bmp::image &image, bmp::pixeldata &data);

	void render(const bmp::image &image);
}

std::ostream& operator<<(std::ostream &out, bin &data);
std::ostream& operator<<(std::ostream &out, bmp::fileheader &header);
std::ostream& operator<<(std::ostream &out, bmp::fileheader &header);
std::ostream& operator<<(std::ostream &out, bmp::colorBGRA &entry);
std::ostream& operator<<(std::ostream &out, bmp::colortable &ct);
std::ostream& operator<<(std::ostream &out, bmp::image &image);

#endif // !BITMAP_H