cmake_minimum_required(VERSION 3.22)
project(yit CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages
find_package(Boost REQUIRED)

# Add subfolders
set(MAIN src/main.cpp)

set(YIT_APP src/yit/app/App.cpp)
set(YIT ${YIT_APP})

# Combine sources
set(SRCS ${MAIN} ${YIT})

# Add executables
add_executable(${PROJECT_NAME} ${SRCS})

# Link targets with libraries
target_link_libraries(${PROJECT_NAME} boost::boost)
