#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#include "bitmap.h"
#include "print.h"

constexpr bool convert_first{ true };

// Allows for the direct printing of bin;
std::ostream& operator<<(std::ostream &out, const bin &data)
{
	out << std::hex;
	for (auto dat : data)
	{
		out << std::setfill('0') << std::setw(2) << static_cast<int>(dat) << ' ';
	}
	out << std::dec;
	return out;
}

// Allows for the printing of the file header struct
std::ostream& operator<<(std::ostream &out, const bmp::fileheader &header)
{
	out << "File Header:\n";
	out << "    File Type:    " << header.file_type[0]   << 
	                               header.file_type[1]   << '\n';
	out << "    File Size:    " << header.file_size      << '\n';
	out << "    Reserved 1:   " << header.reserved1      << '\n';
	out << "    Reserved 2:   " << header.reserved2      << '\n';
	out << "    Image Offset: " << header.image_offset   << '\n';
	return out;
}

// Allows for the printing of the info header struct
std::ostream& operator<<(std::ostream &out, const bmp::infoheader &header)
{
	out << "Info Header:\n";
	out << "    Header Size:  " << header.header_size    << '\n';
	out << "    Image Width:  " << header.width          << '\n';
	out << "    Image Height: " << header.height         << '\n';
	out << "    Color Planes: " << header.color_planes   << '\n';
	out << "    Bits / Pixel: " << header.bits_per_pixel << '\n';
	out << "    Compression:  " << header.compression    << '\n';
	out << "    X-Resolution: " << header.x_resolution   << '\n';
	out << "    Y-Resolution: " << header.y_resolution   << '\n';
	out << "    No. Colors:   " << header.no_colors      << '\n';
	out << "    Impt Colors:  " << header.impt_colors    << '\n';
	return out;
}

// Allows for an individual entry in a color table to be printed
std::ostream& operator<<(std::ostream &out, const bmp::colorBGRA &entry)
{
	out << std::hex;
	out << std::setfill('0') << std::setw(2) << static_cast<int>(entry.alpha) << ' ';
	out << std::setfill('0') << std::setw(2) << static_cast<int>(entry.red) << ' ';
	out << std::setfill('0') << std::setw(2) << static_cast<int>(entry.green) << ' ';
	out << std::setfill('0') << std::setw(2) << static_cast<int>(entry.blue);
	out << std::dec;
	return out;
}

// Allows for an entire color table to be printed along with the index of each color
std::ostream& operator<<(std::ostream &out, const bmp::colortable &ct)
{
	auto decwidth{ std::string{std::to_string(ct.size())}.size() };
	auto hexwidth{ std::string{ std::to_string(((ct.size()) / 16)) }.size() }; // this may be wrong
	uint32_t i{ 0 };

	out << "Color Table:\n";
	if (decwidth > 0)
		for (auto &ctentry : ct)
		{
			out << "    " << std::setfill('0') << std::setw(decwidth) << i;
			out << "    " << std::hex << std::setfill('0') << std::setw(hexwidth) << i++;
			out << " : "  << std::dec << ctentry << '\n';
		}
	else
		out << "    No Color Table Used.";

	return out;
}

// Allows for the printing of the image struct

std::ostream& operator<<(std::ostream &out, const bmp::image &image)
{
	out << "BITMAP IMAGE:\n\n" << image.file << '\n' << image.info << '\n';
	out << image.ctab << "\nPixel Data:\n" << image.data << "\n\n";
	return out;
}

namespace bmp
{
	// Converts 4 uint8_t in order into 1 uint32_t
	inline uint32_t convert4(const uint8_t &byte4, const uint8_t &byte3, const uint8_t &byte2, const uint8_t &byte1)
	{
		return (static_cast<uint32_t>(byte1) * 16777216) + (static_cast<uint32_t>(byte2) * 65536) + (static_cast<uint32_t>(byte3) * 256) + static_cast<uint32_t>(byte4);
	}

	// Converts 2 uint8_t in order into 1 uint16_t
	inline uint16_t convert2(const uint8_t &byte2, const uint8_t &byte1)
	{
		return (static_cast<uint16_t>(byte1) * 256) + static_cast<uint16_t>(byte2);
	}

	inline uint8_t convertH(const bool &bit4, const bool &bit3, const bool &bit2, const bool &bit1)
	{
		return (static_cast<uint32_t>(bit1) * 8) + (static_cast<uint32_t>(bit2) * 4) + (static_cast<uint32_t>(bit3) * 2) + static_cast<uint32_t>(bit4);
	}

