#pragma once
#include <string>
#include <Windows.h>

namespace Clipboard
{
	// Throws a runtime_error if it cannot fetch the handle
	void _GetWindowHandle(HWND& windowHandle);

	// Throws a runtime_error if it encounters any errors (see .what())
	void Set(const std::string& string);

	// Throws a runtime_error if it encounters any errors (see .what())
	// If no text is available the string is untouched
	void Get(std::string& output);
}
