cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(libSpaghetti VERSION ${Spaghetti_VERSION} LANGUAGES C CXX)

find_package(Threads REQUIRED)

if (HAVE_CXX_FILESYSTEM)
  set(SPAGHETTI_FILESYSTEM_IMPLEMENTATION SPAGHETTI_FS_STD_FILESYSTEM)
elseif (HAVE_CXX_EXPERIMENTAL_FILESYSTEM)
  set(SPAGHETTI_FILESYSTEM_IMPLEMENTATION SPAGHETTI_FS_STD_EXPERIMENTAL_FILESYSTEM)
elseif (HAVE_BOOST_FILESYSTEM)
  set(SPAGHETTI_FILESYSTEM_IMPLEMENTATION SPAGHETTI_FS_BOOST_FILESYSTEM)
endif ()

configure_file(include/spaghetti/filesystem.h.in include/spaghetti/filesystem.h)
configure_file(include/spaghetti/version.h.in include/spaghetti/version.h)

set(LIBSPAGHETTI_PUBLIC_GATES_HEADERS
  include/spaghetti/elements/gates/all.h
  include/spaghetti/elements/gates/and.h
  include/spaghetti/elements/gates/nand.h
  include/spaghetti/elements/gates/nor.h
  include/spaghetti/elements/gates/not.h
  include/spaghetti/elements/gates/or.h
  )
set(LIBSPAGHETTI_PUBLIC_LOGIC_HEADERS
  include/spaghetti/elements/logic/all.h
  include/spaghetti/elements/logic/assign_float.h
  include/spaghetti/elements/logic/assign_int.h
  include/spaghetti/elements/logic/blinker.h
  include/spaghetti/elements/logic/counter_down.h
  include/spaghetti/elements/logic/counter_up.h
  include/spaghetti/elements/logic/counter_up_down.h
  include/spaghetti/elements/logic/demultiplexer_int.h
  include/spaghetti/elements/logic/if_equal.h
  include/spaghetti/elements/logic/if_greater.h
  include/spaghetti/elements/logic/if_greater_equal.h
  include/spaghetti/elements/logic/if_lower.h
  include/spaghetti/elements/logic/if_lower_equal.h
  include/spaghetti/elements/logic/latch.h
  include/spaghetti/elements/logic/memory_difference.h
  include/spaghetti/elements/logic/memory_reset_set.h
  include/spaghetti/elements/logic/memory_set_reset.h
  include/spaghetti/elements/logic/multiplexer_int.h
  include/spaghetti/elements/logic/pid.h
  include/spaghetti/elements/logic/snapshot_float.h
  include/spaghetti/elements/logic/snapshot_int.h
  include/spaghetti/elements/logic/switch.h
  include/spaghetti/elements/logic/trigger_falling.h
  include/spaghetti/elements/logic/trigger_rising.h
  )
set(LIBSPAGHETTI_PUBLIC_MATH_HEADERS
  include/spaghetti/elements/math/abs.h
  include/spaghetti/elements/math/add.h
  include/spaghetti/elements/math/add_if.h
  include/spaghetti/elements/math/all.h
  include/spaghetti/elements/math/bcd.h
  include/spaghetti/elements/math/cos.h
  include/spaghetti/elements/math/divide.h
  include/spaghetti/elements/math/divide_if.h
  include/spaghetti/elements/math/lerp.h
  include/spaghetti/elements/math/multiply.h
  include/spaghetti/elements/math/multiply_if.h
  include/spaghetti/elements/math/sign.h
  include/spaghetti/elements/math/sin.h
  include/spaghetti/elements/math/square_root.h
  include/spaghetti/elements/math/subtract.h
  include/spaghetti/elements/math/subtract_if.h
  )
set(LIBSPAGHETTI_PUBLIC_PNEUMATIC_HEADERS
  include/spaghetti/elements/pneumatic/all.h
  include/spaghetti/elements/pneumatic/tank.h
  include/spaghetti/elements/pneumatic/valve.h
  )
