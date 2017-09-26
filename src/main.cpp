/*
ALPHA V0.4

Copyright (c) 2016  Tijmen van Nesselrooij

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

#include "ExtendedIO.hpp"
#include <iomanip>

int main(int argc, char ** argv)
{
	ExtendedIO::Color originalColor;
	std::cout >> originalColor;

	ExtendedIO::Color color(ExtendedIO::Color::BACKGROUND::DARKGREEN);
	for(std::uint16_t i = 0x00; i <= 0x0F; ++i)
	{
		std::cout << std::setw(4) << color + i << i << ' ';
	}

	std::cout << std::endl;
	color = ExtendedIO::Color::FONT::WHITE;
	for(std::uint16_t i = 0x00; i <= 0xF0; i += 0x10)
	{
		std::cout << std::setw(4) << color + i << i << ' ';
	}

	std::cout << originalColor;
	return 0;
}
