# Purpose

A commandline application that encodes a set of WAV files to MP3 on both Unix/Windows based platform.
 
# Requirements
 
1. application is called with pathname as argument, e.g.

   <applicationname> ~\MyWavCollection all WAV-files contained directly in that folder are to be encoded to MP3

2. use all available CPU cores for the encoding process in an efficient way by utilizing multi-threading
 
3. statically link to lame encoder library
 
4. application should be compilable and runnable on Windows and Linux

5. the resulting MP3 files are to be placed within the same directory as the source WAV files, the filename extension should be changed appropriately to .MP3
 
6. non-WAV files in the given folder shall be ignored
 
7. multithreading shall be implemented by means of using Posix Threads (there exist implementations for Windows)
 
8. the Boost library shall not be used
 
9. the LAME encoder should be used with reasonable standard settings (e.g. quality based encoding with quality level "good")

# Implementation notes

* supported 16-bit pcm wave files only

* required a compiler with c++14 support. however, i didn't use `<thread>` and `<atomic>` due to the abovementioned restrictions

* tested with Google Test, Visual Studio 2015 with WinSDK8 on Windows, GNU C 6.0 on Linux and clang-703.0.31 on Mac

* added a CMakeLists.txt for all platform configurations
