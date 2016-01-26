if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
    message(FATAL_ERROR "Unsupported GCC version. GCC >= 4.9 required.")

  elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
    list(APPEND COMPILER_SPECIFIC_FLAGS_LIST "-std=c++14")

  else()
    list(APPEND COMPILER_SPECIFIC_FLAGS_LIST "-std=c++1z")
  endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.7")
    message(FATAL_ERROR "Unsupported Clang version. Clang >= 3.7 required.")

  else()
    list(APPEND COMPILER_SPECIFIC_FLAGS_LIST "-std=c++1z")
  endif()
endif()
