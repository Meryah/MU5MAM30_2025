cmake_minimum_required(VERSION 3.10)
project(MU5MAM30_2025)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable debug iterators to avoid conflicts

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/common)
include_directories(${CMAKE_SOURCE_DIR}/include/utils_debug)
include_directories(${CMAKE_SOURCE_DIR}/include/fem)
include_directories(${CMAKE_SOURCE_DIR}/include/linalg)
include_directories(${CMAKE_SOURCE_DIR}/include/matrix)
include_directories(${CMAKE_SOURCE_DIR}/include/mesh)

# Source files
set(SOURCES
    main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Compiler flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
