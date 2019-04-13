# ConsoleUtilties
A collection of small header libraries to give your Win32 console applications some color and flexibility.
- `src/clipboard.hpp` gives you copy and pasting to the clipboard for simple strings (std::string)
- `src/color.hpp` contains tools to color your console output
- `src/coordops.hpp` contains some basic operator implementations for basic arithmetic with the `COORD` structure
- `src/cursor.hpp` contains iostream overloads for the `COORD` structure to manipulate the cursor (where things will be written)
- `src/event.hpp` contains tools to listen for events: resizing the window, using the mouse, pressing a key, etcetera.
- `src/example.cpp` contains example usages of all the included header files
- `src/window.hpp` contains tools to get information about the active window: how many characters can fit on the screen

All header files are independent, but are more interesting when combined. Unfortunately they all need to include `Windows.h` as well, exposing a lot of Windows API cruft to the global namespace.

Do checkout the included Visual Studio 2019 project to build the example showcase.

## What is this for?
Any platform running Windows (Win2K - Win10) should be fine. This is for people wanting that extra control over the console. Put text at specific coordinates. Use some fancy colors to highlight the important bits. Maybe even use the mouse, or handle window resizing events! All of this and more is made relatively easy. It at least spares you from doing a bunch of it yourself.

And even for the people with no CTRL+C / CTRL+V capabilities in their consoles: you can easily build that yourself now.

## Help?
There is absolutely no guarantee that this code is robust in any way. It was build for giving my console applications some more abilities, but these applications don't even cover half of the possible use cases.

Check the `example.cpp` file in the src directory for some pointers as to how to use this.
