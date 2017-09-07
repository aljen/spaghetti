cmake_minimum_required(VERSION 3.8)

project(Spaghetti VERSION 17.09.06 LANGUAGES C CXX)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_LINK_WHAT_YOU_USE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
set_property(GLOBAL PROPERTY CXX_STANDARD 17)

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif ()
if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif ()
if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  if (UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
  else ()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  endif ()
endif ()

if (UNIX)
#  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-cppcoreguidelines-pro-type-const-cast")
  set(FLAGS_COMMON "-march=native")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS_COMMON}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")
endif ()

set(VENDOR_ROOT ${PROJECT_ROOT}/vendor)
include_directories(SYSTEM ${VENDOR_ROOT})

find_package(Threads REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(SPAGHETTI_SOURCES
  core/id_manager.h
  core/registry.cc
  core/registry.h
  core/strings.h
  elements/logic/all.h
  elements/logic/and.cc
  elements/logic/and.h
  elements/logic/const_bool.cc
  elements/logic/const_bool.h
  elements/logic/const_int.cc
  elements/logic/const_int.h
  elements/logic/nand.cc
  elements/logic/nand.h
  elements/logic/nor.cc
  elements/logic/nor.h
  elements/logic/not.cc
  elements/logic/not.h
  elements/logic/or.cc
  elements/logic/or.h
  elements/logic/switch.cc
  elements/logic/switch.h
  elements/element.cc
  elements/element.h
  elements/package.cc
  elements/package.h
  elements/types.h
  main.cc
  mainwindow.cc
  mainwindow.h
  mainwindow.ui
  )

add_executable(Spaghetti ${SPAGHETTI_SOURCES})
target_link_libraries(Spaghetti Qt5::Widgets ${CMAKE_THREAD_LIBS_INIT})
