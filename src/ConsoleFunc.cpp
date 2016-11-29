/*
ALPHA V0.5

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

#include "ConsoleFunc.h"

namespace ConsoleFunc
{
	bool SetClipboardContents(std::string input, HWND windowHandle)
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

	bool SetClipboardContents(std::wstring input, HWND windowHandle)
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
	
	bool GetClipboardContents(std::string & output, HWND windowHandle)
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

	bool GetClipboardContents(std::wstring & output, HWND windowHandle)
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

	bool GetWindowHandle(HWND & windowHandle)
	{
		windowHandle = GetActiveWindow();
		return windowHandle != nullptr;
	}

	bool EnableConsoleInput(HANDLE & consoleInput, DWORD & previousInputBufferConfig)
	{
		consoleInput = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(consoleInput, &previousInputBufferConfig);
		return SetConsoleMode(consoleInput, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	}

	bool InitConsole(HANDLE & consoleScreen, HANDLE & out_previousOutputHandle)
	{
		
		consoleScreen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
		if (consoleScreen == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		return SetScreenBuffer(consoleScreen, out_previousOutputHandle);
	}

	bool SetScreenBuffer(const HANDLE & consoleScreen, HANDLE & out_previousOutputHandle)
	{
		out_previousOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		return SetConsoleActiveScreenBuffer(consoleScreen);
	}

	bool GetConsoleInfo(const HANDLE & consoleScreen, CONSOLE_SCREEN_BUFFER_INFO & consoleInfo)
	{
		return GetConsoleScreenBufferInfo(consoleScreen, &consoleInfo);
	}

	bool SetupConsole(HANDLE & consoleScreen, CONSOLE_SCREEN_BUFFER_INFO & consoleInfo, HANDLE & out_previousOutputHandle)
	{
		if (InitConsole(consoleScreen, out_previousOutputHandle) && GetConsoleInfo(consoleScreen, consoleInfo))
			return true;
		else
			return false;
	}

	bool DestroyConsole(HANDLE & consoleScreen, HANDLE & previousOutputHandle)
	{
		return SetStdHandle(STD_OUTPUT_HANDLE, previousOutputHandle) && CloseHandle(consoleScreen);
	}

	bool GetCharFromConsole(const HANDLE & consoleScreen, const COORD location, CHAR_INFO & outputBuffer)
	{
		assert(location.X >= 0 && location.Y >= 0);

		static const COORD kZeroCoord = { 0, 0 };
		static const COORD kAreaSize = { 1, 1 };

		SMALL_RECT readRegion;
		readRegion.Left = location.X;
		readRegion.Top = location.Y;
		readRegion.Right = location.X + 1;
		readRegion.Bottom = location.Y + 1;

		return ReadConsoleOutput(consoleScreen, &outputBuffer, kAreaSize, kZeroCoord, &readRegion) && !(readRegion.Left > readRegion.Right || readRegion.Bottom < readRegion.Top);
	}

	bool GetRectFromConsole(const HANDLE & consoleScreen, SMALL_RECT & readArea, CHAR_INFO ** outputBuffer)
	{
		assert(readArea.Bottom > readArea.Top && readArea.Right > readArea.Left);

		static const COORD kZeroCoord = { 0, 0 };
		COORD areaSize = { readArea.Right - readArea.Left, readArea.Bottom - readArea.Top }; // {width, height}

		return ReadConsoleOutput(consoleScreen, *outputBuffer, areaSize, kZeroCoord, &readArea) && !(readArea.Left > readArea.Right || readArea.Bottom < readArea.Top);
	}

	bool GetRectFromConsole(const HANDLE & consoleScreen, const int positionX, const int positionY, const int width, const int height, std::string & outputBuffer, char delimiter)
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

		if (!GetRectFromConsole(consoleScreen, readArea, intermediateBuffer))
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

	bool GetRectFromConsole(const HANDLE & consoleScreen, const int positionX, const int positionY, const int width, const int height, std::wstring & outputBuffer, wchar_t delimiter)
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

		bool result = GetRectFromConsole(consoleScreen, readArea, intermediateBuffer);
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

	bool WriteToConsole(const HANDLE & consoleScreen, const char * string, const int stringSize)
	{
		assert(stringSize > 0);

		WriteConsole(consoleScreen, string, stringSize, 0, 0);
		return true;
	}

	bool WriteToConsole(const HANDLE & consoleScreen, const wchar_t * string, const int stringSize)
	{
		assert(stringSize > 0);

		WriteConsole(consoleScreen, string, stringSize, 0, 0);
		return true;
	}

	bool WriteToConsole(const HANDLE & consoleScreen, const std::string string)
	{
		WriteConsole(consoleScreen, string.c_str(), string.size(), 0, 0);
		return true;
	}

	bool WriteToConsole(const HANDLE & consoleScreen, const std::wstring string)
	{
		WriteConsole(consoleScreen, string.c_str(), string.size(), 0, 0);
		return true;
	}

	bool WriteToConsole(const HANDLE & consoleScreen, const char * string, const int stringSize, const COORD position)
	{
		assert(stringSize > 0);

		if (SetConsoleCursorPosition(consoleScreen, position))
		{
			WriteConsole(consoleScreen, string, stringSize, 0, 0);
			return true;
		}
		return false;
	}

	bool WriteToConsole(const HANDLE & consoleScreen, const wchar_t * string, const int stringSize, const COORD position)
	{
		assert(stringSize > 0);

		if (SetConsoleCursorPosition(consoleScreen, position))
		{
			WriteConsole(consoleScreen, string, stringSize, 0, 0);
			return true;
		}
		return false;
	}

	bool WriteToConsole(const HANDLE & consoleScreen, const std::string string, const COORD position)
	{
		if (SetConsoleCursorPosition(consoleScreen, position))
		{
			WriteConsole(consoleScreen, string.c_str(), string.size(), 0, 0);
			return true;
		}
		return false;
	}

	bool WriteToConsole(const HANDLE & consoleScreen, const std::wstring string, const COORD position)
	{
		if (SetConsoleCursorPosition(consoleScreen, position))
		{
			WriteConsole(consoleScreen, string.c_str(), string.size(), 0, 0);
			return true;
		}
		return false;
	}

	bool GetInput(const HANDLE & consoleInput, INPUT_RECORD * outputBuffer, int outputBufferSize, unsigned int & itemsRead)
	{
		assert(outputBuffer > 0);

		DWORD readItems = 0;
		if (ReadConsoleInput(consoleInput, outputBuffer, outputBufferSize, &readItems))
		{
			itemsRead = readItems;
			return true;
		}
		else
		{
			return false;
		}
	}

	KEY_EVENT_RECORD GetKeyEvent(const HANDLE consoleInput, DWORD sleepduration)
	{
		INPUT_RECORD inputs[8];
		while (true)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputs[0], 8, itemsRead);
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

	KEY_EVENT_RECORD GetKeyEvent(const HANDLE consoleInput, INPUT_RECORD * inputBuffer, const int inputBufferLength, DWORD sleepduration)
	{
		while (true)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputBuffer[0], inputBufferLength, itemsRead);
			for (int i = 0; i < itemsRead; ++i)
			{
				if (inputBuffer[i].EventType == KEY_EVENT)
				{
					return inputBuffer[i].Event.KeyEvent;
				}
			}
			Sleep(sleepduration);
		}
	}

	char GetChar(const HANDLE consoleInput, DWORD sleepduration)
	{
		KEY_EVENT_RECORD recorded = GetKeyEvent(consoleInput, sleepduration);
		return recorded.uChar.AsciiChar;
	}

	char GetChar(const HANDLE consoleInput, INPUT_RECORD * inputBuffer, const int inputBufferLength, DWORD sleepduration)
	{
		KEY_EVENT_RECORD recorded = GetKeyEvent(consoleInput, inputBuffer, inputBufferLength, sleepduration);
		return recorded.uChar.AsciiChar;
	}

	wchar_t GetWChar(const HANDLE consoleInput, DWORD sleepduration)
	{
		KEY_EVENT_RECORD recorded = GetKeyEvent(consoleInput, sleepduration);
		return recorded.uChar.UnicodeChar;
	}

	wchar_t GetWChar(const HANDLE consoleInput, INPUT_RECORD * inputBuffer, const int inputBufferLength, DWORD sleepduration)
	{
		KEY_EVENT_RECORD recorded = GetKeyEvent(consoleInput, inputBuffer, inputBufferLength, sleepduration);
		return recorded.uChar.UnicodeChar;
	}

	void WaitOnKey(const HANDLE consoleInput, DWORD sleepDuration)
	{
		FlushConsoleInputBuffer(consoleInput);
		GetWChar(consoleInput, sleepDuration);
	}

	std::string GetLine(const HANDLE consoleInput, char delimiter, DWORD sleepduration)
	{
		std::string outputString = "";
		INPUT_RECORD inputBuffer[8];
		bool waiting = true;
		while (waiting)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputBuffer[0], 8, itemsRead);
			for (int i = 0; i < itemsRead; ++i)
			{
				// We only want key up events here
				if (inputBuffer[i].EventType == KEY_EVENT && !inputBuffer[i].Event.KeyEvent.bKeyDown)
				{
					char code = inputBuffer[i].Event.KeyEvent.uChar.AsciiChar;
					if (code == delimiter)
						return outputString;
					if (code == 8) // backspace in ASCII
					{
						if (outputString.size() != 0)
							outputString.erase(outputString.end() - 1);
					}
					else if (code > 31)
						outputString += code;
				}
			}
			Sleep(sleepduration);
		}
	}

	std::string GetLine(const HANDLE consoleInput, INPUT_RECORD * inputs, const int inputsLength, char delimiter, DWORD sleepduration)
	{
		assert(inputsLength > 0);

		std::string outputString = "";
		bool waiting = true;
		while (waiting)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputs[0], inputsLength, itemsRead);
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
							outputString.erase(outputString.end() - 1);
					}
					else if (code > 31)
						outputString += code;
				}
			}
			Sleep(sleepduration);
		}
	}

	std::wstring GetWLine(const HANDLE consoleInput, char delimiter, DWORD sleepduration)
	{
		std::wstring outputString = L"";
		INPUT_RECORD inputBuffer[8];
		bool waiting = true;
		while (waiting)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputBuffer[0], 8, itemsRead);
			for (int i = 0; i < itemsRead; ++i)
			{
				// We only want key up events here
				if (inputBuffer[i].EventType == KEY_EVENT && !inputBuffer[i].Event.KeyEvent.bKeyDown)
				{
					wchar_t code = inputBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
					if (code == delimiter)
						return outputString;
					if (code == 8) // backspace
					{
						if (outputString.size() != 0)
							outputString.erase(outputString.end() - 1);
					}
					else if (code > 31)
						outputString += code;
				}
			}
			Sleep(sleepduration);
		}
	}

	std::wstring GetWLine(const HANDLE consoleInput, INPUT_RECORD * inputBuffer, const int inputsLength, char delimiter, DWORD sleepduration)
	{
		assert(inputsLength > 0);

		std::wstring outputString = L"";
		bool waiting = true;
		while (waiting)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputBuffer[0], inputsLength, itemsRead);
			for (int i = 0; i < itemsRead; ++i)
			{
				// We only want key up events here
				if (inputBuffer[i].EventType == KEY_EVENT && !inputBuffer[i].Event.KeyEvent.bKeyDown)
				{
					wchar_t code = inputBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
					if (code == delimiter)
						return outputString;
					if (code == 8) // backspace
					{
						if (outputString.size() != 0)
							outputString.erase(outputString.end() - 1);
					}
					else if (code > 31)
						outputString += code;
				}
			}
			Sleep(sleepduration);
		}
	}

	std::string GetLineVisualized(const HANDLE consoleScreen, const HANDLE consoleInput, const CONSOLE_SCREEN_BUFFER_INFO consoleInfo, char delimiter, DWORD sleepduration)
	{
		std::string outputString = "";
		INPUT_RECORD inputBuffer[8];
		int previousSize = 0;
		bool waiting = true;
		CHAR_INFO oldStyle;
		GetCharFromConsole(consoleScreen, consoleInfo.dwCursorPosition, oldStyle);

		while (waiting)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputBuffer[0], 8, itemsRead);
			int currentSize = previousSize;
			for (int i = 0; i < itemsRead; ++i)
			{
				// We only want key up events here
				if (inputBuffer[i].EventType == KEY_EVENT && !inputBuffer[i].Event.KeyEvent.bKeyDown)
				{
					char code = inputBuffer[i].Event.KeyEvent.uChar.AsciiChar;
					if (code == delimiter)
					{
						return outputString;
					}	
					if (code == 8) // backspace in ASCII
					{
						if (outputString.size() != 0)
						{
							outputString.erase(outputString.end() - 1);
							--currentSize;
						}
					}
					else if (code > 31) // We ignore the non-character codes
					{
						outputString += code;
						++currentSize;
					}
				}
			}
			int difference = currentSize - previousSize;
			if (difference > 0) // characters added
			{
				WriteToConsole(consoleScreen, outputString.substr(outputString.size() - difference, difference));
			}
			else if (difference < 0) // characters removed
			{
				CHAR_INFO currentStyle;
				GetCharFromConsole(consoleScreen, consoleInfo.dwCursorPosition, currentStyle);

				SetConsoleTextAttribute(consoleScreen, oldStyle.Attributes);
				std::string filler = "";
				for (int i = 0; i > difference; --i)
				{
					filler += "\b \b";
				}
				WriteToConsole(consoleScreen, filler);

				SetConsoleTextAttribute(consoleScreen, currentStyle.Attributes);
			}
			Sleep(sleepduration);
		}
	}

	std::string GetLineVisualized(const HANDLE consoleScreen, const HANDLE consoleInput, const CONSOLE_SCREEN_BUFFER_INFO consoleInfo, INPUT_RECORD * inputBuffer, const int inputBufferLength, char delimiter, DWORD sleepduration)
	{
		std::string outputString = "";
		int previousSize = 0;
		bool waiting = true;
		CHAR_INFO oldStyle;
		GetCharFromConsole(consoleScreen, consoleInfo.dwCursorPosition, oldStyle);

		while (waiting)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputBuffer[0], inputBufferLength, itemsRead);
			int currentSize = previousSize;
			for (int i = 0; i < itemsRead; ++i)
			{
				// We only want key up events here
				if (inputBuffer[i].EventType == KEY_EVENT && !inputBuffer[i].Event.KeyEvent.bKeyDown)
				{
					char code = inputBuffer[i].Event.KeyEvent.uChar.AsciiChar;
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
					else if (code > 31) // We ignore the non-character codes
					{
						outputString += code;
						++currentSize;
					}
				}
			}
			int difference = currentSize - previousSize;
			if (difference > 0) // characters added
			{
				WriteToConsole(consoleScreen, outputString.substr(outputString.size() - difference, difference));
			}
			else if (difference < 0) // characters removed
			{
				CHAR_INFO currentStyle;
				GetCharFromConsole(consoleScreen, consoleInfo.dwCursorPosition, currentStyle);

				SetConsoleTextAttribute(consoleScreen, oldStyle.Attributes);
				std::string filler = "";
				for (int i = 0; i > difference; --i)
				{
					filler += "\b \b";
				}
				WriteToConsole(consoleScreen, filler);

				SetConsoleTextAttribute(consoleScreen, currentStyle.Attributes);
			}
			Sleep(sleepduration);
		}
	}

	std::wstring GetWLineVisualized(const HANDLE consoleScreen, const HANDLE consoleInput, const CONSOLE_SCREEN_BUFFER_INFO consoleInfo, char delimiter, DWORD sleepduration)
	{
		std::wstring outputString;
		INPUT_RECORD inputBuffer[8];
		int previousSize = 0;
		bool waiting = true;
		CHAR_INFO oldStyle;
		GetCharFromConsole(consoleScreen, consoleInfo.dwCursorPosition, oldStyle);

		while (waiting)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputBuffer[0], 8, itemsRead);
			int currentSize = previousSize;
			for (int i = 0; i < itemsRead; ++i)
			{
				// We only want key up events here
				if (inputBuffer[i].EventType == KEY_EVENT && !inputBuffer[i].Event.KeyEvent.bKeyDown)
				{
					wchar_t code = inputBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
					if (code == delimiter)
						return outputString;
					if (code == 8) // backspace
					{
						if (outputString.size() != 0)
						{
							outputString.erase(outputString.end() - 1);
							--currentSize;
						}
					}
					else if (code > 31) // We ignore the non-character codes
					{
						outputString += code;
						++currentSize;
					}
				}
			}
			int difference = currentSize - previousSize;
			if (difference > 0) // characters added
			{
				WriteToConsole(consoleScreen, outputString.substr(outputString.size() - difference, difference));
			}
			else if (difference < 0) // characters removed
			{
				CHAR_INFO currentStyle;
				GetCharFromConsole(consoleScreen, consoleInfo.dwCursorPosition, currentStyle);

				SetConsoleTextAttribute(consoleScreen, oldStyle.Attributes);
				std::string filler = "";
				for (int i = 0; i > difference; --i)
				{
					filler += "\b \b";
				}
				WriteToConsole(consoleScreen, filler);

				SetConsoleTextAttribute(consoleScreen, currentStyle.Attributes);
			}
			Sleep(sleepduration);
		}
	}

	std::wstring GetWLineVisualized(const HANDLE consoleScreen, const HANDLE consoleInput, const CONSOLE_SCREEN_BUFFER_INFO consoleInfo, INPUT_RECORD * inputBuffer, const int inputBufferLength, char delimiter, DWORD sleepduration)
	{
		std::wstring outputString;

		int previousSize = 0;
		bool waiting = true;
		CHAR_INFO oldStyle;
		GetCharFromConsole(consoleScreen, consoleInfo.dwCursorPosition, oldStyle);

		while (waiting)
		{
			unsigned int itemsRead = 0;
			GetInput(consoleInput, &inputBuffer[0], inputBufferLength, itemsRead);
			int currentSize = previousSize;
			for (int i = 0; i < itemsRead; ++i)
			{
				// We only want key up events here
				if (inputBuffer[i].EventType == KEY_EVENT && !inputBuffer[i].Event.KeyEvent.bKeyDown)
				{
					wchar_t code = inputBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
					if (code == delimiter)
						return outputString;
					if (code == 8) // backspace
					{
						if (outputString.size() != 0)
						{
							outputString.erase(outputString.end() - 1);
							--currentSize;
						}
					}
					else if (code > 31) // We ignore the non-character codes
					{
						outputString += code;
						++currentSize;
					}
				}
			}
			int difference = currentSize - previousSize;
			if (difference > 0) // characters added
			{
				WriteToConsole(consoleScreen, outputString.substr(outputString.size() - difference, difference));
			}
			else if (difference < 0) // characters removed
			{
				CHAR_INFO currentStyle;
				GetCharFromConsole(consoleScreen, consoleInfo.dwCursorPosition, currentStyle);

				SetConsoleTextAttribute(consoleScreen, oldStyle.Attributes);
				std::string filler = "";
				for (int i = 0; i > difference; --i)
				{
					filler += "\b \b";
				}
				WriteToConsole(consoleScreen, filler);

				SetConsoleTextAttribute(consoleScreen, currentStyle.Attributes);
			}
			Sleep(sleepduration);
		}
	}

	bool DestroyInputHandle(HANDLE & inputBuffer, DWORD previousInputBufferConfig)
	{
		return SetConsoleMode(inputBuffer, previousInputBufferConfig);
	}

	unsigned int CreateColorAttributes(FONTCOLOR fontColor, BACKGROUNDCOLOR bgColor)
	{
		return static_cast<unsigned int>(fontColor) | static_cast<unsigned int>(bgColor);
	}

	bool HideConsoleCursor(HANDLE consoleScreen)
	{
		CONSOLE_CURSOR_INFO cursorInfo;
		if (GetConsoleCursorInfo(consoleScreen, &cursorInfo))
		{
			cursorInfo.bVisible = false;
			if (SetConsoleCursorInfo(consoleScreen, &cursorInfo))
			{
				return true;
			}
		}

		return false;
	}

	bool ShowConsoleCursor(HANDLE consoleScreen)
	{
		CONSOLE_CURSOR_INFO cursorInfo;
		if (GetConsoleCursorInfo(consoleScreen, &cursorInfo))
		{
			cursorInfo.bVisible = true;
			if (SetConsoleCursorInfo(consoleScreen, &cursorInfo))
			{
				return true;
			}
		}

		return false;
	}

}
