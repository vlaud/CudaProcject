#---------------------------------------------------------------------------------------------------
# CMake Build Configuration for the Ray Tracing Weekend Series
#
# See README.md for guidance.
#---------------------------------------------------------------------------------------------------

cmake_minimum_required ( VERSION 3.1.0...4.0.0 )

project ( RTWeekend LANGUAGES CXX )

# 2. 시스템에 CUDA가 설치되어 있는지 명시적으로 찾습니다.
find_package(CUDAToolkit REQUIRED)

# 3. 그 후 CUDA 언어를 활성화합니다.
enable_language(CUDA)

# Set to C++11
set ( CMAKE_CXX_STANDARD          11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

# Source

set ( 2_OUTPUT_AN_IMAGE
  "2. Output an Image/kernel.cu"
)

set ( 3_THE_VEC3_CLASS
  "3. The vec3 Class/kernel.cu"
  "3. The vec3 Class/color.h"
  "3. The vec3 Class/vec3.h"
)

set ( 4_RAYS_A_SIMPLE_CAMERA_AND_BACKGROUND
  "4. Rays, a Simple Camera, and Background/kernel.cu"
  "4. Rays, a Simple Camera, and Background/color.h"
  "4. Rays, a Simple Camera, and Background/vec3.h"
  "4. Rays, a Simple Camera, and Background/ray.h"
)


# Specific compiler flags below. We're not going to add options for all possible compilers, but if
# you're new to CMake (like we are), the following may be a helpful example if you're using a
# different compiler or want to set different compiler options.

message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message (STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message (STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # CXX(C++) 소스파일을 컴파일할 때만 해당 플래그들을 적용하도록 묶어줍니다.
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/W4;/we4265;/we5204;/wd4100;/wd4819>")
    
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor;-Wreorder;-Wmaybe-uninitialized;-Wunused-variable>")
    
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor;-Wreorder;-Wsometimes-uninitialized;-Wunused-variable>")
endif()

# Executables
add_executable(2_output_an_image     ${2_OUTPUT_AN_IMAGE})
add_executable(3_the_vec3_class     ${3_THE_VEC3_CLASS})
add_executable(4_rays_simple_camera_and_bg       ${4_RAYS_A_SIMPLE_CAMERA_AND_BACKGROUND})
