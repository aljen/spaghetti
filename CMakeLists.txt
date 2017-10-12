cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(Spaghetti
  VERSION 17.10.12
  DESCRIPTION "Graphical logic editor/simulator"
  LANGUAGES C CXX
)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(DefaultOptions)
include(BuildDirectories)
include(BuildType)
include(CompilerType)
include(CompilerFlags)

set(VENDOR_ROOT ${PROJECT_ROOT}/vendor)
include_directories(SYSTEM ${VENDOR_ROOT})

add_subdirectory(libspaghetti)
add_subdirectory(editor)
add_subdirectory(plugins)
