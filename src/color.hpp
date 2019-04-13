#pragma once
#include <cstdint>
#include <iostream>

namespace Color
{
	enum class Foreground : std::uint8_t
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

	// Upper nibble
	enum class Background : std::uint8_t
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

	struct Color
	{
		std::uint8_t value;

		void Set(const Foreground fgColor);

		void Set(const Background bgColor);

		void Set(const std::uint8_t value);

		Color(const std::uint8_t colorValue);

		Color(const Foreground foregroundColor = Foreground::WHITE, const Background backgroundColor = Background::BLACK);
	};

	// At the current cursor position plus any consecutive writes afterwards
	void Set(const Color& color);

	// From the current cursor position
	Color Get();
}

std::ostream& operator <<(std::ostream& outputStream, const Color::Color& color);

std::ostream& operator >>(std::ostream& outputStream, Color::Color& color);