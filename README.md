# ConsoleUtilties
A collection of small header libraries to give your Win32 console applications some color and flexibility.

ExtendedIO.hpp contains tools to color your output, set the output cursor, catch window events (resize, focus, key presses, mouse, etc.).

WinUtils.hpp is a tiny header library that gives you copy and pasting to the clipboard for simple strings (std::string). Multibyte strings (std::wstring) are not supported at this time.

# What is this for?
Any platform running Windows (Win2K - Win10) should be fine. This is for people wanting that extra control over the console. Put text at specific coordinates. Use some fancy colors to highlight the important bits. Maybe even use the mouse, or handle window resizing events! All of this and more is made relatively easy. It at least spares you from doing a bunch of it yourself.

And even for the people with no CTRL+C / CTRL+V capabilities in their consoles: you can easily build that yourself now.

# Help?
There is absolutely no guarantee that this code is robust in any way. It was build for giving my console applications some more abilities, but these applications don't even cover half of the possible use cases.

Check the Example.cpp file in the src directory for some pointers as to how to use this.
