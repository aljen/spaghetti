cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(Spaghetti VERSION 17.09.06 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(DefaultOptions)
include(BuildDirectories)
include(BuildType)
include(CompilerType)
include(CompilerFlags)

set(VENDOR_ROOT ${PROJECT_ROOT}/vendor)
include_directories(SYSTEM ${VENDOR_ROOT})

find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SPAGHETTI_SOURCES
  core/id_manager.h
  core/metadata.h
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