set(LIBSPAGHETTI_PUBLIC_TIMERS_HEADERS
  include/spaghetti/elements/timers/all.h
  include/spaghetti/elements/timers/clock.h
  include/spaghetti/elements/timers/delta_time.h
  include/spaghetti/elements/timers/t_off.h
  include/spaghetti/elements/timers/t_on.h
  include/spaghetti/elements/timers/t_pulse.h
  )
set(LIBSPAGHETTI_PUBLIC_UI_HEADERS
  include/spaghetti/elements/ui/all.h
  include/spaghetti/elements/ui/bcd_to_seven_segment_display.h
  include/spaghetti/elements/ui/float_info.h
  include/spaghetti/elements/ui/int_info.h
  include/spaghetti/elements/ui/push_button.h
  include/spaghetti/elements/ui/toggle_button.h
  include/spaghetti/elements/ui/seven_segment_display.h
  )
set(LIBSPAGHETTI_PUBLIC_VALUES_HEADERS
  include/spaghetti/elements/values/all.h
  include/spaghetti/elements/values/characteristic_curve.h
  include/spaghetti/elements/values/clamp_float.h
  include/spaghetti/elements/values/clamp_int.h
  include/spaghetti/elements/values/const_bool.h
  include/spaghetti/elements/values/const_float.h
  include/spaghetti/elements/values/const_int.h
  include/spaghetti/elements/values/degree_to_radian.h
  include/spaghetti/elements/values/float_to_int.h
  include/spaghetti/elements/values/int_to_float.h
  include/spaghetti/elements/values/max_float.h
  include/spaghetti/elements/values/max_int.h
  include/spaghetti/elements/values/min_float.h
  include/spaghetti/elements/values/min_int.h
  include/spaghetti/elements/values/radian_to_degree.h
  include/spaghetti/elements/values/random_bool.h
  include/spaghetti/elements/values/random_float.h
  include/spaghetti/elements/values/random_float_if.h
  include/spaghetti/elements/values/random_int.h
  include/spaghetti/elements/values/random_int_if.h
  )
set(LIBSPAGHETTI_PUBLIC_ELEMENTS_ALL_HEADERS
  include/spaghetti/elements/all.h
  )
set(LIBSPAGHETTI_PUBLIC_COMMON_HEADERS
  include/spaghetti/api.h
  include/spaghetti/element.h
  include/spaghetti/logger.h
  include/spaghetti/package.h
  include/spaghetti/registry.h
  include/spaghetti/strings.h
  include/spaghetti/shared_library.h
  include/spaghetti/utils.h
  )
set(LIBSPAGHETTI_PUBLIC_HEADERS
  ${LIBSPAGHETTI_PUBLIC_GATES_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_LOGIC_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_MATH_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_PNEUMATIC_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_TIMERS_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_UI_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_VALUES_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_ELEMENTS_ALL_HEADERS}
  ${LIBSPAGHETTI_PUBLIC_COMMON_HEADERS}
  )

set(LIBSPAGHETTI_GENERATED_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/filesystem.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/version.h
  )

