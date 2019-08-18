# Lightcycles++ Instructions

* Download the development libraries for [SDL2](https://www.libsdl.org/download-2.0.php)
and [SDL_TTF](https://www.libsdl.org/projects/SDL_ttf/).

``` Cmake
set(SDL2_INCLUDE_DIR  "PATH_TO_SDL2_INCLUDE_DIRECTORY")
set(SDL2_LIBRARY_DIR  "PATH_TO_SDL2_LIBRARY_DIRECTORY")
set(SDL_TTF_INCLUDE_DIR "PATH_TO_SDL_TTF_INCLUDE_DIRECTORY")
set(SDL_TTF_LIBRARY_DIR "PATH_TO_SDL_TTF_LIBRARY_DIRECTORY")
```
* In [CMakeLists.txt](CMakeLists.txt), Change `PATH_TO_SDL2_INCLUDE_DIRECTORY`, `PATH_TO_SDL2_LIBRARY_DIRECTORY`,
`PATH_TO_SDL2_TTF_INCLUDE_DIRECTORY`, and `PATH_TO_SDL2_TTF_LIBRARY_DIRECTORY`
to their respective paths.

* Download runtime binaries for SDL2 and SDL_ttf
and put them in the same folder as the above binaries:
  * https://www.libsdl.org/download-2.0.php
  * https://www.libsdl.org/projects/SDL_ttf/
