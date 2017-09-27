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

// Compiles on Windows 10 v1703 with g++ (x86_64-posix-seh-rev2, Built by MinGW-W64 project) 7.1.0
// g++ -std=c++11 -Wall Example.cpp -o example.exe

#include "ExtendedIO.hpp"
#include "WinUtils.hpp"
#include <chrono>
#include <iomanip>
#include <thread> // sleep

const std::string cTestString = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras sollicitudin mi arcu, in semper nisl suscipit id. Praesent laoreet ultrices leo, quis tincidunt purus porttitor at. Maecenas molestie a orci a pharetra. In vitae lacus eleifend, viverra ante ut, ornare sapien. Vivamus ullamcorper iaculis dolor et venenatis. Etiam viverra scelerisque egestas. In ac risus eu purus ultrices egestas. Duis tristique facilisis eros eu gravida. Curabitur eget lacinia magna. Maecenas tristique dui neque, at elementum augue ultrices sed. Morbi dignissim dolor sed erat molestie, a fringilla tellus dignissim. Mauris eget metus et dui vulputate interdum a ac neque. In nisl arcu, suscipit et fringilla vitae, tincidunt sit amet lectus. Nam vitae aliquam felis. Praesent volutpat pharetra lacus.\r\n\r\nFusce consequat leo porta, cursus mi in, fringilla lorem. Ut in ligula varius, elementum tellus sit amet, pretium lorem. Cras nec vulputate ante. Quisque fringilla scelerisque quam, quis pulvinar nibh facilisis nec. Donec nec cursus elit. Aenean egestas viverra turpis sit amet tincidunt. Morbi volutpat est sed eros malesuada, in pretium metus imperdiet. Proin ac orci at ipsum rhoncus tristique. Sed eget volutpat nisi, sit amet congue risus.";

int main(int argc, char ** argv)
{
	ExtendedIO::Color originalColor;
	std::cout >> originalColor;
	ExtendedIO::Coordinate originalCoord;
	std::cout >> originalCoord;

	/* Color demonstration */
	ExtendedIO::Color color(ExtendedIO::Color::Background::BLACK);
	for(std::uint16_t i = 0x00; i <= 0x0F; ++i)
	{
		std::cout << std::setw(4) << color + i << i << ' ';
	}

	std::cout << std::endl;
	color = ExtendedIO::Color::Font::WHITE;
	for(std::uint16_t i = 0x00; i <= 0xF0; i += 0x10)
	{
		std::cout << std::setw(4) << color + i << i << ' ';
	}

	std::cout << originalColor << std::endl;

	/* Clipboard demonstration */
	std::cout << "Please give me something to copy to your clipboard: ";
	std::string content;
	std::getline(std::cin, content);

	try
	{
		WinUtil::SetClipboardText(content);
		std::cout << "Pasted succesfully to the clipboard!" << std::endl;
	}
	catch(const std::runtime_error & e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << std::endl;

	/* Utility demonstration */
	ExtendedIO::Window::Rectangle rect;
	std::cout >> rect;
	ExtendedIO::Coordinate size = rect.bottomRight - rect.topLeft;
	std::cout << "Current window is " << size.X << " characters wide and ";
	std::cout << size.Y << " characters tall." << std::endl;

	/* Cursor demonstration */
	std::cout << std::setprecision(3) << std::fixed <<  "Pasting Lorum ipsum in ";
	ExtendedIO::Coordinate timeCoord;
	std::cout >> timeCoord;

	const double secondsToWait = 3.0;
	const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::duration<double> waitTime;
	do
	{
		waitTime = std::chrono::system_clock::now() - start;
		std::cout << timeCoord << std::setw(3) << secondsToWait - waitTime.count() << " seconds.";
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	} while(waitTime.count() < secondsToWait);

	std::cout << originalCoord;
	std::cout << ExtendedIO::Color(ExtendedIO::Color::Font::GREEN);
	std::cout << cTestString << std::endl;

	/* Restore color */
	std::cout << originalColor;
	return 0;
}
