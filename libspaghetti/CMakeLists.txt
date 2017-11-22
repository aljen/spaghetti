cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(libSpaghetti VERSION ${Spaghetti_VERSION} LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

configure_file(core/version.h.in core/version.h)

set(LIBSPAGHETTI_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/core/version.h
  core/api.h
  core/logger.cc
  core/logger.h
  core/registry.cc
  core/registry.h
  core/strings.h
  core/version.h.in

  elements/arithmetic/add.cc
  elements/arithmetic/add.h
  elements/arithmetic/add_if.cc
  elements/arithmetic/add_if.h
  elements/arithmetic/all.h
  elements/arithmetic/multiply.cc
  elements/arithmetic/multiply.h
  elements/arithmetic/multiply_if.cc
  elements/arithmetic/multiply_if.h

  elements/const/all.h
  elements/const/bool.cc
  elements/const/bool.h
  elements/const/float.cc
  elements/const/float.h
  elements/const/int.cc
  elements/const/int.h

  elements/gates/all.h
  elements/gates/and.cc
  elements/gates/and.h
  elements/gates/nand.cc
  elements/gates/nand.h
  elements/gates/nor.cc
  elements/gates/nor.h
  elements/gates/not.cc
  elements/gates/not.h
  elements/gates/or.cc
  elements/gates/or.h

  elements/logic/all.h
  elements/logic/blinker.cc
  elements/logic/blinker.h
  elements/logic/clock.cc
  elements/logic/clock.h
  elements/logic/if_equal.cc
  elements/logic/if_equal.h
  elements/logic/if_greater.cc
  elements/logic/if_greater.h
  elements/logic/if_greater_equal.cc
  elements/logic/if_greater_equal.h
  elements/logic/if_lower.cc
  elements/logic/if_lower.h
  elements/logic/if_lower_equal.cc
  elements/logic/if_lower_equal.h
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

  elements/all.h
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

  nodes/const/all.h
  nodes/const/bool.cc
  nodes/const/bool.h
  nodes/const/float.cc
  nodes/const/float.h
  nodes/const/int.cc
  nodes/const/int.h

  nodes/logic/all.h
  nodes/logic/blinker.cc
  nodes/logic/blinker.h
  nodes/logic/clock.cc
  nodes/logic/clock.h

  nodes/ui/all.h
  nodes/ui/float_info.cc
  nodes/ui/float_info.h
  nodes/ui/push_button.cc
  nodes/ui/push_button.h
  nodes/ui/toggle_button.cc
  nodes/ui/toggle_button.h

  nodes/all.h
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
  PUBLIC ${CMAKE_THREAD_LIBS_INIT} Qt5::Widgets
  PRIVATE ${CMAKE_DL_LIBS} Boost::filesystem
)