set(LIBSPAGHETTI_SOURCES
  ${LIBSPAGHETTI_PUBLIC_HEADERS}
  include/spaghetti/filesystem.h.in
  include/spaghetti/version.h.in

  source/elements/gates/and.cc
  source/elements/gates/nand.cc
  source/elements/gates/nor.cc
  source/elements/gates/not.cc
  source/elements/gates/or.cc

  source/elements/logic/assign_float.cc
  source/elements/logic/assign_int.cc
  source/elements/logic/blinker.cc
  source/elements/logic/counter_down.cc
  source/elements/logic/counter_up.cc
  source/elements/logic/counter_up_down.cc
  source/elements/logic/demultiplexer_int.cc
  source/elements/logic/if_equal.cc
  source/elements/logic/if_greater.cc
  source/elements/logic/if_greater_equal.cc
  source/elements/logic/if_lower.cc
  source/elements/logic/if_lower_equal.cc
  source/elements/logic/latch.cc
  source/elements/logic/memory_difference.cc
  source/elements/logic/memory_reset_set.cc
  source/elements/logic/memory_set_reset.cc
  source/elements/logic/multiplexer_int.cc
  source/elements/logic/pid.cc
  source/elements/logic/snapshot_float.cc
  source/elements/logic/snapshot_int.cc
  source/elements/logic/switch.cc
  source/elements/logic/trigger_falling.cc
  source/elements/logic/trigger_rising.cc

  source/elements/math/abs.cc
  source/elements/math/add.cc
  source/elements/math/add_if.cc
  source/elements/math/bcd.cc
  source/elements/math/cos.cc
  source/elements/math/divide.cc
  source/elements/math/divide_if.cc
  source/elements/math/lerp.cc
  source/elements/math/multiply.cc
  source/elements/math/multiply_if.cc
  source/elements/math/sign.cc
  source/elements/math/sin.cc
  source/elements/math/square_root.cc
  source/elements/math/subtract.cc
  source/elements/math/subtract_if.cc

  source/elements/pneumatic/tank.cc
  source/elements/pneumatic/valve.cc

  source/elements/timers/clock.cc
  source/elements/timers/delta_time.cc
  source/elements/timers/t_off.cc
  source/elements/timers/t_on.cc
  source/elements/timers/t_pulse.cc

  source/elements/ui/bcd_to_seven_segment_display.cc
  source/elements/ui/float_info.cc
  source/elements/ui/int_info.cc
  source/elements/ui/push_button.cc
  source/elements/ui/toggle_button.cc
  source/elements/ui/seven_segment_display.cc

  source/elements/values/characteristic_curve.cc
  source/elements/values/clamp_float.cc
  source/elements/values/clamp_int.cc
  source/elements/values/const_bool.cc
  source/elements/values/const_float.cc
  source/elements/values/const_int.cc
  source/elements/values/degree_to_radian.cc
  source/elements/values/float_to_int.cc
  source/elements/values/int_to_float.cc
  source/elements/values/max_float.cc
  source/elements/values/max_int.cc
  source/elements/values/min_float.cc
  source/elements/values/min_int.cc
  source/elements/values/radian_to_degree.cc
  source/elements/values/random_bool.cc
  source/elements/values/random_float.cc
  source/elements/values/random_float_if.cc
  source/elements/values/random_int.cc
  source/elements/values/random_int_if.cc

  source/element.cc
  source/logger.cc
  source/package.cc
  source/registry.cc
  source/shared_library.cc
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
  PRIVATE source
  )
target_include_directories(Spaghetti SYSTEM PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/include
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/spdlog/include>
  )
target_link_libraries(Spaghetti
  PRIVATE ${CMAKE_DL_LIBS} ${CXX_FILESYSTEM_LIBS}
)
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
install(FILES ${LIBSPAGHETTI_PUBLIC_GATES_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/gates)
install(FILES ${LIBSPAGHETTI_PUBLIC_LOGIC_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/logic)
install(FILES ${LIBSPAGHETTI_PUBLIC_MATH_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/math)
install(FILES ${LIBSPAGHETTI_PUBLIC_PNEUMATIC_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/pneumatic)
install(FILES ${LIBSPAGHETTI_PUBLIC_TIMERS_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/timers)
install(FILES ${LIBSPAGHETTI_PUBLIC_UI_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/ui)
install(FILES ${LIBSPAGHETTI_PUBLIC_VALUES_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements/values)
install(FILES ${LIBSPAGHETTI_PUBLIC_ELEMENTS_ALL_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/elements)
install(FILES ${LIBSPAGHETTI_PUBLIC_COMMON_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/version.h
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/filesystem.h
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
foreach(HEADER ${VENDOR_SPARSEPP_HEADERS})
  configure_file(${VENDOR_ROOT}/sparsepp/${HEADER} ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/vendor/sparsepp/${HEADER} COPYONLY)
endforeach ()
install(DIRECTORY ${VENDOR_ROOT}/sparsepp
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/vendor)
foreach(HEADER ${VENDOR_SPDLOG_HEADERS})
  configure_file(${VENDOR_ROOT}/spdlog/include/spdlog/${HEADER} ${CMAKE_CURRENT_BINARY_DIR}/include/spaghetti/vendor/spdlog/${HEADER} COPYONLY)
endforeach ()
install(DIRECTORY ${VENDOR_ROOT}/spdlog/include/spdlog
  COMPONENT SDK
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spaghetti/vendor)
