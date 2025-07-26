cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(fallinc
  VERSION
    0.1.0
  LANGUAGES
    C
)

add_subdirectory(src)
if (WASM)
  add_subdirectory(web)
endif()
