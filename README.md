# ConsoleUtilties
Current version is 0.4 and it is in ALPHA stage.

ConsoleFunc (.h && .cpp) contain functions useful for console work. This ranges from input to ouput, clipboard pasting and copying and creating handles. Everything basic is in here. Start looking here if you want start building classes yourself.

ConsoleObj (.h && .cpp) contains helper objects that make use of the functions defined in ConsoleFunc.cpp. For example the class Cout, which mimics the std::cout behaviour, an easy solution if you just want to print pretty colors and not worry about where to get your HANDLE's. A std::cin equivalent is in the making, I have to sort out output streams before that though. Other objects are menu structures, to easily create a TUI.

For now I will refer to the header files for explanation what each function and object does. If there is no description there it should be self explanatory or probably means it shouldn't be used yet.

# What is this for?
Any platform running Windows (Win2K - Win10). This is for people wanting that extra control over the console. Put text at specific coordinates. Use some fancy colors to highlight the important bits. Maybe even use the mouse, or handle window resizing events! All of this and more is made relatively easy. It at least spares you from doing a bunch of it yourself.

And even for the people with no CTRL+C / CTRL+V capabilities with their console (I believe that is Windows 10 only?): you can easily build that yourself.

# Work in progress
For now it remains in ALPHA state. What does this mean?
  - It still requires some thorough testing
  - It still lacks some essential features that are planned
  - Names (namespaces, classes, variables, functions, etc.) can change!
  - Don't use this blindly in your projects whilst in ALPHA / BETA stage, there are zero guarrantees

In the current release there are a few asserts in place to provide some sanity checks and ease of debugging, but for release builds there is no such thing and all functions and methods, unless otherwise specified, require valid parameters. To debug functions and methods returning false or displaying other unexpected behaviour see the MSDN page on GetLastError to check things are all right on your end: https://msdn.microsoft.com/nl-nl/library/windows/desktop/ms679360(v=vs.85).aspx .

# Why ?
I am doing this as practice, writing code that others can understand and read. Apart from that it often bothers me that I cannot simply use some colors in my console applications. Plus, before Windows 10, there was no copy and paste, which was annoying!
