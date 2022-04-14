# Template for SDL2 for C

For the purpose of programming with the Simple DirectMedia Layer library, this repository provides a standard SDL template to use on Windows.

This template SDL project uses Make to build.
Within the makefile
- GCC is used for compilation
- The -Wall compiler flag is used to enable all warnings during build
- The -g compiler flag is used to produce debugging information during build
- When adding more *.c files to the /src folder, don't forget to add them in the makefile in the IN param
- The output is located in the /bin folder as output.exe
- The -l compiler flag is used to link libraries. A set of standard system directories is searched for the given library plus additional directories that are specified with the -L compiler flag.
- The -I compiler flag is used to add the directory that contains header files for preprocessing.

# References
- https://man7.org/linux/man-pages/man1/gcc.1.html