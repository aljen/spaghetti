cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(libSpaghetti VERSION ${Spaghetti_VERSION} LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

configure_file(core/version.h.in core/version.h)

set(LIBSPAGHETTI_SOURCES
  core/api.h
  core/logger.cc
  core/logger.h
  core/registry.cc
  core/registry.h
  core/strings.h
  ${CMAKE_CURRENT_BINARY_DIR}/core/version.h
  core/version.h.in
  elements/logic/all.h
  elements/logic/and.cc
  elements/logic/and.h
  elements/logic/blinker.cc
  elements/logic/blinker.h
  elements/logic/clock.cc
  elements/logic/clock.h
  elements/logic/const_bool.cc
  elements/logic/const_bool.h
  elements/logic/const_float.cc
  elements/logic/const_float.h
  elements/logic/const_int.cc
  elements/logic/const_int.h
  elements/logic/multiply_if.cc
  elements/logic/multiply_if.h
  elements/logic/multiply.cc
  elements/logic/multiply.h
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
  elements/ui/all.h
  elements/ui/float_info.cc
  elements/ui/float_info.h
  elements/ui/push_button.cc
  elements/ui/push_button.h
  elements/ui/toggle_button.cc
  elements/ui/toggle_button.h
  elements/element.cc
  elements/element.h
  elements/package.cc
  elements/package.h
  icons/icons.qrc
  nodes/ui/float_info.cc
  nodes/ui/float_info.h
  nodes/ui/push_button.cc
  nodes/ui/push_button.h
  nodes/ui/toggle_button.cc
  nodes/ui/toggle_button.h
  nodes/blinker.cc
  nodes/blinker.h
  nodes/clock.cc
  nodes/clock.h
  nodes/const_value.cc
  nodes/const_value.h
  nodes/node.cc
  nodes/node.h
  ui/colors.cc
  ui/colors.h
  ui/editor.cc
  ui/editor.h
  ui/editor.ui
  ui/elements_list.cc
  ui/elements_list.h
  ui/expander_widget.cc
  ui/expander_widget.h
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
target_include_directories(Spaghetti
  SYSTEM PUBLIC ${PROJECT_ROOT}/vendor/spdlog/include
)
target_include_directories(Spaghetti PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(Spaghetti
  PUBLIC Qt5::Widgets
  PRIVATE Boost::filesystem ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS}
)

