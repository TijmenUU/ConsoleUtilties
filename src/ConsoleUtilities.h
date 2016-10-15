#ifndef CONSOLE_UTILITIES_H
#define CONSOLE_UTILITIES_H

/*
	ALPHA V0.3
	
	Copyright (c) 2016  Tijmen van Nesselrooij

	Permission is hereby granted, free of charge, to any person obtaining a copy of 
	this software and associated documentation files (the "Software"), to deal in the 
	Software without restriction, including without limitation the rights to use, copy, 
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
	and to permit persons to whom the Software is furnished to do so, subject to the 
	following conditions:

	The above copyright notice and this permission notice shall be included in all 
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	The functions in the ConsoleBasics namespace are designed to provide very
	low level control. The bare minimum. This allows you to create your own
	functionality without having to rewrite the interfacing with the 
	Windows API as much.
*/

#pragma once
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include <Windows.h>

namespace ConsoleBasic
{
	/*
		If any of these functions return false, check
		GetLastError() (Windows.h)

		All functions and methods assume valid parameters, unless
		otherwise specified

		For Debug builds there are ASSERTS included as sanity checks

		For Release builds: Please don't expect my functions to deal
		with absolute bogus arguments (e.g. negative width).

		ASCII == ANSI == 8 bit = 1 byte
		Unicode == UTF-16 == 16 bit == 2 bytes
	*/

	// Attempts to copy the clipboard to given ascii encoded string if
	// the clipboard has text available
	// Returns if succesfull
	static bool SetClipboardContents(std::string input, HWND windowHandle = nullptr)
	{
		if (!OpenClipboard(windowHandle))
			return false;
		if (EmptyClipboard())
		{
			HGLOBAL globalAlloc = GlobalAlloc(GMEM_MOVEABLE, input.size());
			if (!globalAlloc)
			{
				CloseClipboard();
				return false;
			}
			memcpy(GlobalLock(globalAlloc), input.c_str(), input.size());
			GlobalUnlock(globalAlloc);
			if (SetClipboardData(CF_TEXT, globalAlloc) == nullptr)
			{
				CloseClipboard();
				GlobalFree(globalAlloc);
				return false;
			}
			CloseClipboard(); // Not much we can do if this fails...
			GlobalFree(globalAlloc);
			return true;
		}
		else
		{
			CloseClipboard();
			return false;
		}
	}

	// Attempts to copy the clipboard to given unicode encoded string if
	// the clipboard has text available
	// Returns if succesfull
	static bool SetClipboardContents(std::wstring input, HWND windowHandle = nullptr)
	{
		if (!OpenClipboard(windowHandle))
			return false;
		if (EmptyClipboard())
		{
			HGLOBAL globalAlloc = GlobalAlloc(GMEM_MOVEABLE, input.size() * sizeof(wchar_t));
			if (!globalAlloc)
			{
				CloseClipboard();
				return false;
			}
			memcpy(GlobalLock(globalAlloc), input.c_str(), input.size() * sizeof(wchar_t));
			GlobalUnlock(globalAlloc);
			if (SetClipboardData(CF_UNICODETEXT, globalAlloc) == nullptr)
			{
				CloseClipboard();
				GlobalFree(globalAlloc);
				return false;
			}
			CloseClipboard(); // Not much we can do if this fails...
			GlobalFree(globalAlloc);
			return true;
		}
		else
		{
			CloseClipboard();
			return false;
		}
	}

	// Attempts to paste given ascii encoded string to the clipboard in
	// text format
	// Returns if succesfull
	static bool GetClipboardContents(std::string &output, HWND windowHandle = nullptr)
	{
		if (!OpenClipboard(windowHandle))
			return false;
		if (IsClipboardFormatAvailable(CF_TEXT))
		{
			HGLOBAL clipboardData = GetClipboardData(CF_TEXT);
			if (clipboardData != nullptr)
			{
				LPVOID clipboardDataPointer = GlobalLock(clipboardData);
				if (clipboardDataPointer != nullptr)
				{
					output.assign(static_cast<char*>(clipboardDataPointer));
				}
				GlobalUnlock(clipboardData);
				CloseClipboard();
				return true;
			}
		}
		CloseClipboard();
		return false;
	}

