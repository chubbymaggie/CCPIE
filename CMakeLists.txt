cmake_minimum_required(VERSION 2.8)

project(pie CXX)



list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(compilers)                              # compiler-specific flags
include(warnings)                               # emabled warning and errors
include(configure)                              # build configurations
include(boost)                                  # Boost libs and compiler flags
include(targets)                                # custom targets

set(CMAKE_CXX_FLAGS_LIST
  ${COMPILER_SPECIFIC_FLAGS_LIST}
  ${WARNING_FLAGS_LIST}
  ${BUILD_FLAGS_LIST}
  ${BOOST_COMPILER_FLAGS_LIST}
)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_LIST}")



include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(pie PUBLIC include)

add_executable(pie
  src/pie/Log.cpp
  src/pie.cpp
)
target_link_libraries(pie
  ${Boost_LIBRARIES}
  rt                                            # Why? I don't need this on my PC
)
