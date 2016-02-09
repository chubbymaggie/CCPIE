set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithLog" CACHE STRING "" FORCE)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  include(debug)

elseif(CMAKE_BUILD_TYPE MATCHES Release)
  include(release)

elseif(CMAKE_BUILD_TYPE MATCHES RelWithLog)
  include(rel-with-log)

else()                                                   # default to RelWithLog
  message("** Unknown BUILD_TYPE. Defaulting to RelWithLog. **")
  include(rel-with-log)
endif()
