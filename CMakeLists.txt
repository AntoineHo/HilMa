cmake_minimum_required (VERSION 2.8.12)
PROJECT(HilMa CXX)

## Sets version number
set (MAJOR 0)
set (MINOR 1)
set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
# CHANGE INCLUDE VALUES BY DIRECTORIES IN OUTPUT OF pkg-config --cflags cairo (remove -Is)
INCLUDE_DIRECTORIES(/usr/include/cairo /usr/include/glib-2.0 /usr/lib/x86_64-linux-gnu/glib-2.0/include /usr/include/pixman-1 /usr/include/freetype2 /usr/include/libpng12)
FILE(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

ADD_EXECUTABLE(HilMa ${SOURCES})
TARGET_LINK_LIBRARIES(HilMa PRIVATE -lcairo)
