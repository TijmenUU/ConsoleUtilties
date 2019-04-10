// Tested with Visual Studio 2019
#include <chrono>
#include "clipboard.hpp"
#include "color.hpp"
#include "coordops.hpp"
#include "cursor.hpp"
#include <iomanip>
#include <thread> // sleep
#include "window.hpp"

const std::string cTestString = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras sollicitudin mi arcu, in semper nisl suscipit id. Praesent laoreet ultrices leo, quis tincidunt purus porttitor at. Maecenas molestie a orci a pharetra. In vitae lacus eleifend, viverra ante ut, ornare sapien. Vivamus ullamcorper iaculis dolor et venenatis. Etiam viverra scelerisque egestas. In ac risus eu purus ultrices egestas. Duis tristique facilisis eros eu gravida. Curabitur eget lacinia magna. Maecenas tristique dui neque, at elementum augue ultrices sed. Morbi dignissim dolor sed erat molestie, a fringilla tellus dignissim. Mauris eget metus et dui vulputate interdum a ac neque. In nisl arcu, suscipit et fringilla vitae, tincidunt sit amet lectus. Nam vitae aliquam felis. Praesent volutpat pharetra lacus.\r\n\r\nFusce consequat leo porta, cursus mi in, fringilla lorem. Ut in ligula varius, elementum tellus sit amet, pretium lorem. Cras nec vulputate ante. Quisque fringilla scelerisque quam, quis pulvinar nibh facilisis nec. Donec nec cursus elit. Aenean egestas viverra turpis sit amet tincidunt. Morbi volutpat est sed eros malesuada, in pretium metus imperdiet. Proin ac orci at ipsum rhoncus tristique. Sed eget volutpat nisi, sit amet congue risus.";

void DemonstrateColors()
{
	Color::Color originalColor(0);
	std::cout >> originalColor;

	for (std::uint8_t i = 0x00; i <= 0x0F; ++i)
	{
		std::uint8_t bgColor = static_cast<std::uint8_t>(Color::Background::BLACK);
		std::cout << std::setw(4) << Color::Color(bgColor | i) << static_cast<int>(i);
	}

	std::cout << '\n';

	for (std::uint16_t i = 0x00; i <= 0xF0; i += 0x10)
	{
		std::uint8_t fontColor = static_cast<std::uint8_t>(Color::Foreground::WHITE);
		std::cout << std::setw(4) << Color::Color(fontColor | i) << ' ';
	}

	std::cout << originalColor << '\n';
}

void DemonstrateClipboard()
{
	std::cout << "Please give me something to copy to your clipboard: ";
	std::string content;
	std::getline(std::cin, content);

	try
	{
		Clipboard::Set(content);
		std::cout << "Pasted succesfully to the clipboard!\n";
	}
	catch (const std::runtime_error & e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << '\n';
}

void DemonstrateWindowUtils()
{
	Window::Rectangle rect;
	std::cout >> rect;
	COORD size = rect.bottomRight - rect.topLeft;
	std::cout << "Current window is " << size.X << " characters wide and ";
	std::cout << size.Y << " characters tall.\n";
}

void DemonstrateCursor()
{
	COORD originalCoord;
	std::cout >> originalCoord;
	Color::Color originalColor(0);
	std::cout >> originalColor;

	std::cout << std::setprecision(3) << std::fixed << "Pasting Lorum ipsum in ";
	COORD timeCoord;
	std::cout >> timeCoord;

	const int millisecondsToWait = 3000U;
	const auto start = std::chrono::system_clock::now();
	int waitTime = 0U;
	while (waitTime < millisecondsToWait)
	{
		const auto elapsedTime = std::chrono::system_clock::now() - start;		
		waitTime = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
		
		double remainingTime = static_cast<double>(millisecondsToWait - waitTime) / 1000.0;
		if (remainingTime < 0.0)
			remainingTime = 0.0;

		std::cout << timeCoord << std::setw(3) << remainingTime << " seconds.";

		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}

	std::cout << originalCoord;
	std::cout << Color::Color(Color::Foreground::GREEN);
	std::cout << cTestString << '\n';

	// Restore the color
	std::cout << originalColor;
}

int main(int argc, char** argv)
{
	DemonstrateColors();

	DemonstrateClipboard();

	DemonstrateWindowUtils();

	DemonstrateCursor();

	return 0;
}
