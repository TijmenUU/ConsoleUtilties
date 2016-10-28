#ifndef CONSOLEFUNC_H
#define CONSOLEFUNC_H
#pragma once 

/*
ALPHA V0.4

Copyright (c) 2016 Tijmen van Nesselrooij

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

#include <cassert>
#include <memory>
#include <vector>
#include <Windows.h>

namespace ConsoleFunc
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

#pragma region Clipboard utils
	// Attempts to copy the clipboard to given ascii encoded string if
	// the clipboard has text available
	// Returns if succesfull
	bool SetClipboardContents(std::string input, HWND windowHandle = nullptr);

	// Attempts to copy the clipboard to given unicode encoded string if
	// the clipboard has text available
	// Returns if succesfull
	bool SetClipboardContents(std::wstring input, HWND windowHandle = nullptr);

	// Attempts to paste given ascii encoded string to the clipboard in
	// text format
	// Returns if succesfull
	bool GetClipboardContents(std::string & output, HWND windowHandle = nullptr);

	// Attempts to paste given unicode encoded string to the clipboard in
	// text format
	// Returns if succesfull
	bool GetClipboardContents(std::wstring & output, HWND windowHandle = nullptr);

#pragma endregion
	// Get the current Active Window handle
	// Returns if succesfull
	bool GetWindowHandle(HWND & windowHandle);

	// Stores the current input configuration in previousInputBufferConfig
	// and stores the current input handle to consoleInputBuffer with mouse and window inputs
	// You do not have to initialize the consoleInputBuffer handle
	// Returns if succesful
	bool EnableConsoleInput(HANDLE & consoleInputBuffer, DWORD & previousInputBufferConfig);

	// Handle parameter will contain the new handle
	// Returns if succesfull
	bool InitConsole(HANDLE & consoleScreenBuffer);

	// Sets the console screenbuffer to the given handle, returns if succesful
	bool SetScreenBuffer(const HANDLE & consoleScreenBuffer);

	// Stores the window data in the second parameter, returns if succesful
	bool GetConsoleInfo(const HANDLE & consoleScreenBuffer, CONSOLE_SCREEN_BUFFER_INFO & consoleInfo);

	// Combines the init, setting screen buffer and fetching console buffer info
	// Returns if succesful
	bool SetupConsole(HANDLE & consoleScreenBuffer, CONSOLE_SCREEN_BUFFER_INFO & consoleInfo);

#pragma region Console Input
	// Reads what is displayed on screen on a given location, 
	// Returns if succesful, if reading out of bounds it returns false as well
	bool GetCharFromConsole(const HANDLE & consoleScreenBuffer,
		const COORD location,
		CHAR_INFO & outputBuffer);

	// Reads a given area to the outputBuffer(This has to be a two dimensional array of the same size as the readArea!)
	// If you give it an area (partially) outside of bounds the readArea will contain the actual read area
	// Returns false if the read area is completely out of bounds
	// Returns if succesful
	bool GetRectFromConsole(const HANDLE & consoleScreenBuffer,
		SMALL_RECT & readArea,
		CHAR_INFO ** outputBuffer);

	// Reads a given area to the outputBuffer as one continuous string (ASCII)
	// Returns false if area is completely out of bounds
	// The delimiter tells you when the next row starts, last row does not have a trailing delimiter
	// Returns if succesful
	bool GetRectFromConsole(const HANDLE & consoleScreenBuffer,
		const int positionX,
		const int positionY,
		const int width,
		const int height,
		std::string & outputBuffer,
		char delimiter = '\n');

	// Reads a given area to the outputBuffer as one continuous string (Unicode)
	// If you give it an area (partially) outside of bounds the behaviour is undefined
	// The delimiter tells you when the next row starts, last row does not have a trailing delimiter
	// Returns if succesful
	bool GetRectFromConsole(const HANDLE & consoleScreenBuffer,
		const int positionX,
		const int positionY,
		const int width,
		const int height,
		std::wstring & outputBuffer,
		wchar_t delimiter = '\n');
#pragma endregion
#pragma region Console Output
	// Prints to a given location. Won't check if the string fits or not
	// Return value is always true
	bool WriteToConsole(const HANDLE & consoleScreenBuffer,
		const char * string,
		const int stringSize);

	// Prints to a given location. Won't check if the string fits or not
	// Return value is always true
	bool WriteToConsole(const HANDLE & consoleScreenBuffer,
		const wchar_t * string,
		const int stringSize);

	// Prints to a given location. Won't check if the string fits or not
	// Return value is always true
	bool WriteToConsole(const HANDLE & consoleScreenBuffer, const std::string string);

	// Prints to a given location. Won't check if the string fits or not
	// Return value is always true
	bool WriteToConsole(const HANDLE & consoleScreenBuffer, const std::wstring string);

	// Prints to a given location. Won't check if the string fits or not
	// Returns if succesful
	bool WriteToConsole(const HANDLE & consoleScreenBuffer,
		const char * string,
		const int stringSize,
		const COORD position);

	// Prints to a given location. Won't check if the string fits or not
	// Returns if succesful
	bool WriteToConsole(const HANDLE & consoleScreenBuffer,
		const wchar_t * string,
		const int stringSize,
		const COORD position);

	// Prints to a given location. Won't check if the string fits or not
	// Returns if succesful
	bool WriteToConsole(const HANDLE & consoleScreenBuffer,
		const std::string string,
		const COORD position);

	// Prints to a given location. Won't check if the string fits or not
	// Returns if succesful
	bool WriteToConsole(const HANDLE & consoleScreenBuffer,
		const std::wstring string,
		const COORD position);
#pragma endregion
#pragma region User Input
	// Gets the specified number of events from the buffer
	// and stores them in the given INPUT_RECORD array
	// Interesting event types are KEY_EVENT, MOUSE_EVENT and WINDOW_BUFFER_SIZE_EVENT
	// Returns if it was succesful
	bool GetInput(const HANDLE & consoleInputBuffer,
		INPUT_RECORD * outputBuffer,
		int outputBufferSize,
		unsigned int & itemsRead);

	// Gets the first 128 events from the buffer (doesn't clear the output vectors!)
	// and stores them in the given vectors
	// Returns if it was succesful
	bool GetInput(const HANDLE & consoleInputBuffer,
		std::vector<KEY_EVENT_RECORD> & keyboardEvents,
		std::vector<MOUSE_EVENT_RECORD> & mouseEvents,
		std::vector<WINDOW_BUFFER_SIZE_RECORD> & windowResizeEvents);

	// Returns the first key down
	// Sleepduration is in ms
	// This method is blocking
	KEY_EVENT_RECORD GetKeyEvent(const HANDLE consoleInputBuffer, DWORD sleepduration = 5);

	// Returns the first key down
	// Sleepduration is in ms
	// This method is blocking
	KEY_EVENT_RECORD GetKeyEvent(const HANDLE consoleInputBuffer,
		INPUT_RECORD * inputBuffer,
		const int inputBufferLength,
		DWORD sleepduration = 5);

	// Returns the first key down, similar to GETCH, ascii encoding
	// Sleepduration is in ms
	// This method is blocking
	char GetChar(const HANDLE consoleInputBuffer, DWORD sleepduration = 5);

	// Returns the first key down, similar to GETCH, ascii encoding
	// Sleepduration is in ms
	// This method is blocking
	char GetChar(const HANDLE consoleInputBuffer,
		INPUT_RECORD * inputBuffer,
		const int inputBufferLength,
		DWORD sleepduration = 5);

	// Returns the first key down, similar to GETCH, unicode encoding
	// Sleepduration is in ms
	// This method is blocking
	wchar_t GetWChar(const HANDLE consoleInputBuffer, DWORD sleepduration = 5);

	// Returns the first key down, similar to GETCH, unicode encoding
	// Sleepduration is in ms
	// This method is blocking
	wchar_t GetWChar(const HANDLE consoleInputBuffer,
		INPUT_RECORD * inputBuffer,
		const int inputBufferLength,
		DWORD sleepduration = 5);

	// Returns an ascii encoded string
	// default value 13 is enter, sleepduration is in ms
	// This method is blocking
	// Can be empty if the first keypress is the delimiter
	// Does not add the first 31 ascii values to the string
	std::string GetLine(const HANDLE inputBuffer, char delimiter = 13, DWORD sleepduration = 5);

	// Returns an ascii encoded string
	// default value 13 is enter, sleepduration is in ms
	// This method is blocking
	// Can be empty if the first keypress is the delimiter
	// Does not add the first 31 ascii values to the string
	std::string GetLine(const HANDLE inputBuffer,
		INPUT_RECORD * inputs,
		const int inputsLength,
		char delimiter = 13,
		DWORD sleepduration = 5);

	// Returns an unicode encoded string
	// default value 13 is enter, sleepduration is in ms
	// This method is blocking
	// Can be empty if the first keypress is the delimiter
	// Does not add the first 31 unicode values to the string
	std::wstring GetWLine(const HANDLE inputBuffer, char delimiter = 13, DWORD sleepduration = 5);

	// Returns an unicode encoded string
	// default value 13 is enter, sleepduration is in ms
	// This method is blocking
	// Can be empty if the first keypress is the delimiter
	// Does not add the first 31 unicode values to the string
	std::wstring GetWLine(const HANDLE inputBuffer,
		INPUT_RECORD * inputs,
		const int inputsLength,
		char delimiter = 13,
		DWORD sleepduration = 5);


	// Returns an ascii encoded string, visualizes the typing process
	// default value 13 is enter, sleepduration is in ms
	// This method is blocking
	// Can be empty if the first keypress is the delimiter
	// Does NOT print the delimiter!
	std::string GetLineVisualized(const HANDLE consoleScreenBuffer,
		const HANDLE consoleInputBuffer,
		const CONSOLE_SCREEN_BUFFER_INFO consoleInfo,
		char delimiter = 13,
		DWORD sleepduration = 5);

	// Returns an ascii encoded string, visualizes the typing process
	// default value 13 is enter, sleepduration is in ms
	// This method is blocking
	// Can be empty if the first keypress is the delimiter
	// Does NOT print the delimiter!
	std::string GetLineVisualized(const HANDLE consoleScreenBuffer,
		const HANDLE consoleInputBuffer,
		const CONSOLE_SCREEN_BUFFER_INFO consoleInfo,
		INPUT_RECORD * inputBuffer,
		const int inputBufferLength,
		char delimiter = 13,
		DWORD sleepduration = 5);

	// Returns an unicode encoded string, visualizes the typing process
	// default value 13 is enter, sleepduration is in ms
	// This method is blocking
	// Can be empty if the first keypress is the delimiter
	// Does NOT print the delimiter!
	std::wstring GetWLineVisualized(const HANDLE consoleScreenBuffer,
		const HANDLE consoleInputBuffer,
		const CONSOLE_SCREEN_BUFFER_INFO consoleInfo,
		char delimiter = 13,
		DWORD sleepduration = 5);

	// Returns an unicode encoded string, visualizes the typing process
	// default value 13 is enter, sleepduration is in ms
	// This method is blocking
	// Can be empty if the first keypress is the delimiter
	// Does NOT print the delimiter!
	std::wstring GetWLineVisualized(const HANDLE consoleScreenBuffer,
		const HANDLE consoleInputBuffer,
		const CONSOLE_SCREEN_BUFFER_INFO consoleInfo,
		INPUT_RECORD * inputBuffer,
		const int inputBufferLength,
		char delimiter = 13,
		DWORD sleepduration = 5);

#pragma endregion

	// Closes the output handle and restores the input handle created by the console
	// Returns if succesfull
	bool DeleteInputHandle(HANDLE &inputBuffer, DWORD previousInputBufferConfig);

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

	unsigned int CreateColorAttributes(FONTCOLOR fontColor, BACKGROUNDCOLOR bgColor);

}
#endif // !CONSOLEFUNC_H
 