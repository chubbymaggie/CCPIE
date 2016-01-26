cmake_minimum_required(VERSION 2.8)

project(pie CXX)



list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(compilers)
include(warnings)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  include(debug)
else()
  include(release)
endif()

set(CMAKE_CXX_FLAGS_LIST
  ${COMPILER_SPECIFIC_FLAGS_LIST}
  ${WARNING_FLAGS_LIST}
  ${BUILD_FLAGS_LIST}
)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_LIST}")



set(CMAKE_VERBOSE_MAKEFILE OFF)
set(Boost_USE_STATIC_LIBS ON)

# Old CMake sucks! Doesn't auto-detect Boost components' dependencies
# We only use Boost::log with PIE
find_package(Boost 1.55.0 COMPONENTS
                   log log_setup thread filesystem date_time system REQUIRED)



add_definitions(-DPIE_TRACE_LOGGING)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(pie PUBLIC include)

add_executable(pie
  src/pie/Log.cpp
  src/pie.cpp
)
target_link_libraries(pie
  ${Boost_LIBRARIES}
  rt                            # But why?? GCC <--> Boost linking bug?
)
