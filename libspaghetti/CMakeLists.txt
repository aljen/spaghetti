cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(libSpaghetti VERSION ${Spaghetti_VERSION} LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
if (SPAGHETTI_USE_OPENGL)
  find_package(Qt5 REQUIRED COMPONENTS OpenGL)
endif ()

if (HAVE_CXX_FILESYSTEM)
  set(SPAGHETTI_FILESYSTEM_IMPLEMENTATION SPAGHETTI_FS_STD_FILESYSTEM)
elseif (HAVE_CXX_EXPERIMENTAL_FILESYSTEM)
  set(SPAGHETTI_FILESYSTEM_IMPLEMENTATION SPAGHETTI_FS_STD_EXPERIMENTAL_FILESYSTEM)
elseif (HAVE_BOOST_FILESYSTEM)
  set(SPAGHETTI_FILESYSTEM_IMPLEMENTATION SPAGHETTI_FS_BOOST_FILESYSTEM)
endif ()

configure_file(source/filesystem.h.in include/filesystem.h)
configure_file(include/spaghetti/version.h.in include/spaghetti/version.h)

set(LIBSPAGHETTI_PUBLIC_HEADERS
  include/spaghetti/api.h
  include/spaghetti/editor.h
  include/spaghetti/element.h
  include/spaghetti/logger.h
  include/spaghetti/node.h
  include/spaghetti/package.h
  include/spaghetti/registry.h
  include/spaghetti/socket_item.h
  include/spaghetti/strings.h
  include/spaghetti/utils.h
  )

set(LIBSPAGHETTI_GENERATED_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/version.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/filesystem.h
  )

set(LIBSPAGHETTI_SOURCES
  ${LIBSPAGHETTI_PUBLIC_HEADERS}
  include/spaghetti/version.h.in

  source/elements/gates/all.h
  source/elements/gates/and.cc
  source/elements/gates/and.h
  source/elements/gates/nand.cc
  source/elements/gates/nand.h
  source/elements/gates/nor.cc
  source/elements/gates/nor.h
  source/elements/gates/not.cc
  source/elements/gates/not.h
  source/elements/gates/or.cc
  source/elements/gates/or.h

  source/elements/logic/all.h
  source/elements/logic/blinker.cc
  source/elements/logic/blinker.h
  source/elements/logic/clock.cc
  source/elements/logic/clock.h
  source/elements/logic/demultiplexer_int.cc
  source/elements/logic/demultiplexer_int.h
  source/elements/logic/if_equal.cc
  source/elements/logic/if_equal.h
  source/elements/logic/if_greater.cc
  source/elements/logic/if_greater.h
  source/elements/logic/if_greater_equal.cc
  source/elements/logic/if_greater_equal.h
  source/elements/logic/if_lower.cc
  source/elements/logic/if_lower.h
  source/elements/logic/if_lower_equal.cc
  source/elements/logic/if_lower_equal.h
  source/elements/logic/multiplexer_int.cc
  source/elements/logic/multiplexer_int.h
  source/elements/logic/switch.cc
  source/elements/logic/switch.h

  source/elements/math/abs.cc
  source/elements/math/abs.h
  source/elements/math/add.cc
  source/elements/math/add.h
  source/elements/math/add_if.cc
  source/elements/math/add_if.h
  source/elements/math/all.h
  source/elements/math/bcd.cc
  source/elements/math/bcd.h
  source/elements/math/cos.cc
  source/elements/math/cos.h
  source/elements/math/divide.cc
  source/elements/math/divide.h
  source/elements/math/divide_if.cc
  source/elements/math/divide_if.h
  source/elements/math/multiply.cc
  source/elements/math/multiply.h
  source/elements/math/multiply_if.cc
  source/elements/math/multiply_if.h
  source/elements/math/sin.cc
  source/elements/math/sin.h
  source/elements/math/subtract.cc
  source/elements/math/subtract.h
  source/elements/math/subtract_if.cc
  source/elements/math/subtract_if.h

  source/elements/ui/all.h
  source/elements/ui/bcd_to_seven_segment_display.cc
  source/elements/ui/bcd_to_seven_segment_display.h
  source/elements/ui/float_info.cc
  source/elements/ui/float_info.h
  source/elements/ui/int_info.cc
  source/elements/ui/int_info.h
  source/elements/ui/push_button.cc
  source/elements/ui/push_button.h
  source/elements/ui/toggle_button.cc
  source/elements/ui/toggle_button.h
  source/elements/ui/seven_segment_display.cc
  source/elements/ui/seven_segment_display.h

  source/elements/values/all.h
  source/elements/values/clamp_float.cc
  source/elements/values/clamp_float.h
  source/elements/values/clamp_int.cc
  source/elements/values/clamp_int.h
  source/elements/values/const_bool.cc
  source/elements/values/const_bool.h
  source/elements/values/const_float.cc
  source/elements/values/const_float.h
  source/elements/values/const_int.cc
  source/elements/values/const_int.h
  source/elements/values/degree_to_radian.cc
  source/elements/values/degree_to_radian.h
  source/elements/values/float_to_int.cc
  source/elements/values/float_to_int.h
  source/elements/values/int_to_float.cc
  source/elements/values/int_to_float.h
  source/elements/values/max_float.cc
  source/elements/values/max_float.h
  source/elements/values/max_int.cc
  source/elements/values/max_int.h
  source/elements/values/min_float.cc
  source/elements/values/min_float.h
  source/elements/values/min_int.cc
  source/elements/values/min_int.h
  source/elements/values/radian_to_degree.cc
  source/elements/values/radian_to_degree.h
  source/elements/values/random_bool.cc
  source/elements/values/random_bool.h

  source/elements/all.h

  source/icons/icons.qrc

  source/nodes/logic/all.h
  source/nodes/logic/blinker.cc
  source/nodes/logic/blinker.h
  source/nodes/logic/clock.cc
  source/nodes/logic/clock.h

  source/nodes/ui/all.h
  source/nodes/ui/float_info.cc
  source/nodes/ui/float_info.h
  source/nodes/ui/int_info.cc
  source/nodes/ui/int_info.h
  source/nodes/ui/push_button.cc
  source/nodes/ui/push_button.h
  source/nodes/ui/toggle_button.cc
  source/nodes/ui/toggle_button.h
  source/nodes/ui/seven_segment_display.cc
  source/nodes/ui/seven_segment_display.h

  source/nodes/values/all.h
  source/nodes/values/const_bool.cc
  source/nodes/values/const_bool.h
  source/nodes/values/const_float.cc
  source/nodes/values/const_float.h
  source/nodes/values/const_int.cc
  source/nodes/values/const_int.h

  source/nodes/all.h

  source/ui/colors.cc
  source/ui/colors.h
  source/ui/editor.cc
  source/ui/editor.ui
  source/ui/elements_list.cc
  source/ui/elements_list.h
  source/ui/expander_widget.cc
  source/ui/expander_widget.h
  source/ui/link_item.cc
  source/ui/link_item.h
  source/ui/package_view.cc
  source/ui/package_view.h
  source/ui/socket_item.cc

  source/element.cc
  source/logger.cc
  source/node.cc
  source/package.cc
  source/registry.cc
  source/shared_library.cc
  source/shared_library.h
  source/filesystem.h.in
  )

set(LIBSPAGHETTI_ALL_SOURCES
  ${LIBSPAGHETTI_GENERATED_SOURCES}
  ${LIBSPAGHETTI_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIBSPAGHETTI_SOURCES})
source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${LIBSPAGHETTI_GENERATED_SOURCES})

