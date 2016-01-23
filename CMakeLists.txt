cmake_minimum_required(VERSION 2.8)


project(pie CXX)

set(CMAKE_VERBOSE_MAKEFILE OFF)


set(Boost_USE_STATIC_LIBS ON)

# Old CMake sucks! Doesn't auto-detect Boost components' dependencies
# We only use Boost::log with PIE
find_package(Boost 1.55.0 COMPONENTS
                   log log_setup thread filesystem date_time system REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-unused-local-typedef -std=c++1z")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")


add_definitions(-DPIE_TRACE_LOGGING)


include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(pie PUBLIC include)


add_executable(pie
  src/pie/Log.cpp
  src/pie.cpp
)


target_link_libraries(pie ${Boost_LIBRARIES} rt)
