#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>

namespace ExtIO
{
	/*
		Important notice: these functions will fail silently.
	*/

	struct Color
	{
		// Lower nibble
		enum class Font : std::uint8_t
		{
			BLACK		= 0x00,
			DARKBLUE	= 0x01,
			DARKGREEN	= 0x02,
			DARKCYAN	= 0x03,
			DARKRED		= 0x04,
			DARKPURPLE	= 0x05,
			DARKYELLOW	= 0x06,
			GRAY		= 0x08,
			LIGHTGRAY	= 0x07,
			BLUE		= 0x09,
			GREEN		= 0x0A,
			CYAN		= 0x0B,
			RED			= 0x0C,
			PURPLE		= 0x0D,
			YELLOW		= 0x0E,
			WHITE		= 0x0F
		};

		// Upper nibble
		enum class Background : std::uint8_t
		{
			BLACK		= 0x00,
			DARKBLUE	= 0x10,
			DARKGREEN	= 0x20,
			DARKCYAN	= 0x30,
			DARKRED		= 0x40,
			DARKPURPLE	= 0x50,
			DARKYELLOW	= 0x60,
			GRAY		= 0x80,
			LIGHTGRAY	= 0x70,
			BLUE		= 0x90,
			GREEN		= 0xA0,
			CYAN		= 0xB0,
			RED			= 0xC0,
			PURPLE		= 0xD0,
			YELLOW		= 0xE0,
			WHITE		= 0xF0
		};

		std::uint8_t value;

		Color(const std::uint8_t _value = 0U)
		: value(_value)
		{}

		Color(const Font fontColor = Font::WHITE, const Background backgroundColor = Background::BLACK)
		: value(static_cast<std::uint16_t>(fontColor) | static_cast<std::uint16_t>(backgroundColor))
		{}
	};

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
			return Coordinate(X + other.X, Y + other.Y);
		}

		Coordinate operator -(const COORD & other)
		{
			return Coordinate(X - other.X, Y - other.Y);
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

		Coordinate(const std::int16_t x = 0, const std::int16_t y = 0)
		: COORD{x, y}
		{
		}
	};

	std::ostream & operator <<(std::ostream & outputStream, const Coordinate & cursor)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);

		return outputStream;
	}

	std::ostream & operator >>(std::ostream & outputStream, Coordinate & cursor)
	{
		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo))
		{
			cursor.X = conInfo.dwCursorPosition.X;
			cursor.Y = conInfo.dwCursorPosition.Y;
		}

		return outputStream;
	}

	namespace Window
	{
		struct Rectangle
		{
			Coordinate topLeft;
			Coordinate bottomRight;

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
		Coordinate BufferSize()
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
		Rectangle ActiveViewport()
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
