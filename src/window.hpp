#pragma once
#include <cstdint>
#include <iostream>
#include <Windows.h>

namespace Window
{
	struct Rectangle
	{
		COORD topLeft;
		COORD bottomRight;

		Rectangle()
			: topLeft(),
			bottomRight()
		{
		}

		Rectangle(const std::int16_t x0,
			const std::int16_t y0,
			const std::int16_t x1,
			const std::int16_t y1)
			: topLeft{ x0, y0 },
			bottomRight{ x1, y1 }
		{
		}

		Rectangle(const COORD _topLeft,
			const COORD _bottomRight)
			: topLeft(_topLeft),
			bottomRight(_bottomRight)
		{
		}
	};

	// Returns the maximum coordinate allowed for the cursor
	// Return is zero'd if it fails to fetch it
	COORD BufferSize()
	{
		COORD result;

		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo))
		{
			result.X = conInfo.dwSize.X;
			result.Y = conInfo.dwSize.Y;
		}

		return result;
	}

	// Returns the current window viewport (part of the buffer on display)
	// Return is zero'd if it fails to fetch it
	Rectangle ActiveViewport()
	{
		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo))
		{
			return Rectangle(
				conInfo.srWindow.Left,
				conInfo.srWindow.Top,
				conInfo.srWindow.Right,
				conInfo.srWindow.Bottom);
		}

		return Rectangle();
	}
}

std::ostream& operator >>(std::ostream& outputStream,
	Window::Rectangle& rect)
{
	rect = Window::ActiveViewport();
	return outputStream;
}