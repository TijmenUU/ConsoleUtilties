# ConsoleUtilties
A header-only library to get an STD experience with the Windows console in C++. Written for personal projects, but made public as practice to write readable code for others too.

# What is this for?
Any platform running Windows (Win2K - Win10). This is for people wanting that extra control over the console. Put text at specific coordinates. Use some fancy colors to highlight the important bits. Maybe even use the mouse, or handle window resizing events! All of this and more is made relatively easy with this header library. It at least spares you from doing a bunch of it yourself. For some implementation pointers see the Console class in the namespace ConsoleHelperObjects, or use the console class yourself as it pretty much wraps it all in a nice package for you. 

And even for the people with no CTRL+C / CTRL+V capabilities with their console (I believe that is Windows 10 only?): you can easily build that yourself. The pasting is even already there, just listen for the CTRL + V event. 

# Work in progress
For now it remains in ALPHA state. What does this mean?
  - It still requires some thorough testing
  - It still lacks some essential features that are planned
  - It has not been used much yet, so the way of doing things might seem clunky (I am looking into making it more like the STL)
  - The stability, albeit not having any issues thus far, is not guarranteed and it is not recommended to use this library yet in
    an environment that needs stability
  - Performance and optimization come in second for now (This doesn't mean the code is written without that in mind)
    
In the current release there are a few asserts in place to provide some sanity checks and ease of debugging, but for release builds there is no such thing and all functions and methods, unless otherwise specified, require valid parameters. To debug functions and methods returning false or displaying other unexpected behavious see the MSDN page on GetLastError to check things are all right on your end: https://msdn.microsoft.com/nl-nl/library/windows/desktop/ms679360(v=vs.85).aspx .
