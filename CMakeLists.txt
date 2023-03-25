cmake_minimum_required(VERSION 3.22)
project(yit CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages
find_package(Boost REQUIRED)
find_package(fmt REQUIRED)

# Add modules
## Add actions
set(YIT_INIT_ACTION src/yit/actions/init/InitAction.cpp)
set(YIT_CAT_FILE_ACTION src/yit/actions/catfile/CatFileAction.cpp)
set(YIT_ACTIONS ${YIT_INIT_ACTION} ${YIT_CAT_FILE_ACTION})
## Add utils
set(YIT_UTILS src/yit/utils/utils.cpp)
## Add app
set(YIT_APP src/yit/app/App.cpp)

## Combine YIT
set(YIT ${YIT_APP} ${YIT_ACTIONS} ${YIT_UTILS})

# Add main
set(MAIN src/main.cpp)

# Combine sources
set(SRCS ${MAIN} ${YIT})

# Add executables
add_executable(${PROJECT_NAME} ${SRCS})

# Link targets with libraries
target_link_libraries(${PROJECT_NAME} boost::boost)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
