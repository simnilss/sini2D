cmake_minimum_required(VERSION 3.11)
project("sini2D"
  LANGUAGES CXX
  VERSION 0.3
)


if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(WARNING
    "sini2D only supports and is tested in 64-bit mode. "
    "Proceed at own risk.")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(WARNING
    "No build type specified. Please set CMAKE_BUILD_TYPE "
    "if a specific build is targeted (Release or Debug).")
  set(CMAKE_BUILD_TYPE "Unspecified")
else()
  message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode")
endif()


# ----------------------------------------------------------
# Project directories
# ----------------------------------------------------------
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Build output
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")


# ----------------------------------------------------------
# Dependencies
# ----------------------------------------------------------
find_package(SDL2 REQUIRED)

add_definitions(-DGLEW_STATIC)
find_package(GLEW REQUIRED)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/catch2")


# ----------------------------------------------------------
# Source files
# ----------------------------------------------------------
set(SINI_2D_MATH_HEADERS
  "${INCLUDE_DIR}/sini2D/math/Vector.hpp"
  "${INCLUDE_DIR}/sini2D/math/Vector.inl"
  "${INCLUDE_DIR}/sini2D/math/Matrix.hpp"
  "${INCLUDE_DIR}/sini2D/math/Matrix.inl"
  "${INCLUDE_DIR}/sini2D/math/MathUtilities.hpp"
  "${INCLUDE_DIR}/sini2D/math/MathUtilities.inl"
  "${INCLUDE_DIR}/sini2D/math/MathUtilitiesBase.hpp"
  "${INCLUDE_DIR}/sini2D/math/MathUtilitiesBase.inl"
  "${INCLUDE_DIR}/sini2D/math/VectorUtilities.hpp"
  "${INCLUDE_DIR}/sini2D/math/VectorUtilities.inl"
  "${INCLUDE_DIR}/sini2D/math/MatrixUtilities.hpp"
  "${INCLUDE_DIR}/sini2D/math/MatrixUtilities.hpp"
)
set(SINI_2D_MATH_FILES
  "${SOURCE_DIR}/math/Vector.cpp"
  "${SOURCE_DIR}/math/Matrix.cpp"
  "${SOURCE_DIR}/math/MathUtilities.cpp"
  "${SOURCE_DIR}/math/MathUtilitiesBase.cpp"
  "${SOURCE_DIR}/math/VectorUtilities.cpp"
  "${SOURCE_DIR}/math/MatrixUtilities.cpp"
)
set(SINI_2D_UTIL_HEADERS
  "${INCLUDE_DIR}/sini2D/util/IO.hpp"
  "${INCLUDE_DIR}/sini2D/util/testutil.hpp"
  "${INCLUDE_DIR}/sini2D/util/testutil.inl"
)
set(SINI_2D_UTIL_FILES
  "${SOURCE_DIR}/util/IO.cpp"
  "${SOURCE_DIR}/util/testutil.cpp"
)
set(SINI_2D_GEOMETRY_HEADERS
  "${INCLUDE_DIR}/sini2D/Geometry.hpp"
  "${INCLUDE_DIR}/sini2D/geometry/Line.hpp"
  "${INCLUDE_DIR}/sini2D/geometry/Line.inl"
  "${INCLUDE_DIR}/sini2D/geometry/Polygon.hpp"
)
set(SINI_2D_GEOMETRY_FILES
  "${SOURCE_DIR}/geometry/Line.cpp"
  "${SOURCE_DIR}/geometry/Polygon.cpp"
)
set(SINI_2D_SDL_HEADERS
  "${INCLUDE_DIR}/sini2D/sdl/SdlException.hpp"
  "${INCLUDE_DIR}/sini2D/sdl/SubsystemInitializer.hpp"
  "${INCLUDE_DIR}/sini2D/sdl/Window.hpp"
)
set(SINI_2D_SDL_FILES
  "${SOURCE_DIR}/sdl/SdlException.cpp"
  "${SOURCE_DIR}/sdl/SubsystemInitializer.cpp"
  "${SOURCE_DIR}/sdl/Window.cpp"
)
set(SINI_2D_GL_HEADERS
  "${INCLUDE_DIR}/sini2D/gl/Camera.hpp"
  "${INCLUDE_DIR}/sini2D/gl/GLContext.hpp"
  "${INCLUDE_DIR}/sini2D/gl/OpenGlException.hpp"
  "${INCLUDE_DIR}/sini2D/gl/glutil.hpp"
  "${INCLUDE_DIR}/sini2D/gl/SimpleRenderer.hpp"
)
set(SINI_2D_GL_FILES
  "${SOURCE_DIR}/gl/Camera.cpp"
  "${SOURCE_DIR}/gl/GLContext.cpp"
  "${SOURCE_DIR}/gl/OpenGlException.cpp"
  "${SOURCE_DIR}/gl/SimpleRenderer.cpp"
  "${SOURCE_DIR}/gl/glutil.cpp"
)

