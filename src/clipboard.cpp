#include "clipboard.hpp"
#include <cstring> // memcpy
#include <stdexcept>

namespace Clipboard
{
	void _GetWindowHandle(HWND& windowHandle)
	{
		windowHandle = GetForegroundWindow();
		if (windowHandle == nullptr)
		{
			throw std::runtime_error("Failed to retrieve handle for active window.");
		}
	}

	void Set(const std::string& string)
	{
		HWND windowHandle;
		_GetWindowHandle(windowHandle);

		if (!OpenClipboard(windowHandle))
			throw std::runtime_error("Failed to open clipboard.");

		if (EmptyClipboard())
		{
			// The Windows clipboard seems to want to store a terminating char
			// in the last position. So we reserve an additional byte for that.
			std::size_t allocSize = string.size() + 1U;
			HGLOBAL globalAlloc = GlobalAlloc(GMEM_MOVEABLE, allocSize);
			if (!globalAlloc)
			{
				CloseClipboard();
				throw std::runtime_error("Failed to allocate required space.");
			}
			std::memcpy(GlobalLock(globalAlloc), string.c_str(), allocSize);
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

	void Get(std::string& output)
	{
		HWND windowHandle;
		_GetWindowHandle(windowHandle);

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
