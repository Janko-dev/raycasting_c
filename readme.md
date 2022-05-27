# Template for SDL2 for C

For the purpose of programming with the Simple DirectMedia Layer library, this repository provides a template for the SDL2 library to use on Windows.
It's setup to build for 32bit binary executables.

This template SDL project uses Make as build-tool, within which:
- GCC is used for compilation. I use the native windows port of the GNU compiler collection through MinGW.
- The -Wall -Wextra compiler flag is used to enable and show all warnings during build
- The -g compiler flag is used to produce debugging information during build
- When adding more *.c files, don't forget to add them in the IN param of the Makefile
- The -l compiler flag is used to link libraries. A set of standard system directories is searched for the given library plus additional directories that are specified with the -L compiler flag.
- The -I compiler flag is used to add the directory that contains header files for preprocessing.

## Usage
Specify -L and -I flags by providing the path to the respective SDL2 library folder and include folder
```
> git clone https://github.com/Janko-dev/SDL_Template_C.git
> cd SDL_Template_C
> make
```

## References
- https://man7.org/linux/man-pages/man1/gcc.1.html
- https://www.libsdl.org/