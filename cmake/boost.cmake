set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATICRUNTIME OFF)

# Old CMake sucks! Doesn't auto-detect Boost components' dependencies
# We only use Boost::log with PIE
find_package(Boost 1.55.0
  COMPONENTS
  log_setup
  log
  date_time
  system
  filesystem
  thread
  regex
  chrono
  REQUIRED
  )

set(BOOST_COMPILER_FLAGS_LIST)
