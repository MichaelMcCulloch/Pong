# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_INCLUDE_PATH ./src/main/cpp)
project(Pong VERSION 1.0 LANGUAGES CXX)

include_directories(src)

#Include Native_Activity
add_subdirectory(src/main/cpp)