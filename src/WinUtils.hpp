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

namespace WinUtil
{
	bool GetWindowHandle(HWND & windowHandle)
	{
		windowHandle = GetActiveWindow();
		return windowHandle != nullptr;
	}

	bool SetClipboardText(const std::string & string)
	{
		HANDLE windowHandle;
		if (!GetWindowHandle(windowHandle))
			return false;

		if (!OpenClipboard(windowHandle))
			return false;

		if (EmptyClipboard())
		{
			HGLOBAL globalAlloc = GlobalAlloc(GMEM_MOVEABLE, string.size());
			if (!globalAlloc)
			{
				CloseClipboard();
				return false;
			}
			memcpy(GlobalLock(globalAlloc), &string[0], string.size());
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

	bool GetClipboardText(std::string & output)
	{
		HANDLE windowHandle;
		if (!GetWindowHandle(windowHandle))
			return false;

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
					output.clear();
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
}

#endif
