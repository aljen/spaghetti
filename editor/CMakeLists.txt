cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(SpaghettiEditor VERSION ${Spaghetti_VERSION} LANGUAGES C CXX)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(SpaghettiEditor main.cc)
target_compile_definitions(SpaghettiEditor
  PRIVATE ${SPAGHETTI_DEFINITIONS}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_DEFINITIONS_DEBUG}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_DEFINITIONS_RELEASE}>
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
target_link_libraries(SpaghettiEditor Spaghetti)
