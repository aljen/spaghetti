cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(libSpaghetti VERSION ${Spaghetti_VERSION} LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

configure_file(include/spaghetti/version.h.in include/spaghetti/version.h)

set(LIBSPAGHETTI_SOURCES
  include/spaghetti/api.h
  include/spaghetti/element.h
  include/spaghetti/logger.h
  include/spaghetti/node.h
  include/spaghetti/package.h
  include/spaghetti/registry.h
  include/spaghetti/strings.h
  include/spaghetti/utils.h
  include/spaghetti/version.h.in

  ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/version.h

  core/logger.cc
  core/registry.cc

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
  elements/logic/demultiplexer_int.cc
  elements/logic/demultiplexer_int.h
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
  elements/logic/multiplexer_int.cc
  elements/logic/multiplexer_int.h
  elements/logic/switch.cc
  elements/logic/switch.h

  elements/math/abs.cc
  elements/math/abs.h
  elements/math/add.cc
  elements/math/add.h
  elements/math/add_if.cc
  elements/math/add_if.h
  elements/math/all.h
  elements/math/cos.cc
  elements/math/cos.h
  elements/math/divide.cc
  elements/math/divide.h
  elements/math/divide_if.cc
  elements/math/divide_if.h
  elements/math/multiply.cc
  elements/math/multiply.h
  elements/math/multiply_if.cc
  elements/math/multiply_if.h
  elements/math/sin.cc
  elements/math/sin.h
  elements/math/subtract.cc
  elements/math/subtract.h
  elements/math/subtract_if.cc
  elements/math/subtract_if.h

  elements/ui/all.h
  elements/ui/float_info.cc
  elements/ui/float_info.h
  elements/ui/int_info.cc
  elements/ui/int_info.h
  elements/ui/push_button.cc
  elements/ui/push_button.h
  elements/ui/toggle_button.cc
  elements/ui/toggle_button.h

  elements/values/all.h
  elements/values/clamp_float.cc
  elements/values/clamp_float.h
  elements/values/clamp_int.cc
  elements/values/clamp_int.h
  elements/values/const_bool.cc
  elements/values/const_bool.h
  elements/values/const_float.cc
  elements/values/const_float.h
  elements/values/const_int.cc
  elements/values/const_int.h
  elements/values/degree_to_radian.cc
  elements/values/degree_to_radian.h
  elements/values/float_to_int.cc
  elements/values/float_to_int.h
  elements/values/int_to_float.cc
  elements/values/int_to_float.h
  elements/values/max_float.cc
  elements/values/max_float.h
  elements/values/max_int.cc
  elements/values/max_int.h
  elements/values/min_float.cc
  elements/values/min_float.h
  elements/values/min_int.cc
  elements/values/min_int.h
  elements/values/radian_to_degree.cc
  elements/values/radian_to_degree.h
  elements/values/random_bool.cc
  elements/values/random_bool.h

  elements/all.h
  elements/element.cc
  elements/package.cc

  icons/icons.qrc

  nodes/ui/float_info.cc
  nodes/ui/float_info.h
  nodes/ui/push_button.cc
  nodes/ui/push_button.h
  nodes/ui/toggle_button.cc
  nodes/ui/toggle_button.h

  nodes/logic/all.h
  nodes/logic/blinker.cc
  nodes/logic/blinker.h
  nodes/logic/clock.cc
  nodes/logic/clock.h

  nodes/ui/all.h
  nodes/ui/float_info.cc
  nodes/ui/float_info.h
  nodes/ui/int_info.cc
  nodes/ui/int_info.h
  nodes/ui/push_button.cc
  nodes/ui/push_button.h
  nodes/ui/toggle_button.cc
  nodes/ui/toggle_button.h

  nodes/values/all.h
  nodes/values/const_bool.cc
  nodes/values/const_bool.h
  nodes/values/const_float.cc
  nodes/values/const_float.h
  nodes/values/const_int.cc
  nodes/values/const_int.h

  nodes/all.h
  nodes/node.cc

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
target_compile_features(Spaghetti PUBLIC cxx_std_17)
target_compile_definitions(Spaghetti
  PUBLIC SPAGHETTI_SHARED
  PRIVATE SPAGHETTI_EXPORTS ${SPAGHETTI_DEFINITIONS}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_DEFINITIONS_DEBUG}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_DEFINITIONS_RELEASE}>
  )
target_compile_options(Spaghetti
  PRIVATE ${SPAGHETTI_FLAGS}
  PRIVATE ${SPAGHETTI_FLAGS_C}
  PRIVATE ${SPAGHETTI_FLAGS_CXX}
  PRIVATE ${SPAGHETTI_FLAGS_LINKER}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_FLAGS_DEBUG}>
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_WARNINGS}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_FLAGS_RELEASE}>
  )
target_include_directories(Spaghetti PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
#  $<INSTALL_INTERFACE:include>
  )
target_include_directories(Spaghetti SYSTEM PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/spdlog/include>
##  $<INSTALL_INTERFACE:vendor>
##  $<INSTALL_INTERFACE:vendor/spdlog/include>
  )
target_link_libraries(Spaghetti
  PUBLIC ${CMAKE_THREAD_LIBS_INIT} Qt5::Widgets
  PRIVATE ${CMAKE_DL_LIBS} Boost::system Boost::filesystem
)

if (CLANG)
  target_link_libraries(Spaghetti PUBLIC -stdlib=libc++)
endif ()

install(TARGETS Spaghetti EXPORT Spaghetti
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT Spaghetti DESTINATION share/Spaghetti/cmake)
export(TARGETS Spaghetti FILE Spaghetti.cmake)
