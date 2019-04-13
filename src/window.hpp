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

		Rectangle();

		Rectangle(const std::int16_t x0,
			const std::int16_t y0,
			const std::int16_t x1,
			const std::int16_t y1);

		Rectangle(const COORD _topLeft,
			const COORD _bottomRight);
	};

	// Returns the maximum coordinate allowed for the cursor
	// Return is zero'd if it fails to fetch it
	COORD GetBufferSize();

	// Returns the current window viewport (part of the buffer on display)
	// Return is zero'd if it fails to fetch it
	Rectangle GetActiveViewportSize();
}

std::ostream& operator >>(std::ostream& outputStream, Window::Rectangle& rect);