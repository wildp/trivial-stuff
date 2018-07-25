#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "bitmap.h"
#include "print.h"

// Allows for the direct printing of bin;
std::ostream& operator<<(std::ostream &out, bin &data)
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
std::ostream& operator<<(std::ostream &out, bmp::fileheader &header)
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
std::ostream& operator<<(std::ostream &out, bmp::infoheader &header)
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
std::ostream& operator<<(std::ostream &out, bmp::ctentry &entry)
{
	out << std::hex;
	out << std::setfill('0') << std::setw(2) << static_cast<int>(entry.zero) << ' ';
	out << std::setfill('0') << std::setw(2) << static_cast<int>(entry.red) << ' ';
	out << std::setfill('0') << std::setw(2) << static_cast<int>(entry.green) << ' ';
	out << std::setfill('0') << std::setw(2) << static_cast<int>(entry.blue);
	out << std::dec;
	return out;
}

// Allows for an entire color table to be printed along with the index of each color
std::ostream& operator<<(std::ostream &out, bmp::colortable &ct)
{
	uint32_t decwidth{ std::string{std::to_string(ct.size())}.size() };
	uint32_t hexwidth{ std::string{ std::to_string(((ct.size()) / 16)) }.size() }; // this may be wrong
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

std::ostream& operator<<(std::ostream &out, bmp::image &image)
{
	out << "BITMAP IMAGE:\n\n" << image.file << '\n' << image.info << '\n';
	out << image.ctab << "\nPixel Data:\n" << image.data << "\n\n";
	return out;
}

namespace bmp
{
	// Converts 4 uint8_t in order into 1 uint32_t
	uint32_t convert4(const uint8_t &byte4, const uint8_t &byte3, const uint8_t &byte2, const uint8_t &byte1)
	{
		return (static_cast<uint32_t>(byte1) * 16777216) + (static_cast<uint32_t>(byte2) * 65536) + (static_cast<uint32_t>(byte3) * 256) + static_cast<uint32_t>(byte4);
	}

	// Converts 2 uint8_t in order into 1 uint16_t
	uint16_t convert2(const uint8_t &byte2, const uint8_t &byte1)
	{
		return (static_cast<uint16_t>(byte1) * 256) + static_cast<uint16_t>(byte2);
	}

	// Extracts the image into a (slightly) more useable format
	void extract(const bin &data, bmp::image &image)
	{
		image.file.file_type[0]   = data[0];
		image.file.file_type[1]   = data[1];
		image.file.file_size      = convert4(data[2], data[3], data[4], data[5]);
		image.file.reserved1      = convert2(data[6], data[7]);
		image.file.reserved2      = convert2(data[8], data[9]);
		image.file.image_offset   = convert4(data[10], data[11], data[12], data[13]);
		
		image.info.header_size    = convert4(data[14], data[15], data[16], data[17]);
		image.info.width          = convert4(data[18], data[19], data[20], data[21]);
		image.info.height         = convert4(data[22], data[23], data[24], data[25]);
		image.info.color_planes   = convert2(data[26], data[27]);
		image.info.bits_per_pixel = convert2(data[28], data[29]);
		image.info.compression    = convert4(data[30], data[31], data[32], data[33]);
		image.info.image_size     = convert4(data[34], data[35], data[36], data[37]);
		image.info.x_resolution   = convert4(data[38], data[39], data[40], data[41]);
		image.info.y_resolution	  = convert4(data[42], data[43], data[44], data[45]);
		image.info.no_colors      = convert4(data[46], data[47], data[48], data[49]);
		image.info.impt_colors    = convert4(data[50], data[51], data[52], data[53]);

		for (unsigned int i{ 0 }; i < ((image.file.image_offset - 54) / 4); ++i)
		{
			image.ctab.push_back({ data[54 + (i * 4)], data[55 + (i * 4)], data[56 + (i * 4)], data[57 + (i * 4)] });
		}

		for (unsigned int i{ image.file.image_offset }; i < data.size(); ++i)
		{
			image.data.push_back(data[i]);
		}

		return;
	}

	void generate24(const bmp::image &image, bmp::pixeldata &data)
	{
		if (image.info.bits_per_pixel == 24)
		{
			for (int i{ 0 }; i < (image.data.size() / 3); i++)
				data.push_back({ image.data[(i * 3)], image.data[(i * 3) + 1], image.data[(i * 3) + 2] });
		}
		return;
	}

	void render(const bmp::image &image)
	{
		if (image.info.bits_per_pixel == 24)
		{
			bmp::pixeldata data;
			bmp::generate24(image, data);
			return print::bitmap24(image, data);
		}
		return;
	}
}

