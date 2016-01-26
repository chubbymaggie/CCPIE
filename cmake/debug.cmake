set(CMAKE_CXX_FLAGS_DEBUG)      # Remove default debug flags
set(BUILD_FLAGS_LIST
  " -O0"                        # No optimizations
  " -g"                         # Debug symbols
)



add_definitions(-DPIE_TRACE_LOGGING)
