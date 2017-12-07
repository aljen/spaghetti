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
include(DetectCxxFilesystem)
include(GetRepoInfo)

option(SPAGHETTI_BUILD_EDITOR "Build editor" ON)
option(SPAGHETTI_BUILD_EXAMPLE_PLUGIN "Build example plugin" ON)
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

add_subdirectory(libspaghetti)

if (SPAGHETTI_BUILD_EDITOR)
  add_subdirectory(editor)
endif ()

if (SPAGHETTI_BUILD_EXAMPLE_PLUGIN)
  add_subdirectory(plugins)
endif ()
