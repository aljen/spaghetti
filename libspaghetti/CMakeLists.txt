cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(libSpaghetti VERSION 17.09.06 LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(LIBSPAGHETTI_SOURCES
  core/api.h
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
  icons/icons.qrc
  nodes/node.cc
  nodes/node.h
  ui/colors.cc
  ui/colors.h
  ui/editor.cc
  ui/editor.h
  ui/editor.ui
  ui/elements_list.cc
  ui/elements_list.h
  ui/link_item.cc
  ui/link_item.h
  ui/package_view.cc
  ui/package_view.h
  ui/socket_item.cc
  ui/socket_item.h
  )

add_library(Spaghetti SHARED ${LIBSPAGHETTI_SOURCES})
target_compile_definitions(Spaghetti
  PUBLIC -DSPAGHETTI_SHARED
  PRIVATE -DSPAGHETTI_EXPORTS
  )
target_include_directories(Spaghetti PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(Spaghetti
  PUBLIC Qt5::Widgets
  PRIVATE Boost::filesystem ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS}
)

