cmake_minimum_required (VERSION 3.1.0 FATAL_ERROR)

set (PROJECT IqPostman)
set (VERSION "0.3.0")

if (UNIX)
    string (TOLOWER ${PROJECT} TARGET)
else ()
    set (TARGET ${PROJECT})
endif ()

set (IqPostman_FOUND ON CACHE BOOL "True if headers and requested libraries were found")
set (IqPostman_VERSION ${VERSION} CACHE STRING "IqPostman version")
set (IqPostman_LIBRARIES ${TARGET} CACHE STRING "IqPostman component libraries to be linked")
set (IqPostman_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" CACHE STRING "IqPostman include directories")

project (${PROJECT})

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

find_package (Qt5Network REQUIRED)
find_package (Qt5Gui REQUIRED)

file (GLOB SOURCES src/*.cpp)
source_group ("Source Files" FILES ${SOURCES})
file (GLOB HEADERS src/*.h)
source_group ("Header Files" FILES ${HEADERS})

add_definitions (-DIQPOSTMAN_LIBRARY)
add_definitions (-DLIB_VERSION=${VERSION})

set (VERSIONINFO_RES_FILE "")
if(MINGW)
    file (GLOB VERSIONINFO_RES_FILE . src/versioninfo.rc)
    SET (CMAKE_RC_COMPILE_OBJECT
            "<CMAKE_RC_COMPILER> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(MINGW)

add_library (${TARGET} SHARED ${SOURCES} ${VERSIONINFO_RES_FILE})

target_link_libraries (${TARGET} Qt5::Network Qt5::Gui)

set_target_properties (${TARGET} PROPERTIES
                       VERSION ${VERSION})

if (MINGW)
    add_definitions(-std=c++11)
else ()
    target_compile_features (${TARGET} PRIVATE cxx_range_for)
endif ()

if (MSVC)
    add_definitions (/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -pedantic)
else ()
    message ("Unknown compiler")
endif ()
