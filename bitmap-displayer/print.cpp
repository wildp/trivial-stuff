#include <cstdint>
#include <iostream>
#include <locale>
#include <string>

//#include <chrono>

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
#define BLOCK L'\u2580'


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
					std::wcerr << "Unable to set VT mode.\n";
					return -1;
				}
			}

			//DWORD InMode = OriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
			//if (!SetConsoleMode(In, InMode))
			//{
			//	std::wcerr << "Unable to set VT mode.\n";
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
	#else
		static auto status
		{
			[](){
				std::locale::global(std::locale(""));
				std::wcout << ESC << "]0;Bitmap Displayer\x07";
				return true;
			}()
		};
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
	
	inline void fillcode(std::wstring &existingcode, bmp::colorBGRA color)
	{
		extendcodeS(existingcode, color.red);
		extendcodeS(existingcode, color.green);
		extendcode(existingcode, color.blue);
		existingcode.append(L"m");
		return;
	}

	inline std::wstring getcolorFG(bmp::colorBGRA color)
	{
		std::wstring output{ ESC,'[','3','8',';','2',';' };
		fillcode(output, color);
		return output;
	}

	inline std::wstring getcolorBG(bmp::colorBGRA color)
	{
		std::wstring output{ ESC,'[','4','8',';','2',';' };
		fillcode(output, color);
		return output;
	}

	inline void setcolorFG(bmp::colorBGRA color)
	{
		std::wcout << getcolorFG(color);
		return;
	}
	
	inline void setcolorBG(bmp::colorBGRA color)
	{
		std::wcout << getcolorBG(color);
		return;
	}

	inline void resetcolor()
	{
		static std::wstring reset{ ESC ,'[','0','m' };
		std::wcout << reset;
	}

	inline void setmode(bool isWide)
	{
#ifdef _WIN32
		std::wcerr << std::flush;
		std::wcout << std::flush;
	
		if (isWide == true)
		{

			_setmode(_fileno(stdout), _O_U16TEXT);
		}
		else
		{
			_setmode(_fileno(stdout), _O_TEXT);
		}
	#endif
		return;
	}
	
	void bitmap(const bmp::image &image, bmp::pixeldata &data)
	{
		//auto start{ std::chrono::system_clock::now() };
	
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
					setcolorFG(data[index1]);
					setcolorBG(data[index2]);
					std::wcout << BLOCK;
	
					++index1;
					++index2;
				}
			}
	
			if (((height - y - 2) * width) >= data.size())
			{
				static unsigned int index1;
	
				index1 = 0 + ((height - y - 1) * width);
	
				for (uint32_t x{ 0 }; x < width; ++x)
				{
					setcolorFG(data[index1]);
					std::wcout << BLOCK;
	
					++index1;
				}
			}
			
			resetcolor();
			std::wcout << L'\n';
		}
	
		setmode(false);
	
		// auto end = std::chrono::system_clock::now();
		// auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	
		// std::cout << "\nTime taken: " << duration.count() << " ms\n\n";
	
		return;
	}
	
	void bitmap(const bmp::image &image, bmp::i1pixeldata &idata)
	{
		//auto start{ std::chrono::system_clock::now() };

		setmode(true);
		uint32_t width{ image.info.width };
		uint32_t height{ image.info.height };

		std::wstring colorFG[2]{ getcolorFG(image.ctab[0]), getcolorFG(image.ctab[1]) };
		std::wstring colorBG[2]{ getcolorBG(image.ctab[0]), getcolorBG(image.ctab[1]) };

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
					std::wcout << colorFG[idata[index1]] << colorBG[idata[index2]]  << BLOCK;
					++index1;
					++index2;
				}
			}

			if (((height - y - 2) * width) >= idata.size())
			{
				static unsigned int index1;
				index1 = 0 + ((height - y - 1) * width);

				for (uint32_t x{ 0 }; x < width; ++x)
				{
					std::wcout << colorFG[idata[index1]] << BLOCK;
					++index1;
				}
			}

			resetcolor();
			std::wcout << L'\n';
		}
		
		setmode(false);

		// auto end = std::chrono::system_clock::now();
		// auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// std::cout << "\nTime taken: " << duration.count() << " ms\n\n";

		return;
	}
}
