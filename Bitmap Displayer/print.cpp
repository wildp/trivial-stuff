#include <cstdint>
#include <iostream>
#include <string>

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

	void extendcode(std::wstring &existingcode, unsigned char addition, bool noSemicolon = false)
	{
		existingcode.append(std::to_wstring(static_cast<int>((addition))));
		if (noSemicolon == false)
			existingcode.append(L";");
		return;
	}

	void setcolor(uint8_t red, uint8_t green, uint8_t blue, bool isBackground)
	{
		static auto status{ enableVT() }; 
		// lazy method to ensure only one execution of enableVT()

		std::wstring output{ ESC ,'[' };

		if (isBackground == true)
			output.append(L"48;2;");
		else
			output.append(L"38;2;");

		extendcode(output, red);
		extendcode(output, green);
		extendcode(output, blue, true);
		output.append(L"m");

		std::wcout << output;

		return;
	}

	void setmode(bool isWide)
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

	void bitmap24(const bmp::image &image, bmp::pixeldata &data)
	{
		setmode(true);
		uint32_t width{ image.info.width };
		uint32_t height{ image.info.height };

		// The extracted data in &data has been inverted vertically
		for (uint32_t y{ 0 }; y < height; y += 2)
		{
			for (uint32_t x{ 0 }; x < width; ++x)
			{
				static int64_t index1;
				static int64_t index2;

				index1 =  x + ((height - y - 1) * width);
				index2 =  x + ((height - y - 2) * width);

				setcolor(data[index1].red, data[index1].green, data[index1].blue, false);

				if (index2 < data.size())
					setcolor(data[index2].red, data[index2].green, data[index2].blue, true);
				else
					setcolor(0, 0, 0, true);

				std::wcout << BLOCK;
			}
			std::wcout << L'\n';
		}

		std::wstring reset{ ESC ,'[','0','m' };
		std::wcout << reset;
		setmode(false);
		return;
	}

	void setup()
	{
#ifdef _WIN32
		SetConsoleTitle("Bitmap Displayer");
#endif
		return;
	}
}
