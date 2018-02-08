cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(Spaghetti
  VERSION 17.12.12
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
include(VendorHeaders)

option(SPAGHETTI_BUILD_EDITOR "Build editor" ON)
option(SPAGHETTI_BUILD_EXAMPLE_PLUGIN "Build example plugin" ON)
option(SPAGHETTI_ENABLE_CPACK "Enable CPack" OFF)
option(SPAGHETTI_ENABLE_ALL_WARNINGS "Enable all warnings" OFF)
option(SPAGHETTI_TREAT_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(SPAGHETTI_USE_OPENGL "Use OpenGL in QGraphicsView" OFF)
option(BUILD_SHARED_LIBS "Build shared libs" ON)
if (GCC OR CLANG)
  option(SPAGHETTI_BUILD_NATIVE "Build native" OFF)
  option(SPAGHETTI_USE_CLANG_TIDY "Use clang-tidy" OFF)
  option(SPAGHETTI_USE_SANITIZERS "Use sanitizers" OFF)
endif ()

if (SPAGHETTI_USE_CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-cppcoreguidelines-pro-type-const-cast")
endif ()

set(VENDOR_ROOT ${PROJECT_SOURCE_DIR}/vendor)

add_subdirectory(libspaghetti)

if (SPAGHETTI_BUILD_EDITOR)
  add_subdirectory(editor)
endif ()

if (SPAGHETTI_BUILD_EXAMPLE_PLUGIN)
  add_subdirectory(plugins)
endif ()

if (SPAGHETTI_ENABLE_CPACK)
  include(InstallRequiredSystemLibraries)
#  set(CPACK_GENERATOR TBZ2)
  list(APPEND CPACK_GENERATOR IFW)
  if (WIN32)
    list(APPEND CPACK_GENERATOR NSIS64)
  endif ()
  set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
  set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
  set(CPACK_PACKAGE_DESCRIPTION "Graphical logic editor/simulator")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Graphical logic editor/simulator")
  set(CPACK_PACKAGE_VENDOR "Artur Wyszyński")
  set(CPACK_PACKAGE_CONTACT "spaghetti@hitomi.pl")
  set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
  set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

  set(CPACK_IFW_PRODUCT_URL "https://github.com/aljen/spaghetti")
  set(CPACK_IFW_PACKAGE_ALLOW_SPACE_IN_PATH OFF)

  include(CPack)
  include(CPackIFW)

  cpack_add_component(SDK
    DISPLAY_NAME "Spaghetti SDK"
    REQUIRED
  )
  cpack_add_component(Editor
    DISPLAY_NAME "Editor"
    )
  cpack_add_component(ExamplePlugin
    DISPLAY_NAME "Example plugin"
    )
endif ()
