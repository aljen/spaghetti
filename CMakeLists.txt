cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(Spaghetti
  VERSION 17.12.04
  DESCRIPTION "Graphical logic editor/simulator"
  LANGUAGES C CXX
)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(DefaultOptions)
include(BuildDirectories)
include(BuildType)
include(CompilerType)
include(CompilerFlags)

option(SPAGHETTI_USE_CLANG_TIDY "Use clang-tidy" OFF)
option(SPAGHETTI_ENABLE_CPACK "Enable CPack" OFF)
option(SPAGHETTI_ENABLE_ALL_WARNINGS "Enable all warnings" OFF)
option(SPAGHETTI_TREAT_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(SPAGHETTI_USE_SANITIZERS "Use sanitizers" OFF)
option(BUILD_SHARED_LIBS "Build shared libs" ON)
if (GCC OR CLANG)
  option(SPAGHETTI_BUILD_NATIVE "Build native" OFF)
endif ()

if (SPAGHETTI_USE_CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-cppcoreguidelines-pro-type-const-cast")
endif ()

execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE Spaghetti_GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE Spaghetti_GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE Spaghetti_GIT_COMMIT_SHORT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_subdirectory(libspaghetti)
add_subdirectory(editor)
add_subdirectory(plugins)
