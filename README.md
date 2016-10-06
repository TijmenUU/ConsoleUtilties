# ConsoleUtilties
A header-only library to get an STD experience with the Windows console in C++. Written for personal projects, but made public as practice to write readable code for others too. 

For now it remains in ALPHA state. What does this mean?
  - It still requires some thorough testing
  - It still lacks some essential features that are planned
  - It has not been used much yet, so the way of doing things might seem clunky (I am looking into making it more like the STL)
  - The stability, albeit not having any issues thus far, is not guarranteed and it is not recommended to use this library yet in
    an environment that needs stability
  - Performance and optimization come in second for now (This doesn't mean the code is written without that in mind)
    
In the current release there are a few asserts in place to provide some sanity checks and ease of debugging, but for release builds there is no such thing and all functions and methods, unless otherwise specified, require valid parameters. To debug functions and methods returning false or displaying other unexpected behavious see the MSDN page on GetLastError to check things are all right on your end: https://msdn.microsoft.com/nl-nl/library/windows/desktop/ms679360(v=vs.85).aspx .
