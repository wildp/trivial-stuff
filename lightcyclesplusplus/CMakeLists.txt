cmake_minimum_required (VERSION 3.7)
project (Lightcycles++)

set(SDL2_INCLUDE_DIR  "PATH_TO_SDL2_INCLUDE_DIRECTORY")
set(SDL2_LIBRARY_DIR  "PATH_TO_SDL2_LIBRARY_DIRECTORY")
set(SDL_TTF_INCLUDE_DIR "PATH_TO_SDL_TTF_INCLUDE_DIRECTORY")
set(SDL_TTF_LIBRARY_DIR "PATH_TO_SDL_TTF_LIBRARY_DIRECTORY")

include_directories(${PROJECT_BINARY_DIR})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL_TTF_INCLUDE_DIR})

set(SOURCE_FILES main.cpp. game.cpp log.cpp player.cpp text.cpp)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
else ()
	add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif ()

target_link_libraries (${PROJECT_NAME} ${SDL2_LIBRARY_DIR}/SDL2.lib)
target_link_libraries (${PROJECT_NAME} ${SDL2_LIBRARY_DIR}/SDL2main.lib)
target_link_libraries (${PROJECT_NAME} ${SDL2_LIBRARY_DIR}/SDL2test.lib)
target_link_libraries (${PROJECT_NAME} ${SDL_TTF_LIBRARY_DIR}/SDL2_ttf.lib)

