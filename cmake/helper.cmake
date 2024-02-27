cmake_minimum_required(VERSION 3.15)
if(${CMAKE_VERSION} VERSION_LESS 3.28)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
  cmake_policy(VERSION 2.8)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/macro.cmake)

function(add_cli_exe T)
  add_executable(${T} ${ARGN})
  # target_link_libraries(${T} PUBLIC CLI11)
  set_property(TARGET ${T} PROPERTY FOLDER "Examples")
  if(CLI11_FORCE_LIBCXX)
    set_property(
      TARGET ${T}
      APPEND_STRING
      PROPERTY LINK_FLAGS -stdlib=libc++)
  endif()
endfunction()

include(CMakeDependentOption)
include(GNUInstallDirs)

if(NOT CMAKE_VERSION VERSION_LESS 3.11)
  include(FetchContent)
endif()

set(CMAKE_CXX_STANDARD 11)

if(NOT DEFINED CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

