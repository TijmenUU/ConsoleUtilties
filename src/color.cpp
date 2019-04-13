#include "color.hpp"
#include <Windows.h>

namespace Color
{
	void Color::Set(const Foreground fgColor)
	{
		value &= 0xF0;
		value |= static_cast<std::uint8_t>(fgColor);
	}

	void Color::Set(const Background bgColor)
	{
		value &= 0xF;
		value |= static_cast<std::uint8_t>(bgColor);
	}

	void Color::Set(const std::uint8_t value)
	{
		this->value = value;
	}

	Color::Color(const std::uint8_t colorValue)
		: value(colorValue)
	{}

	Color::Color(const Foreground foregroundColor, const Background backgroundColor)
		: value(static_cast<std::uint8_t>(foregroundColor) | static_cast<std::uint8_t>(backgroundColor))
	{}

	// At the current cursor position plus any consecutive writes afterwards
	void Set(const Color& color)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color.value);
	}

	// From the current cursor position
	Color Get()
	{
		Color color;

		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo))
		{
			color.value = conInfo.wAttributes & 0xFF;
		}

		return color;
	}
}

std::ostream& operator <<(std::ostream& outputStream, const Color::Color& color)
{
	Color::Set(color);

	return outputStream;
}

std::ostream& operator >>(std::ostream& outputStream, Color::Color& color)
{
	color = Color::Get();

	return outputStream;
}