	// Extracts the image into a (slightly) more useable format
	int extract(const bin &data, bmp::image &image)
	{
		image.file.file_type[0]   = data[0];
		image.file.file_type[1]   = data[1];

		if (image.file.file_type[0] != 'B' || image.file.file_type[1] != 'M')
		{
			std::cerr << "Error: Image is not a valid bitmap file\n";
			return 1; // abort: not BM
		}
			

		image.file.file_size      = convert4(data[2], data[3], data[4], data[5]);
		image.file.reserved1      = convert2(data[6], data[7]);
		image.file.reserved2      = convert2(data[8], data[9]);
		image.file.image_offset   = convert4(data[10], data[11], data[12], data[13]);
		
		image.info.header_size    = convert4(data[14], data[15], data[16], data[17]);

		if (image.info.header_size != 40)
		{
			std::cerr << "Error: Image does not use BITMAPINFOHEADER - other headers are not yet supported\n";
			return 1; // abort: not BITMAPINFOHEADER
		}

		image.info.width          = convert4(data[18], data[19], data[20], data[21]);
		image.info.height         = convert4(data[22], data[23], data[24], data[25]);
		image.info.color_planes   = convert2(data[26], data[27]);

		if (image.info.color_planes != 1)
		{
			std::cerr << "Error: Image does not have one colour plane\n";
			return 1; // abort: doesnt  have 1 color plane
		}

		image.info.bits_per_pixel = convert2(data[28], data[29]);
		image.info.compression    = convert4(data[30], data[31], data[32], data[33]);

		if (image.info.compression != 0)
		{
			std::cerr << "Error: Image is compression - compressed images are not yet supported\n";
			return 1; // abort: is compressed
		}

		image.info.image_size     = convert4(data[34], data[35], data[36], data[37]);
		image.info.x_resolution   = convert4(data[38], data[39], data[40], data[41]);
		image.info.y_resolution	  = convert4(data[42], data[43], data[44], data[45]);
		image.info.no_colors      = convert4(data[46], data[47], data[48], data[49]);
		image.info.impt_colors    = convert4(data[50], data[51], data[52], data[53]);

		if (image.file.image_offset > 54)
			for (unsigned int i{ 0 }; (i < ((image.file.image_offset - 54) / 4)) && (i < ((image.info.no_colors != 0) ? image.info.no_colors : UINT_MAX - 1)); ++i)
			{
				image.ctab.push_back({ data[54 + (i * 4)], data[55 + (i * 4)], data[56 + (i * 4)], data[57 + (i * 4)] });
			}

		for (unsigned int i{ image.file.image_offset }; i < data.size(); ++i)
		{
			image.data.push_back(data[i]);
		}

		return 0;
	}

	void generate1(const bmp::image &image, bmp::i1pixeldata &i1data)
	{
		uint32_t counter{ 0 };
		int32_t cooldown{ 0 };
		for (auto byte : image.data)
		{
			std::bitset<8> b{ byte };
			// data is little endian so extract byte in reverse
			for (int i{ 7 }; i >= 0; i--)
			{
				if (cooldown == 0)
				{
					counter++;
					i1data.push_back(b[i]);
				}
				else
					cooldown--;

				if (counter >= image.info.width)
				{
					cooldown = ((counter % 32) == 0) ? 0 : (32 - (counter % 32));
					counter = 0;
				}
			}
		}
		return;
	}

	void generate8(const bmp::image &image, bmp::i8pixeldata &i8data)
	{
		if (image.info.bits_per_pixel == 1)
		{
			unsigned int counter{ 0 };
			int cooldown{ 0 };
			for (auto byte : image.data)
			{
				std::bitset<8> b{ byte };
				// data is little endian so extract byte in reverse
				for (int i{ 7 }; i >= 0; i--)
				{
					if (cooldown == 0)
					{
						counter++;
						i8data.push_back(b[i]);
					}
					else
						cooldown--;

					if (counter >= image.info.width)
					{
						cooldown = ((counter % 32) == 0) ? 0 : (32 - (counter % 32));
						counter = 0;
					}
				}
			}
		}

		if (image.info.bits_per_pixel == 4)
		{
			unsigned int counter{ 0 };
			int cooldown{ 0 };
			for (auto byte : image.data)
			{
				std::bitset<8> b{ byte };
				// data is little endian so extract byte in reverse
				for (int i{ 1 }; i >= 0; i--)
				{
					if (cooldown == 0)
					{
						counter++;
						i8data.push_back(convertH(b[(4 * i)], b[(4 * i) + 1], b[(4 * i) + 2], b[(4 * i) + 3]));
					}
					else
						cooldown--;

					if (counter >= image.info.width)
					{
						cooldown = ((counter % 8) == 0) ? 0 : (8 - (counter % 8));
						counter = 0;
					}
				}
			}
		}

		if (image.info.bits_per_pixel == 8)
		{
			unsigned int counter{ 0 };
			for (unsigned int i{ 0 }; i < image.data.size(); i++)
			{
				//if (i > image.data.size())
				counter++;
				i8data.push_back(image.data[i]);

				// System to allow for the skipping of 4-byte padding
				if (counter >= image.info.width)
				{
					i += ((counter % 4) == 0) ? 0 : (4 - (counter % 4));
					counter = 0;
				}
			}
		}
		return;
	}

	void generate24(const bmp::image &image, bmp::pixeldata &data)
	{
		if (image.info.bits_per_pixel == 8 || image.info.bits_per_pixel == 4 || image.info.bits_per_pixel == 1)
		{
			bmp::i8pixeldata idata{};
			generate8(image, idata);
			for (auto entry : idata)
				data.push_back(image.ctab[entry]);
		}
		else if (image.info.bits_per_pixel == 24)
		{
			uint32_t counter{ 0 };
			for (unsigned int i{ 0 }; i < image.data.size(); i += 3)
			{
				//if (i > image.data.size())
				counter++;
				data.push_back({ image.data[i], image.data[i + 1], image.data[i + 2], 255 });

				// System to allow for the skipping of 4-byte padding
				if (counter >= image.info.width)
				{
					i += ((counter % 4) == 0) ? 0 : (4 - ((counter * 3) % 4));
					counter = 0;
				}
			}
		}
		return;
	}

	void render(const bmp::image &image)
	{
		if (image.info.bits_per_pixel == 1)
		{
			bmp::i1pixeldata idata;
			bmp::generate1(image, idata);
			return print::bitmap(image, idata);
		}
		if ((image.info.bits_per_pixel == 24) || (convert_first))
		{
			bmp::pixeldata data;
			bmp::generate24(image, data);
			//std::cout << data;
			return print::bitmap(image, data);
		}

		return;
	}
}