set(SINI_2D_SRC_FILES
  "${INCLUDE_DIR}/sini2D/CudaCompat.hpp"

  "${SINI_2D_MATH_FILES}"
  "${SINI_2D_UTIL_FILES}"
  "${SINI_2D_GEOMETRY_FILES}"
  "${SINI_2D_SDL_FILES}"
  "${SINI_2D_GL_FILES}"
)

# Group source files for Visual Studio
source_group(sini2D_math FILES
  "${SINI_2D_MATH_FILES}" "${SINI_2D_MATH_HEADERS}")
source_group(sini2D_util FILES
  "${SINI_2D_UTIL_FILES}" "${SINI_2D_UTIL_HEADERS}")
source_group(sini2D_geometry FILES
  "${SINI_2D_GEOMETRY_FILES}" "${SINI_2D_GEOMETRY_HEADERS}")
source_group(sini2D_sdl FILES
  "${SINI_2D_SDL_FILES}" "${SINI_2D_SDL_HEADERS}")
source_group(sini2D_gl FILES
  "${SINI_2D_GL_FILES}" "${SINI_2D_GL_HEADERS}")


# ----------------------------------------------------------
# Library
# ----------------------------------------------------------
add_library(sini2D "${SINI_2D_SRC_FILES}")
target_include_directories(sini2D
  PUBLIC
    "${INCLUDE_DIR}"
    "${SDL2_INCLUDE_DIR}"
    "${CATCH_INCLUDE_DIR}"
)
target_link_libraries(sini2D
  "${SDL2_LIBRARY}"
  GLEW::GLEW
  OpenGL::GL
)


# ----------------------------------------------------------
# Compiler flags
# ----------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # W4 = Warnings (level 4)
  # Zi = Produce .pdb debug information
  # wd4201 = Disable warning 4201 (non-standard extension used, nameless struct/union)
  # Od = Disable optimization
  set(PRIVATE_COMPILE_FLAGS /W4 /Zi /wd4201)
  set(PRIVATE_DEBUG_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS} /Od)
  set(PRIVATE_RELEASE_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS})
  set(PRIVATE_UNSPECIFIED_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
       OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(PRIVATE_COMPILE_FLAGS -Wall -Wextra)
  set(PRIVATE_DEBUG_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS} -O0)
  set(PRIVATE_RELEASE_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS})
  set(PRIVATE_UNSPECIFIED_COMPILE_FLAGS ${PRIVATE_COMPILE_FLAGS} -O2 -g)
endif()

target_compile_features(sini2D PUBLIC cxx_std_17)
target_compile_options(sini2D
  PRIVATE
    $<$<CONFIG:Debug>:${PRIVATE_DEBUG_COMPILE_FLAGS}>
    $<$<CONFIG:Release>:${PRIVATE_RELEASE_COMPILE_FLAGS}>
    $<$<CONFIG:Unspecified>:${PRIVATE_UNSPECIFIED_COMPILE_FLAGS}>
)


# ----------------------------------------------------------
# Tests
# ----------------------------------------------------------
if(SINI_2D_BUILD_TESTS)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()
