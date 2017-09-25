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
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED COMPONENTS filesystem)

set(LIBSPAGHETTI_SOURCES
  core/id_manager.h
  core/metadata.h
  core/registry.cc
  core/registry.h
  core/strings.h
  elements/logic/all.h
  elements/logic/and.cc
  elements/logic/and.h
  elements/logic/clock.cc
  elements/logic/clock.h
  elements/logic/const_bool.cc
  elements/logic/const_bool.h
  elements/logic/const_float.cc
  elements/logic/const_float.h
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
  elements/logic/random_bool.cc
  elements/logic/random_bool.h
  elements/logic/switch.cc
  elements/logic/switch.h
  elements/element.cc
  elements/element.h
  elements/package.cc
  elements/package.h
  elements/types.h
  icons/icons.qrc
  nodes/node.cc
  nodes/node.h
  ui/config.cc
  ui/config.h
  ui/editor.cc
  ui/editor.h
  ui/editor.ui
  ui/elements_list.cc
  ui/elements_list.h
  ui/link_item.cc
  ui/link_item.h
  ui/nodes_view.cc
  ui/nodes_view.h
  ui/package_view.cc
  ui/package_view.h
  ui/socket_item.cc
  ui/socket_item.h
  )

set(SPAGHETTI_SOURCES
  main.cc
  )

add_library(Spaghetti STATIC ${LIBSPAGHETTI_SOURCES})
target_link_libraries(Spaghetti PUBLIC Qt5::Widgets PRIVATE Boost::filesystem ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})

add_executable(SpaghettiEditor ${SPAGHETTI_SOURCES})
target_link_libraries(SpaghettiEditor Spaghetti)

add_subdirectory(plugins)