add_library(Spaghetti SHARED ${LIBSPAGHETTI_ALL_SOURCES})
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
target_include_directories(Spaghetti
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  PUBLIC $<INSTALL_INTERFACE:include>
  PRIVATE source
  )
target_include_directories(Spaghetti SYSTEM PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/spdlog/include>
##  $<INSTALL_INTERFACE:vendor>
##  $<INSTALL_INTERFACE:vendor/spdlog/include>
  )
target_link_libraries(Spaghetti
  PUBLIC ${CMAKE_THREAD_LIBS_INIT} Qt5::Widgets Qt5::OpenGL
  PRIVATE ${CMAKE_DL_LIBS} ${CXX_FILESYSTEM_LIBS}
)
if (SPAGHETTI_USE_OPENGL)
  target_compile_definitions(Spaghetti PRIVATE SPAGHETTI_USE_OPENGL)
  target_link_libraries(Spaghetti PUBLIC Qt5::OpenGL)
endif ()

if (CLANG)
  target_link_libraries(Spaghetti PUBLIC -stdlib=libc++)
endif ()

install(TARGETS Spaghetti
  COMPONENT SDK
  EXPORT SpaghettiConfig
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
install(FILES ${LIBSPAGHETTI_PUBLIC_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/version.h
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti)
install(EXPORT SpaghettiConfig
  COMPONENT SDK
  DESTINATION share/Spaghetti/cmake)
export(TARGETS Spaghetti FILE SpaghettiConfig.cmake)

# Copy vendor headers to libspaghetti/include
message (STATUS "Synchronizing vendor headers...")
foreach(HEADER ${VENDOR_FREE_STANDING_HEADERS})
  configure_file(${VENDOR_ROOT}/${HEADER} ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/vendor/${HEADER} COPYONLY)
  install(FILES ${VENDOR_ROOT}/${HEADER}
    COMPONENT SDK
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/vendor)
endforeach ()
foreach(HEADER ${VENDOR_SPDLOG_HEADERS})
  configure_file(${VENDOR_ROOT}/spdlog/include/spdlog/${HEADER} ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/vendor/spdlog/${HEADER} COPYONLY)
endforeach ()
install(DIRECTORY ${VENDOR_ROOT}/spdlog/include/spdlog
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/vendor)
