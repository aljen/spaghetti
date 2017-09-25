cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(ExamplePlugin VERSION 17.09.06 LANGUAGES C CXX)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(EXAMPLE_SOURCES
  example.cc
  )

add_library(Example SHARED ${EXAMPLE_SOURCES})
target_include_directories(Example PRIVATE ${PROJECT_ROOT})
set_target_properties(Example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
set_target_properties(Example PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")

target_link_libraries(Example Spaghetti)
