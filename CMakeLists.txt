cmake_minimum_required(VERSION 3.22)
project(yit CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages

# Add subfolders
set(MAIN src/main.cpp)

# Add executables
add_executable(${PROJECT_NAME} ${MAIN})

# Link targets with libraries

