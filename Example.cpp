#pragma once
#include <conio.h>
#include "ConsoleUtilities.h"
#include <algorithm>

int main(int argc, char** argv)
{
	ConsoleHelperObjects::Console *console = new ConsoleHelperObjects::Console(); // On the heap to illustrate destruction later on

// Printing methods
	*console << "Hello World" << "\n";
	console->Print("\nThis is a newline");
	console->Print("\nI am unreadable!", ConsoleBasics::FONTCOLOR::WHITE, ConsoleBasics::BACKGROUNDCOLOR::WHITE);

// Setting the default colors (this is what the Print methods without color parameters use)
	console->SetBackgroundColor(ConsoleBasics::BACKGROUNDCOLOR::DARKBLUE);
	console->SetFontColor(ConsoleBasics::FONTCOLOR::GREEN);

// Copy to clipboard
	*console << "\nNow going to paste \"Hello World\\n\" into the clipboard and retrieve it\n";
	std::string string_buffer = "Hello World!\n";
	ConsoleBasics::CopyToClipboardContents(string_buffer);
// Paste from clipboard
	string_buffer = "";
	ConsoleBasics::GetClipboardContents(string_buffer);
	*console << (string_buffer.size() == 0 ? "Nothing to paste :-(" : string_buffer) << "\n";

	console->SetBackgroundColor(ConsoleBasics::BACKGROUNDCOLOR::DARKGREEN);
	console->SetFontColor(ConsoleBasics::FONTCOLOR::RED);

// Getting user input
	
	*console << "\nTime to get some input going, type your stuff and press enter when done: ";
	string_buffer = console->GetLineVisualized();
	*console << "\nLet me sort that for you:";
	std::sort(string_buffer.begin(), string_buffer.end());
	*console << string_buffer;
	
	/*
	*console << "\nCharacter code test\n";
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		char c = console->GetChar();
		*console << c << '\n';
	}
	*/
	*console << "\nPress any key to exit the console...";
	console->GetChar();
	delete console;

// Clean destruction example, std::cout and std::cin work normally again!
	std::cout << "\nGive me something to output: ";
	char c;
	std::cin >> c;
	std::cout << c;
	std::cout << "\nPress any key to exit...";
	_getch();

	return 0;
}
