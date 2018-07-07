cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(SpaghettiEditor VERSION ${Spaghetti_VERSION} LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

if (SPAGHETTI_USE_OPENGL)
  find_package(Qt5 REQUIRED COMPONENTS OpenGL)
endif ()

if (SPAGHETTI_USE_CHARTS)
  find_package(Qt5 REQUIRED COMPONENTS Charts)
endif ()

set(SPAGHETTI_CHARTS_SOURCES
  source/nodes/values/characteristic_curve/editor_widget.cc
  source/nodes/values/characteristic_curve/editor_widget.h
  source/nodes/values/characteristic_curve/editor_window.cc
  source/nodes/values/characteristic_curve/editor_window.h
  source/nodes/values/characteristic_curve/editor_window.ui
  source/nodes/values/characteristic_curve/generators.cc
  source/nodes/values/characteristic_curve/generators.h
  source/nodes/values/characteristic_curve/line.cc
  source/nodes/values/characteristic_curve/line.h
  source/nodes/values/characteristic_curve/point.cc
  source/nodes/values/characteristic_curve/point.h
  source/nodes/values/characteristic_curve.cc
  source/nodes/values/characteristic_curve.h
  )

add_executable(SpaghettiEditor
  icons/icons.qrc
  include/spaghetti/editor/editor.h
  include/spaghetti/editor/node.h
  include/spaghetti/editor/socket_item.h

  source/nodes/logic/all.h
  source/nodes/logic/blinker.cc
  source/nodes/logic/blinker.h
  source/nodes/logic/clock.cc
  source/nodes/logic/clock.h

  source/nodes/pneumatic/all.h
  source/nodes/pneumatic/tank.cc
  source/nodes/pneumatic/tank.h

  source/nodes/ui/all.h
  source/nodes/ui/float_info.cc
  source/nodes/ui/float_info.h
  source/nodes/ui/int_info.cc
  source/nodes/ui/int_info.h
  source/nodes/ui/push_button.cc
  source/nodes/ui/push_button.h
  source/nodes/ui/seven_segment_display.cc
  source/nodes/ui/seven_segment_display.h
  source/nodes/ui/toggle_button.cc
  source/nodes/ui/toggle_button.h

  source/nodes/values/all.h
  source/nodes/values/const_bool.cc
  source/nodes/values/const_bool.h
  source/nodes/values/const_float.cc
  source/nodes/values/const_float.h
  source/nodes/values/const_int.cc
  source/nodes/values/const_int.h
  source/nodes/values/random_float.cc
  source/nodes/values/random_float.h
  source/nodes/values/random_float_if.cc
  source/nodes/values/random_float_if.h
  source/nodes/values/random_int.cc
  source/nodes/values/random_int.h
  source/nodes/values/random_int_if.cc
  source/nodes/values/random_int_if.h

  source/nodes/all.h
  source/nodes/node.cc
  source/nodes/package.cc
  source/nodes/package.h

  source/colors.cc
  source/colors.h
  source/editor.cc
  source/editor.ui
  source/elements_list.cc
  source/elements_list.h
  source/expander_widget.cc
  source/expander_widget.h
  source/link_item.cc
  source/link_item.h
  source/main.cc
  source/nodes_registry.cc
  source/nodes_registry.h
  source/package_view.cc
  source/package_view.h
  source/socket_item.cc
  )

target_sources(SpaghettiEditor PRIVATE $<$<BOOL:${SPAGHETTI_USE_CHARTS}>:${SPAGHETTI_CHARTS_SOURCES}>)

target_compile_definitions(SpaghettiEditor
  PRIVATE ${SPAGHETTI_DEFINITIONS}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_DEFINITIONS_DEBUG}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_DEFINITIONS_RELEASE}>
  PRIVATE $<$<BOOL:${SPAGHETTI_USE_OPENGL}>:SPAGHETTI_USE_OPENGL>
  PRIVATE $<$<BOOL:${SPAGHETTI_USE_CHARTS}>:SPAGHETTI_USE_CHARTS>
  )

target_include_directories(SpaghettiEditor
  PRIVATE include
  PRIVATE source
  )

target_compile_options(SpaghettiEditor
  PRIVATE ${SPAGHETTI_FLAGS}
  PRIVATE ${SPAGHETTI_FLAGS_C}
  PRIVATE ${SPAGHETTI_FLAGS_CXX}
  PRIVATE ${SPAGHETTI_FLAGS_LINKER}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_FLAGS_DEBUG}>
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_WARNINGS}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_FLAGS_RELEASE}>
  )

target_link_libraries(SpaghettiEditor
  PUBLIC Spaghetti
  PUBLIC Qt5::Widgets
  PUBLIC $<$<BOOL:${SPAGHETTI_USE_OPENGL}>:Qt5::OpenGL>
  PUBLIC $<$<BOOL:${SPAGHETTI_USE_CHARTS}>:Qt5::Charts>
  )

install(TARGETS SpaghettiEditor
  COMPONENT Editor
  EXPORT SpaghettiEditor
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