	// Attempts to paste given unicode encoded string to the clipboard in
	// text format
	// Returns if succesfull
	static bool GetClipboardContents(std::wstring &output, HWND windowHandle = nullptr)
	{
		if (!OpenClipboard(windowHandle))
			return false;
		if (IsClipboardFormatAvailable(CF_UNICODETEXT))
		{
			HGLOBAL clipboardData = GetClipboardData(CF_UNICODETEXT);
			if (clipboardData != nullptr)
			{
				LPVOID clipboardDataPointer = GlobalLock(clipboardData);
				if (clipboardDataPointer != nullptr)
				{
					output.assign(static_cast<wchar_t*>(clipboardDataPointer));
				}
				GlobalUnlock(clipboardData);
				CloseClipboard();
				return true;
			}
		}
		CloseClipboard();
		return false;
	}

	// Get the current Active Window handle
	// Returns if succesfull
	static bool GetWindowHandle(HWND &windowHandle)
	{
		windowHandle = GetActiveWindow();
		return windowHandle != nullptr;
	}

	// Stores the current input configuration in previousInputBufferConfig
	// and stores the current input handle to consoleInputBuffer with mouse and window inputs
	// You do not have to initialize the consoleInputBuffer handle
	// Returns if succesful
	static bool EnableConsoleInput(HANDLE &consoleInputBuffer, DWORD &previousInputBufferConfig)
	{
		consoleInputBuffer = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(consoleInputBuffer, &previousInputBufferConfig);
		return SetConsoleMode(consoleInputBuffer, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	}

	// Gets the specified number of events from the buffer
	// and stores them in the given INPUT_RECORD array
	// Interesting event types are KEY_EVENT, MOUSE_EVENT and WINDOW_BUFFER_SIZE_EVENT
	// Returns if it was succesful
	static bool GetInput(const HANDLE &consoleInputBuffer, 
		INPUT_RECORD *outputBuffer, 
		int outputBufferSize, 
		unsigned int &itemsRead)
	{
		assert(outputBuffer > 0);

		DWORD readItems = 0;
		if (ReadConsoleInput(consoleInputBuffer, outputBuffer, outputBufferSize, &readItems))
		{
			itemsRead = readItems;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Gets the first 128 events from the buffer (doesn't clear the output vectors!)
	// and stores them in the given vectors
	// Returns if it was succesful
	static bool GetInput(const HANDLE &consoleInputBuffer, 
		std::vector<KEY_EVENT_RECORD> &keyboardEvents,
		std::vector<MOUSE_EVENT_RECORD> &mouseEvents, 
		std::vector<WINDOW_BUFFER_SIZE_RECORD> &windowResizeEvents)
	{
		INPUT_RECORD inputDataBuffer[128];
		unsigned int itemsRead = 0;
		if (GetInput(consoleInputBuffer, &inputDataBuffer[0], 128, itemsRead))
		{
			for (unsigned int i = 0; i < itemsRead; ++i)
			{
				switch (inputDataBuffer[i].EventType)
				{
				case KEY_EVENT: // keyboard input 
				{
					keyboardEvents.push_back(inputDataBuffer[i].Event.KeyEvent);
					break;
				}
				case MOUSE_EVENT: // mouse input 
				{
					mouseEvents.push_back(inputDataBuffer[i].Event.MouseEvent);
					break;
				}
				case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
				{
					windowResizeEvents.push_back(inputDataBuffer[i].Event.WindowBufferSizeEvent);
					break;
				}

				case FOCUS_EVENT:  // disregard focus events 
				case MENU_EVENT:   // disregard menu events 
				default:
				{
					break;
				}
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	// Handle parameter will contain the new handle
	// Returns if succesfull
	static bool InitConsole(HANDLE &consoleScreenBuffer)
	{
		consoleScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
		if (consoleScreenBuffer == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		return SetConsoleActiveScreenBuffer(consoleScreenBuffer);
	}

	// Sets the console screenbuffer to the given handle, returns if succesful
	static bool SetScreenBuffer(const HANDLE &consoleScreenBuffer)
	{
		return SetConsoleActiveScreenBuffer(consoleScreenBuffer);
	}

	// Stores the window data in the second parameter, returns if succesful
	static bool GetConsoleInfo(const HANDLE &consoleScreenBuffer, CONSOLE_SCREEN_BUFFER_INFO &consoleInfo)
	{
		return GetConsoleScreenBufferInfo(consoleScreenBuffer, &consoleInfo);
	}

	// Combines the init, setting screen buffer and fetching console buffer info
	// Returns if succesful
	static bool SetupConsole(HANDLE &consoleScreenBuffer, CONSOLE_SCREEN_BUFFER_INFO &consoleInfo)
	{
		if (InitConsole(consoleScreenBuffer) && SetScreenBuffer(consoleScreenBuffer) && GetConsoleInfo(consoleScreenBuffer, consoleInfo))
			return true;
		else
			return false;
	}

	// Reads what is displayed on screen on a given location, 
	// Returns if succesful, if reading out of bounds it returns false as well
	static bool GetCharFromConsole(const HANDLE &consoleScreenBuffer, 
		const COORD location, 
		CHAR_INFO &outputBuffer)
	{
		assert(location.X >= 0 && location.Y >= 0);

		static const COORD kZeroCoord = { 0, 0 };
		static const COORD kAreaSize = { 1, 1 };

		SMALL_RECT readRegion;
		readRegion.Left = location.X;
		readRegion.Top = location.Y;
		readRegion.Right = location.X + 1;
		readRegion.Bottom = location.Y + 1;

		return ReadConsoleOutput(consoleScreenBuffer, &outputBuffer, kAreaSize, kZeroCoord, &readRegion) && !(readRegion.Left > readRegion.Right || readRegion.Bottom < readRegion.Top);
	}

	// Reads a given area to the outputBuffer(This has to be a two dimensional array of the same size as the readArea!)
	// If you give it an area (partially) outside of bounds the readArea will contain the actual read area
	// Returns false if the read area is completely out of bounds
	// Returns if succesful
	static bool GetRectFromConsole(const HANDLE &consoleScreenBuffer, 
		SMALL_RECT &readArea, 
		CHAR_INFO **outputBuffer)
	{
		assert(readArea.Bottom > readArea.Top && readArea.Right > readArea.Left);

		static const COORD kZeroCoord = { 0, 0 };
		COORD areaSize = { readArea.Right - readArea.Left, readArea.Bottom - readArea.Top }; // {width, height}

		return ReadConsoleOutput(consoleScreenBuffer, *outputBuffer, areaSize, kZeroCoord, &readArea) && !(readArea.Left > readArea.Right || readArea.Bottom < readArea.Top);
	}

	// Reads a given area to the outputBuffer as one continuous string (ASCII)
	// Returns false if area is completely out of bounds
	// The delimiter tells you when the next row starts, last row does not have a trailing delimiter
	// Returns if succesful
	static bool GetRectFromConsole(const HANDLE &consoleScreenBuffer, 
		const int positionX, 
		const int positionY, 
		const int width, 
		const int height, 
		std::string &outputBuffer, 
		char delimiter = '\n')
	{
		assert(width > 0 && height > 0); // Technically the position can be anywhere

		SMALL_RECT readArea;
		readArea.Left = positionX;
		readArea.Top = positionY;
		readArea.Right = positionX + width;
		readArea.Bottom = positionY + height;

		CHAR_INFO **intermediateBuffer = new CHAR_INFO*[width];
		for (int i = 0; i < width; ++i)
		{
			intermediateBuffer[i] = new CHAR_INFO[height];
		}

		if (!GetRectFromConsole(consoleScreenBuffer, readArea, intermediateBuffer))
			return false;

		int actualWidth = readArea.Right - readArea.Left;
		int actualHeight = readArea.Bottom - readArea.Top;

		if (actualWidth < 0 || actualHeight < 0)
		{
			return false;
		}

		for (int x = 0; x < actualWidth; ++x)
		{
			for (int y = 0; y < actualHeight; ++y)
			{
				outputBuffer += intermediateBuffer[x][y].Char.AsciiChar;
			}
			outputBuffer += delimiter;
			delete[] intermediateBuffer[x];
		}
		delete[] intermediateBuffer;

		outputBuffer.erase(outputBuffer.end() - 1); // Delete last delimiter
		return true;
	}

	// Reads a given area to the outputBuffer as one continuous string (Unicode)
	// If you give it an area (partially) outside of bounds the behaviour is undefined
	// The delimiter tells you when the next row starts, last row does not have a trailing delimiter
	// Returns if succesful
	static bool GetRectFromConsole(const HANDLE &consoleScreenBuffer, 
		const int positionX, 
		const int positionY, 
		const int width, 
		const int height, 
		std::wstring &outputBuffer, 
		wchar_t delimiter = '\n')
	{
		assert(height > 0 && width > 0);

		SMALL_RECT readArea;
		readArea.Left = positionX;
		readArea.Top = positionY;
		readArea.Right = positionX + width;
		readArea.Bottom = positionY + height;

		CHAR_INFO **intermediateBuffer = new CHAR_INFO*[width];
		for (int i = 0; i < width; ++i)
		{
			intermediateBuffer[i] = new CHAR_INFO[height];
		}

		bool result = GetRectFromConsole(consoleScreenBuffer, readArea, intermediateBuffer);
		if (!result)
			return false;

		int actualWidth = readArea.Right - readArea.Left;
		int actualHeight = readArea.Bottom - readArea.Top;

		if (actualWidth < 1 || actualHeight < 1)
		{
			return false;
		}

		for (int x = 0; x < actualWidth; ++x)
		{
			for (int y = 0; y < actualHeight; ++y)
			{
				outputBuffer += intermediateBuffer[x][y].Char.UnicodeChar;
			}
			outputBuffer += delimiter;
			delete[] intermediateBuffer[x];
		}
		delete[] intermediateBuffer;

		outputBuffer.erase(outputBuffer.end() - 1); // Delete last delimiter
		return true;
	}

	// Prints to a given location. Won't check if the string fits or not
	// Return value is always true
	static bool WriteToConsole(const HANDLE &consoleScreenBuffer, 
		const char* string, 
		const int stringSize)
	{
		assert(stringSize > 0);

		WriteConsole(consoleScreenBuffer, string, stringSize, 0, 0);
		return true;
	}

	// Prints to a given location. Won't check if the string fits or not
	// Return value is always true
	static bool WriteToConsole(const HANDLE &consoleScreenBuffer, 
		const wchar_t* string, 
		const int stringSize)
	{
		assert(stringSize > 0);
		
		WriteConsole(consoleScreenBuffer, string, stringSize, 0, 0);
		return true;
	}

	// Prints to a given location. Won't check if the string fits or not
	// Return value is always true
	static bool WriteToConsole(const HANDLE &consoleScreenBuffer, const std::string string)
	{
		WriteConsole(consoleScreenBuffer, string.c_str(), string.size(), 0, 0);
		return true;
	}

	// Prints to a given location. Won't check if the string fits or not
	// Return value is always true
	static bool WriteToConsole(const HANDLE &consoleScreenBuffer, const std::wstring string)
	{
		WriteConsole(consoleScreenBuffer, string.c_str(), string.size(), 0, 0);
		return true;
	}

	// Prints to a given location. Won't check if the string fits or not
	// Returns if succesful
	static bool WriteToConsole(const HANDLE &consoleScreenBuffer, 
		const char* string, 
		const int stringSize, 
		const COORD position)
	{
		assert(stringSize > 0);

		if (SetConsoleCursorPosition(consoleScreenBuffer, position))
		{
			WriteConsole(consoleScreenBuffer, string, stringSize, 0, 0);
			return true;
		}
		return false;
	}

	// Prints to a given location. Won't check if the string fits or not
	// Returns if succesful
	static bool WriteToConsole(const HANDLE &consoleScreenBuffer, 
		const wchar_t* string, 
		const int stringSize, 
		const COORD position)
	{
		assert(stringSize > 0);

		if (SetConsoleCursorPosition(consoleScreenBuffer, position))
		{
			WriteConsole(consoleScreenBuffer, string, stringSize, 0, 0);
			return true;
		}
		return false;
	}

	// Prints to a given location. Won't check if the string fits or not
	// Returns if succesful
	static bool WriteToConsole(const HANDLE &consoleScreenBuffer, 
		const std::string string, 
		const COORD position)
	{
		if (SetConsoleCursorPosition(consoleScreenBuffer, position))
		{
			WriteConsole(consoleScreenBuffer, string.c_str(), string.size(), 0, 0);
			return true;
		}
		return false;
	}

	// Prints to a given location. Won't check if the string fits or not
	// Returns if succesful
	static bool WriteToConsole(const HANDLE &consoleScreenBuffer, 
		const std::wstring string, 
		const COORD position)
	{
		if (SetConsoleCursorPosition(consoleScreenBuffer, position))
		{
			WriteConsole(consoleScreenBuffer, string.c_str(), string.size(), 0, 0);
			return true;
		}
		return false;
	}

	// Closes the output handle and restores the input handle created by the console
	// Returns if succesfull
	static bool DeleteConsole(HANDLE &consoleScreenBuffer, 
		HANDLE &inputBuffer, 
		DWORD previousInputBufferConfig)
	{
		return CloseHandle(consoleScreenBuffer) && SetConsoleMode(inputBuffer, previousInputBufferConfig);
	}

	enum class FONTCOLOR {
		BLACK = 0x00,
		DARKBLUE = 0x01,
		DARKGREEN = 0x02,
		DARKCYAN = 0x03,
		DARKRED = 0x04,
		DARKPURPLE = 0x05,
		DARKYELLOW = 0x06,
		GRAY = 0x08,
		LIGHTGRAY = 0x07,
		BLUE = 0x09,
		GREEN = 0x0A,
		CYAN = 0x0B,
		RED = 0x0C,
		PURPLE = 0x0D,
		YELLOW = 0x0E,
		WHITE = 0x0F
	};

	enum class BACKGROUNDCOLOR {
		BLACK = 0x00,
		DARKBLUE = 0x10,
		DARKGREEN = 0x20,
		DARKCYAN = 0x30,
		DARKRED = 0x40,
		DARKPURPLE = 0x50,
		DARKYELLOW = 0x60,
		GRAY = 0x80,
		LIGHTGRAY = 0x70,
		BLUE = 0x90,
		GREEN = 0xA0,
		CYAN = 0xB0,
		RED = 0xC0,
		PURPLE = 0xD0,
		YELLOW = 0xE0,
		WHITE = 0xF0
	};
}

/*
	The functions and classes in the ConsoleHelperObjects are already written
	with a more specific design decision made. I have tried to keep it as 
	broad as possible. It is mainly targeted for people to quickly setup a 
	simple console GUI / TUI: some simple user input and printing with colors

	Currently TODO:
	- Add in ofstream support
	- Add in wide-string support for user input (GetLine / GetLineVisualized)
	- Color entire row 
	- Color entire screen
	- Test, test and test
*/
namespace ConsoleObject
{
	//class 

	class Console
	{
	protected:
		int currentFontColor_;
		int currentBGColor_;
		HANDLE screenBuffer_, inputBuffer_, previousOutputHandle_;
		DWORD previousInputConfig_;
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo_;
	public:
		// Throws an std::exception if something went wrong
		Console()
		{
			inputBuffer_ = GetStdHandle(STD_INPUT_HANDLE);
			previousOutputHandle_ = GetStdHandle(STD_OUTPUT_HANDLE);
			// output
			if (!ConsoleBasic::InitConsole(screenBuffer_))
			{
				throw std::exception("Console: Failed to create a console window.");
			}
			if (!ConsoleBasic::SetScreenBuffer(screenBuffer_))
			{
				CloseHandle(screenBuffer_);
				throw std::exception("Console: Failed to set screenbuffer to active.");
			}
			if (!ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_))
			{
				CloseHandle(screenBuffer_);
				throw std::exception("Console: Failed to get screenbuffer info.");
			}
			// input
			if (!ConsoleBasic::EnableConsoleInput(inputBuffer_, previousInputConfig_))
			{
				CloseHandle(screenBuffer_);
				throw std::exception("Console: Failed to get input handle.");
			}

			SetFontColor(ConsoleBasic::FONTCOLOR::WHITE);
			SetBackgroundColor(ConsoleBasic::BACKGROUNDCOLOR::BLACK);
		}
		// Restores the original i/o handles and closes the used ones
		// Throws and std::exception if something went wrong
		~Console()
		{
			if (!ConsoleBasic::DeleteConsole(screenBuffer_, inputBuffer_, previousInputConfig_))
			{
				std::cerr << "Console: Failed to clean up! Possible open handles left!\n";
			}
		}

		void SetFontColor(ConsoleBasic::FONTCOLOR color)
		{
			currentFontColor_ = static_cast<int>(color);
		}

		void SetBackgroundColor(ConsoleBasic::BACKGROUNDCOLOR color)
		{
			currentBGColor_ = static_cast<int>(color);
		}

		// Classical Print behaviour, prints using stored color info
		void Print(const char* message, const int messageSize)
		{
			assert(messageSize > 0);
			
			SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
			ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, prints using stored color info
		void Print(const wchar_t* message, const int messageSize)
		{
			assert(messageSize > 0);

			SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
			ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, prints using stored color info
		void Print(const std::string message)
		{
			SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
			ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, prints using stored color info
		void Print(const std::wstring message)
		{
			SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
			ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, but does not use the stored color settings
		void Print(const char* message, const int messageSize, DWORD premixedColors)
		{
			assert(messageSize > 0);

			SetConsoleTextAttribute(screenBuffer_, premixedColors);
			ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, but does not use the stored color settings
		void Print(const wchar_t* message, const int messageSize, DWORD premixedColors)
		{
			assert(messageSize > 0);

			SetConsoleTextAttribute(screenBuffer_, premixedColors);
			ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, but does not use the stored color settings
		void Print(const std::string message, DWORD premixedColors)
		{
			SetConsoleTextAttribute(screenBuffer_, premixedColors);
			ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, but does not use the stored color settings
		void Print(const std::wstring message, DWORD premixedColors)
		{
			SetConsoleTextAttribute(screenBuffer_, premixedColors);
			ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, but does not use the stored color settings
		void Print(const char* message, 
			const int messageSize, 
			const ConsoleBasic::FONTCOLOR fontColor, 
			const ConsoleBasic::BACKGROUNDCOLOR backgroundColor)
		{
			SetConsoleTextAttribute(screenBuffer_, (static_cast<int>(fontColor) | static_cast<int>(backgroundColor)));
			ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, but does not use the stored color settings
		void Print(const wchar_t* message, 
			const int messageSize, 
			const ConsoleBasic::FONTCOLOR fontColor, 
			const ConsoleBasic::BACKGROUNDCOLOR backgroundColor)
		{
			SetConsoleTextAttribute(screenBuffer_, (static_cast<int>(fontColor) | static_cast<int>(backgroundColor)));
			ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, but does not use the stored color settings
		void Print(const std::string message, 
			const ConsoleBasic::FONTCOLOR fontColor, 
			const ConsoleBasic::BACKGROUNDCOLOR backgroundColor)
		{
			SetConsoleTextAttribute(screenBuffer_, (static_cast<int>(fontColor) | static_cast<int>(backgroundColor)));
			ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Classical Print behaviour, but does not use the stored color settings
		void Print(const std::wstring message, 
			const ConsoleBasic::FONTCOLOR fontColor, 
			const ConsoleBasic::BACKGROUNDCOLOR backgroundColor)
		{
			SetConsoleTextAttribute(screenBuffer_, (static_cast<int>(fontColor) | static_cast<int>(backgroundColor)));
			ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
			ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
		}

		// Returns the first key down
		// Sleepduration is in ms
		// This method is blocking
		KEY_EVENT_RECORD GetKeyEvent(DWORD sleepduration = 5)
		{
			INPUT_RECORD inputs[8];
			while (true)
			{
				unsigned int itemsRead = 0;
				ConsoleBasic::GetInput(inputBuffer_, &inputs[0], 8, itemsRead);
				for (int i = 0; i < itemsRead; ++i)
				{
					if (inputs[i].EventType == KEY_EVENT)
					{
						return inputs[i].Event.KeyEvent;
					}
				}
				Sleep(sleepduration);
			}
		}

		// Returns the first key down, similar to GETCH, ascii encoding
		// Sleepduration is in ms
		// This method is blocking
		char GetChar(DWORD sleepduration = 5)
		{
			KEY_EVENT_RECORD recorded = GetKeyEvent(sleepduration);
			return recorded.uChar.AsciiChar;
		}

		// Returns the first key down, similar to GETCH, unicode encoding
		// Sleepduration is in ms
		// This method is blocking
		wchar_t GetWChar(DWORD sleepduration = 5)
		{
			KEY_EVENT_RECORD recorded = GetKeyEvent(sleepduration);
			return recorded.uChar.UnicodeChar;
		}

		// Returns an ascii encoded string
		// default value 13 is enter, sleepduration is in ms
		// This method is blocking
		// Can be empty if the first keypress is the delimiter
		std::string GetLine(char delimiter = 13, DWORD sleepduration = 5)
		{
			std::string outputString = "";
			INPUT_RECORD inputs[8];
			unsigned int itemsRead = 0;
			bool waiting = true;
			while (waiting)
			{
				ConsoleBasic::GetInput(inputBuffer_, &inputs[0], 8, itemsRead);
				for (int i = 0; i < itemsRead; ++i)
				{
					// We only want key up events here
					if (inputs[i].EventType == KEY_EVENT && !inputs[i].Event.KeyEvent.bKeyDown)
					{
						char code = inputs[i].Event.KeyEvent.uChar.AsciiChar;
						if (code == delimiter)
							return outputString;
						if (code == 8) // backspace in ASCII
						{
							if(outputString.size() != 0)
								outputString.erase(outputString.end() - 1);
						}	
						else if(code > 31 && code < 126)
							outputString += code;
					}
				}
				Sleep(sleepduration);
			}
		}

		// Returns an unicode encoded string
		// default value 13 is enter, sleepduration is in ms
		// This method is blocking
		// Can be empty if the first keypress is the delimiter
		std::wstring GetWLine(char delimiter = 13, DWORD sleepduration = 5)
		{
			std::wstring outputString;
			INPUT_RECORD inputs[8];
			unsigned int itemsRead = 0;
			bool waiting = true;
			while (waiting)
			{
				ConsoleBasic::GetInput(inputBuffer_, &inputs[0], 8, itemsRead);
				for (int i = 0; i < itemsRead; ++i)
				{
					// We only want key up events here
					if (inputs[i].EventType == KEY_EVENT && !inputs[i].Event.KeyEvent.bKeyDown)
					{
						wchar_t code = inputs[i].Event.KeyEvent.uChar.UnicodeChar;
						if (code == delimiter)
							return outputString;
						if (code == 8) // backspace in ASCII
						{
							if (outputString.size() != 0)
								outputString.erase(outputString.end() - 1);
						}
						else if (code > 31 && code < 126)
							outputString += code;
					}
				}
				Sleep(sleepduration);
			}
		}

		// Returns an ascii encoded string, visualizes the typing process
		// default value 13 is enter, sleepduration is in ms
		// This method is blocking
		// Can be empty if the first keypress is the delimiter
		std::string GetLineVisualized(char delimiter = 13, DWORD sleepduration = 5)
		{
			std::string outputString = "";
			INPUT_RECORD inputs[8];
			unsigned int itemsRead = 0;
			int previousSize = 0;
			bool waiting = true;
			CHAR_INFO oldStyle;
			ConsoleBasic::GetCharFromConsole(screenBuffer_, screenBufferInfo_.dwCursorPosition, oldStyle);

			while (waiting)
			{
				ConsoleBasic::GetInput(inputBuffer_, &inputs[0], 8, itemsRead);
				int currentSize = previousSize;
				for (int i = 0; i < itemsRead; ++i)
				{
					// We only want key up events here
					if (inputs[i].EventType == KEY_EVENT && !inputs[i].Event.KeyEvent.bKeyDown)
					{
						char code = inputs[i].Event.KeyEvent.uChar.AsciiChar;
						if (code == delimiter)
							return outputString;
						if (code == 8) // backspace in ASCII
						{
							if (outputString.size() != 0)
							{
								outputString.erase(outputString.end() - 1);
								--currentSize;
							}	
						}	
						else if (code > 31 && code < 126) // We ignore the non-character codes
						{
							outputString += code;
							++currentSize;
						}	
					}
				}
				int difference = currentSize - previousSize;
				if (difference > 0) // characters added
				{
					Print(outputString.substr(outputString.size() - difference, difference));
				}
				else if (difference < 0) // characters removed
				{
					std::string filler = ""; 
					for (int i = 0; i > difference; --i)
					{
						filler += "\b \b";
					}
					Print(filler, oldStyle.Attributes);
				}
				Sleep(sleepduration);
			}
		}

		// Returns an unicode encoded string, visualizes the typing process
		// default value 13 is enter, sleepduration is in ms
		// This method is blocking
		// Can be empty if the first keypress is the delimiter
		std::wstring GetWLineVisualized(char delimiter = 13, DWORD sleepduration = 5)
		{
			std::wstring outputString;
			INPUT_RECORD inputs[8];
			unsigned int itemsRead = 0;
			int previousSize = 0;
			bool waiting = true;
			CHAR_INFO oldStyle;
			ConsoleBasic::GetCharFromConsole(screenBuffer_, screenBufferInfo_.dwCursorPosition, oldStyle);

			while (waiting)
			{
				ConsoleBasic::GetInput(inputBuffer_, &inputs[0], 8, itemsRead);
				int currentSize = previousSize;
				for (int i = 0; i < itemsRead; ++i)
				{
					// We only want key up events here
					if (inputs[i].EventType == KEY_EVENT && !inputs[i].Event.KeyEvent.bKeyDown)
					{
						wchar_t code = inputs[i].Event.KeyEvent.uChar.UnicodeChar;
						if (code == delimiter)
							return outputString;
						if (code == 8) // backspace in Unicode
						{
							if (outputString.size() != 0)
							{
								outputString.erase(outputString.end() - 1);
								--currentSize;
							}
						}
						else if (code > 31 && code < 126) // We ignore the non-character codes
						{
							outputString += code;
							++currentSize;
						}
					}
				}
				int difference = currentSize - previousSize;
				if (difference > 0) // characters added
				{
					Print(outputString.substr(outputString.size() - difference, difference));
				}
				else if (difference < 0) // characters removed
				{
					std::string filler = "";
					for (int i = 0; i > difference; --i)
					{
						filler += "\b \b";
					}
					Print(filler, oldStyle.Attributes);
				}
				Sleep(sleepduration);
			}
		}
		
		// Handle input streams <<
		Console& operator << (const char s)
		{
			Print(&s, 1);
			return *this;
		}

		Console& operator << (const wchar_t s)
		{
			Print(&s, 1);
			return *this;
		}

		Console& operator << (const char* s)
		{
			Print(s);
			return *this;
		}

		Console& operator << (const wchar_t* s)
		{
			Print(s);
			return *this;
		}

		Console& operator << (const std::string &s)
		{
			Print(s);
			return *this;
		}

		Console& operator << (const std::wstring &s)
		{
			Print(s);
			return *this;
		}

		// TODO differentiate between ascii and unicode strings
		Console& operator << (std::istream &in)
		{
			Print(std::string(std::istreambuf_iterator<char>(in), {}));
			return *this;
		}

		// TODO output streams >>

		// Getters and setters
		void SetCursorPosition(const int x, const int y)
		{
			screenBufferInfo_.dwCursorPosition.X = x;
			screenBufferInfo_.dwCursorPosition.Y = y;
		}

		void SetCursorPosition(const COORD position)
		{
			screenBufferInfo_.dwCursorPosition = position;
		}

		COORD GetCursorPosition() const
		{
			return screenBufferInfo_.dwCursorPosition;
		}

		void GetCursorPosition(int &x, int &y)
		{
			x = screenBufferInfo_.dwCursorPosition.X;
			y = screenBufferInfo_.dwCursorPosition.Y;
		}

		// Do not use this if you don't know what you're doing
		HANDLE _GetInputHandle() const
		{
			return inputBuffer_;
		}

		// Do not use this if you don't know what you're doing
		HANDLE _GetOutputHandle() const
		{
			return screenBuffer_;
		}
	};
}

#endif
