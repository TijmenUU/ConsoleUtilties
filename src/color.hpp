#pragma once
#include <cstdint>
#include <iostream>
#include <Windows.h>

namespace Color
{
	enum class Foreground : std::uint8_t
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
		
  struct Color
	{
		std::uint8_t value;

		void Set(const Foreground fgColor)
		{
			value &= 0xF0;
			value |= static_cast<std::uint8_t>(fgColor);
		}

		void Set(const Background bgColor)
		{
			value &= 0xF;
			value |= static_cast<std::uint8_t>(bgColor);
		}

		void Set(const std::uint8_t value)
		{
			this->value = value;
		}

		Color(const std::uint8_t colorValue)
			: value(colorValue)
		{}

		Color(const Foreground foregroundColor = Foreground::WHITE, const Background backgroundColor = Background::BLACK)
			: value(static_cast<std::uint8_t>(foregroundColor) | static_cast<std::uint8_t>(backgroundColor))
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
}