cmake_minimum_required(VERSION 3.10)
project(problem_034_skiplist LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(code src/main.cpp)
target_include_directories(code PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Optimize
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(code PRIVATE -O2 -pipe -static-libstdc++ -static-libgcc)
endif()
