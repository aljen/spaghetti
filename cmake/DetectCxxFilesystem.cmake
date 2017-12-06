set(HAVE_CXX_FILESYSTEM 0)
set(HAVE_CXX_EXPERIMENTAL_FILESYSTEM 0)
set(HAVE_BOOST_FILESYSTEM 0)
set(CXX_FILESYSTEM_LIBS)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/CMakeTemp/test_cxx_filesystem.cc
"#include <filesystem>\n"
"namespace fs = std::filesystem;\n"
"int main() {\n"
"  fs::path current{ \".\" };\n"
"  return 0;\n"
"}\n"
)
message(STATUS "Detecting CXX std::filesystem support")
try_compile(HAVE_CXX_FILESYSTEM
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeTemp/HaveCxxFilesystem
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeTemp/test_cxx_filesystem.cc
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED 17
  OUTPUT_VARIABLE OUTPUT
)
if (HAVE_CXX_FILESYSTEM)
  message(STATUS "Detecting CXX std::filesystem support -- found")
else ()
  message(STATUS "Detecting CXX std::filesystem support -- NOT FOUND")
endif ()

if (NOT HAVE_CXX_FILESYSTEM)
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/CMakeTemp/test_cxx_experimental_filesystem.cc
  "#include <experimental/filesystem>\n"
  "namespace fs = std::experimental::filesystem;\n"
  "int main() {\n"
  "  fs::path current{ \".\" };\n"
  "  return 0;\n"
  "}\n"
  )
  message(STATUS "Detecting CXX std::experimental::filesystem support")
  if (NOT MSVC)
    set(TEMP_FILESYSTEM_LIBS stdc++fs)
  endif ()
  try_compile(HAVE_CXX_EXPERIMENTAL_FILESYSTEM
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeTemp/HaveCxxExperimentalFilesystem
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeTemp/test_cxx_experimental_filesystem.cc
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED 17
    LINK_LIBRARIES ${TEMP_FILESYSTEM_LIBS}
    OUTPUT_VARIABLE OUTPUT
  )
  if (HAVE_CXX_EXPERIMENTAL_FILESYSTEM)
    set(CXX_FILESYSTEM_LIBS ${TEMP_FILESYSTEM_LIBS})
    message(STATUS "Detecting CXX std::experimental::filesystem support -- found")
  else ()
    message(STATUS "Detecting CXX std::experimental::filesystem support -- NOT FOUND")
  endif ()
endif ()

if (NOT HAVE_CXX_EXPERIMENTAL_FILESYSTEM)
  message(STATUS "Detecting CXX boost::filesystem support")
  find_package(Boost QUIET COMPONENTS filesystem system)
  if (Boost_FOUND STREQUAL "1")
    set(HAVE_BOOST_FILESYSTEM 1)
    set(CXX_FILESYSTEM_LIBS Boost::filesystem Boost::system)
    message(STATUS "Detecting CXX boost::filesystem support -- found")
  else ()
    message(STATUS "Detecting CXX boost::filesystem support -- NOT FOUND")
  endif ()
endif ()
