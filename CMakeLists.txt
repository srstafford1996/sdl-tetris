# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Project name
project(Tetris VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL3_DIR "C:\\SDL\\SDL3")

find_package(SDL3 REQUIRED HINTS ${SDL3_DIR})

set(HEADER_FILES
    include/render.hpp
    include/game.hpp
    include/common.hpp
    include/board.hpp
    include/player.hpp
)
add_definitions(-D_AMD64_)
# Manually specify source files
set(SOURCE_FILES
    src/main.cpp
    src/render.cpp
    src/game.cpp
    src/board.cpp
    src/player.cpp
)
include_directories(include)
# Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} SDL3::SDL3) 
# Build the exe in the base project folder
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
)
# Get DLL locations
get_target_property(SDL3_DLL_PATH SDL3::SDL3 IMPORTED_LOCATION)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_DLL_PATH}"
        "${CMAKE_SOURCE_DIR}/bin"
   COMMENT "Copying SDL3 DLLs to output directory"
)