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

#ifndef EXTENDEDIO_HPP
#define EXTENDEDIO_HPP
#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>

namespace ExtendedIO
{
	/*
		Important notice: these functions will fail silently. No exceptions should
		be thrown in case of failures.
	*/

	struct Color
	{
		enum class Font : std::uint16_t
		{
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

		enum class Background : std::uint16_t
		{
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

		std::uint16_t value; // only the first 2 bytes are used for color (& 0xFF)

		Color & operator =(const std::uint16_t _value)
		{
			value = _value;
			return *this;
		}

		Color & operator +=(const Color & other)
		{
			value |= other.value;
			return *this;
		}
		Color & operator +=(const std::uint16_t other)
		{
			value |= other;
			return *this;
		}

		Color & operator -=(const Color & other)
		{
			value ^= other.value;
			return *this;
		}
		Color & operator -=(const std::uint16_t other)
		{
			value ^= other;
			return *this;
		}

		Color operator +(const Color & right) const
		{
			return Color(value | right.value);
		}
		Color operator +(const std::uint16_t right) const
		{
			return Color(value | right);
		}

		Color operator -(const Color & right) const
		{
			return Color(value ^ right.value);
		}
		Color operator -(const std::uint16_t right) const
		{
			return Color(value ^ right);
		}

		// Sets the STD_OUTPUT_HANDLE text color attribute for future output
		friend std::ostream & operator <<(std::ostream & outputStream,
			const Color & color);
		// Retrieves the STD_OUTPUT_HANDLE text color attribute at the current
		// cursor position
		friend std::ostream & operator >>(std::ostream & outputStream,
			Color & color);

		// Swaps the font color with the background color
		void Swap()
		{
			std::uint16_t newBackground = (value & 0x0F) << 1;
			value = (value >> 1) | newBackground;
		}

		Color()
		: value(0U)
		{
		}

		Color(const std::uint16_t _value)
		: value(_value)
		{
		}

		Color(const Font fontColor)
		: value(static_cast<std::uint16_t>(fontColor))
		{
		}

		Color(const Background backgroundColor)
		: value(static_cast<std::uint16_t>(backgroundColor))
		{
		}
	};

	std::uint16_t operator |(const Color::Font fontColor,
		const Color::Background backgroundColor)
	{
		return static_cast<std::uint16_t>(fontColor) |
			static_cast<std::uint16_t>(backgroundColor);
	}
	std::uint16_t operator |(const Color::Background backgroundColor,
		const Color::Font fontColor)
	{
		return static_cast<std::uint16_t>(fontColor) |
			static_cast<std::uint16_t>(backgroundColor);
	}

	std::ostream & operator <<(std::ostream & outputStream, const Color & color)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color.value);

		return outputStream;
	}

	std::ostream & operator >>(std::ostream & outputStream, Color & color)
	{
		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo))
		{
			color.value = conInfo.wAttributes & 0xFF;
		}

		return outputStream;
	}

	struct Coordinate : public COORD
	{
		Coordinate operator +(const COORD & other)
		{
			return Coordinate(X + other.X,
									Y + other.Y);
		}

		Coordinate operator -(const COORD & other)
		{
			return Coordinate(X - other.X,
									Y - other.Y);
		}

		Coordinate & operator +=(const COORD & other)
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		Coordinate & operator -=(const COORD & other)
		{
			X -= other.X;
			Y -= other.Y;
			return *this;
		}

		// Sets the STD_OUTPUT_HANDLE cursor position
		friend std::ostream & operator <<(std::ostream & outputStream,
			const Coordinate & cursor);
		// Retrieves the STD_OUTPUT_HANDLE cursor position
		friend std::ostream & operator >>(std::ostream & outputStream,
			Coordinate & cursor);

		// Creates a cursor at position 0, 0
		Coordinate()
		: COORD{0, 0}
		{
		}

		Coordinate(const std::int16_t x, const std::int16_t y)
		: COORD{x, y}
		{
		}
	};

	std::ostream & operator <<(std::ostream & outputStream,
		const Coordinate & cursor)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);

		return outputStream;
	}

	std::ostream & operator >>(std::ostream & outputStream,
		Coordinate & cursor)
	{
		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo))
		{
			cursor.X = conInfo.dwCursorPosition.X;
			cursor.Y = conInfo.dwCursorPosition.Y;
		}

		return outputStream;
	}

	struct InputEvent : public INPUT_RECORD
	{
		// Retrieves the STD_OUTPUT_HANDLE cursor position
		friend std::istream & operator >>(std::istream & inputStream,
			InputEvent & inputEvent);

		enum class Type : std::uint16_t
		{
			FOCUS  = 0x0010,
			KEY 	 = 0x0001,
			MENU 	 = 0x0008,
			MOUSE  = 0x0002,
			RESIZE = 0x0004
		};

		Type GetEventType() const
		{
			return static_cast<Type>(EventType);
		}
	};

	std::istream & operator >>(std::istream & inputStream,
		InputEvent & inputEvent)
	{
		DWORD itemsRead = 0;
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),
			&inputEvent,
			1,
			&itemsRead);

		return inputStream;
	}

	std::istream & operator >>(std::istream & inputStream,
		std::vector<InputEvent> & inputEvents)
	{
		DWORD itemsRead = 0;
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),
			&inputEvents[0],
			inputEvents.size(),
			&itemsRead);

		inputEvents.resize(itemsRead);

		return inputStream;
	}

	namespace Window
	{
		struct Rectangle
		{
			Coordinate topLeft;
			Coordinate bottomRight;

			// Get the current output viewport rectangle
			friend std::ostream & operator >>(std::ostream & outputStream,
				Rectangle & rect);

			Rectangle()
			: topLeft(),
			bottomRight()
			{
			}

			Rectangle(const std::uint16_t x0,
				const std::uint16_t y0,
				const std::uint16_t x1,
				const std::uint16_t y1)
			: topLeft(x0, y0),
			bottomRight(x1, y1)
			{
			}

			Rectangle(const Coordinate _topLeft,
				const Coordinate _bottomRight)
			: topLeft(_topLeft),
			bottomRight(_bottomRight)
			{
			}
		};

		// Returns the maximum coordinate allowed for the cursor
		// Return is zero'd if it fails to fetch it
		static Coordinate BufferSize()
		{
			Coordinate result;

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
		static Rectangle ActiveViewport()
		{
			CONSOLE_SCREEN_BUFFER_INFO conInfo;
			if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo))
			{
				return Rectangle(	conInfo.srWindow.Left,
										conInfo.srWindow.Top,
										conInfo.srWindow.Right,
										conInfo.srWindow.Bottom);
			}

			return Rectangle();
		}

		std::ostream & operator >>(std::ostream & outputStream,
			Rectangle & rect)
		{
			rect = ActiveViewport();
			return outputStream;
		}
	}
}

#endif
