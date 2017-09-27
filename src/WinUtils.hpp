/*
Alpha v1.0

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

#ifndef WINUTILS_H
#define WINUTILS_H
#pragma once

#include <cstring> // memcpy
#include <exception>
#include <string>
#include <Windows.h>

namespace WinUtil
{
	// Throws a runtime_error if it cannot fetch the handle
	void GetWindowHandle(HWND & windowHandle)
	{
		windowHandle = GetForegroundWindow();
		if(windowHandle == nullptr)
		{
			throw std::runtime_error("Failed to retrieve handle for active window.");
		}
	}

	// Throws a runtime_error if it encounters any errors (see .what())
	void SetClipboardText(const std::string & string)
	{
		HWND windowHandle;
		GetWindowHandle(windowHandle);

		if (!OpenClipboard(windowHandle))
			throw std::runtime_error("Failed to open clipboard.");

		if (EmptyClipboard())
		{
			HGLOBAL globalAlloc = GlobalAlloc(GMEM_MOVEABLE, string.size());
			if (!globalAlloc)
			{
				CloseClipboard();
				throw std::runtime_error("Failed to allocate required space.");
			}
			std::memcpy(GlobalLock(globalAlloc), &string[0], string.size());
			GlobalUnlock(globalAlloc);
			if (SetClipboardData(CF_TEXT, globalAlloc) == nullptr)
			{
				CloseClipboard();
				GlobalFree(globalAlloc);
				throw std::runtime_error("Failed to set clipboard data.");
			}
			CloseClipboard(); // Not much we can do if this fails...
			GlobalFree(globalAlloc);
		}
		else
		{
			CloseClipboard();
			throw std::runtime_error("Failed to empty the clipboard.");
		}
	}

	// Throws a runtime_error if it encounters any errors (see .what())
	// If no text is available the string is untouched
	void GetClipboardText(std::string & output)
	{
		HWND windowHandle;
		GetWindowHandle(windowHandle);

		if (!OpenClipboard(windowHandle))
			throw std::runtime_error("Failed to open clipboard.");

		if (IsClipboardFormatAvailable(CF_TEXT))
		{
			HGLOBAL clipboardData = GetClipboardData(CF_TEXT);
			if (clipboardData != nullptr)
			{
				LPVOID clipboardDataPointer = GlobalLock(clipboardData);
				if (clipboardDataPointer != nullptr)
				{
					output.clear();
					output.assign(static_cast<char*>(clipboardDataPointer));
				}
				GlobalUnlock(clipboardData);
				CloseClipboard();
			}
		}
		CloseClipboard();
	}
}

#endif
