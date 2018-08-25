#include <cstdint>
#include <iostream>
#include <string>

#include <chrono>

#include "bitmap.h"
#include "print.h"

#ifdef _WIN32
	#define DEFINE_CONSOLEV2_PROPERTIES
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <io.h>
	#include <fcntl.h>
#endif

#define ESC '\x1b'
#define BLOCK L'\x2580'

namespace print
{
	int enableVT()
	{
#ifdef _WIN32
		static bool done{ false };
		if (done == false)
		{
			// Set output mode to handle virtual terminal sequences
			HANDLE Out = GetStdHandle(STD_OUTPUT_HANDLE);
			if (Out == INVALID_HANDLE_VALUE)
			{
				return 1;
			}

			//HANDLE In = GetStdHandle(STD_INPUT_HANDLE);
			//if (In == INVALID_HANDLE_VALUE)
			//{
			//	return 1;
			//}

			DWORD OriginalOutMode = 0;
			//DWORD OriginalInMode = 0;

			if (!GetConsoleMode(Out, &OriginalOutMode))
			{
				return 1;
			}

			//if (!GetConsoleMode(In, &OriginalInMode))
			//{
			//	return 1;
			//}

			DWORD RequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
			//DWORD RequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

			DWORD OutMode = OriginalOutMode | RequestedOutModes;
			if (!SetConsoleMode(Out, OutMode))
			{
				RequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				OutMode = OriginalOutMode | RequestedOutModes;
				if (!SetConsoleMode(Out, OutMode))
				{
					std::cerr << "Unable to set VT mode.\n";
					return -1;
				}
			}

			//DWORD InMode = OriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
			//if (!SetConsoleMode(In, InMode))
			//{
			//	std::cerr << "Unable to set VT mode.\n";
			//	return -1;
			//}

			done = true;
		}
#endif
		return 0;
	}

	void setup()
	{
	#ifdef _WIN32
		SetConsoleTitle("Bitmap Displayer");
		static auto status{ enableVT() };
		// lazy method to ensure only one execution of enableVT()
	#endif
		return;
	}
	
	inline void extendcode(std::wstring &existingcode, unsigned char addition)
	{
		existingcode.append(std::to_wstring(static_cast<int>((addition))));
		return;
	}
	
	inline void extendcodeS(std::wstring &existingcode, unsigned char addition)
	{
		existingcode.append(std::to_wstring(static_cast<int>((addition))));
		existingcode.append(L";");
		return;
	}
	
	inline void setcolorF(uint8_t red, uint8_t green, uint8_t blue)
	{
		std::wstring output{ ESC,'[','3','8',';','2',';' };
	
		extendcodeS(output, red);
		extendcodeS(output, green);
		extendcode(output, blue);
		output.append(L"m");
	
		std::wcout << output;
	
		return;
	}
	
	inline void setcolorB(uint8_t red, uint8_t green, uint8_t blue)
	{
		std::wstring output{ ESC,'[','4','8',';','2',';' };
	
		extendcodeS(output, red);
		extendcodeS(output, green);
		extendcode(output, blue);
		output.append(L"m");
	
		std::wcout << output;
	
		return;
	}
	
	inline void setmode(bool isWide)
	{
		std::cerr << std::flush;
		std::cout << std::flush;
		std::wcout << std::flush;
	
		if (isWide == true)
		{
	#ifdef _WIN32
			_setmode(_fileno(stdout), _O_U16TEXT);
	#endif
		}
		else
		{
	#ifdef _WIN32
			_setmode(_fileno(stdout), _O_TEXT);
	#endif
		}
		return;
	}
	
	void bitmap(const bmp::image &image, bmp::pixeldata &data)
	{
		auto start{ std::chrono::system_clock::now() };
	
		setmode(true);
		uint32_t width{ image.info.width };
		uint32_t height{ image.info.height };
	
		// The extracted data in &data has been inverted vertically
		for (uint32_t y{ 0 }; y < height; y += 2)
		{
			if (((height - y - 2) * width) < data.size())
			{
				static unsigned int index1;
				static unsigned int index2;
	
				index1 = 0 + ((height - y - 1) * width);
				index2 = 0 + ((height - y - 2) * width);
	
				for (uint32_t x{ 0 }; x < width; ++x)
				{
					setcolorF(data[index1].red, data[index1].green, data[index1].blue);
					setcolorB(data[index2].red, data[index2].green, data[index2].blue);
					std::wcout << BLOCK;
	
					++index1;
					++index2;
				}
			}
	
			if (((height - y - 2) * width) >= data.size())
			{
				static unsigned int index1;
	
				index1 = 0 + ((height - y - 1) * width);
	
				setcolorB(0, 0, 0);
				for (uint32_t x{ 0 }; x < width; ++x)
				{
					setcolorF(data[index1].red, data[index1].green, data[index1].blue);
					std::wcout << BLOCK;
	
					++index1;
				}
			}
			std::wcout << L'\n';
		}
	
		std::wstring reset{ ESC ,'[','0','m' };
		std::wcout << reset;
		setmode(false);
	
		auto end = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	
		//std::cout << "\nTime taken: " << duration.count() << " ms\n\n";
	
		return;
	}
	/*
	void ibitmap(const bmp::image &image, bmp::ipixeldata &idata)
	{
		auto start = std::time(0);

		setmode(true);
		uint32_t width{ image.info.width };
		uint32_t height{ image.info.height };

		// The extracted data in &data has been inverted vertically
		for (uint32_t y{ 0 }; y < height; y += 2)
		{
			if (((height - y - 2) * width) < idata.size())
			{
				static unsigned int index1;
				static unsigned int index2;

				index1 = 0 + ((height - y - 1) * width);
				index2 = 0 + ((height - y - 2) * width);

				for (uint32_t x{ 0 }; x < width; ++x)
				{
					setcolorF(data[index1].red, data[index1].green, data[index1].blue);
					setcolorB(data[index2].red, data[index2].green, data[index2].blue);
					std::wcout << BLOCK;

					++index1;
					++index2;
				}
			}

			if (((height - y - 2) * width) >= idata.size())
			{
				static unsigned int index1;

				index1 = 0 + ((height - y - 1) * width);

				setcolorB(0, 0, 0);
				for (uint32_t x{ 0 }; x < width; ++x)
				{
					setcolorF(data[index1].red, data[index1].green, data[index1].blue);
					std::wcout << BLOCK;

					++index1;
				}
			}
			std::wcout << L'\n';
		}

		std::wstring reset{ ESC ,'[','0','m' };
		std::wcout << reset;
		setmode(false);

		auto end = std::time(0);
		auto duration = end - start;

		std::cout << "\nTime taken: " << duration << "\n\n";

		return;
	}*/
}