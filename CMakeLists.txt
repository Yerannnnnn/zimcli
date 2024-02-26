cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME zimcli)
project(${PROJECT_NAME} LANGUAGES CXX)
include(cmake/helper.cmake)
set(EXECUTABLE_OUTPUT_PATH         ${CMAKE_HOME_DIRECTORY}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)
set(CMAKE_PDB_OUTPUT_DIRECTORY     ${CMAKE_HOME_DIRECTORY}/build)
set(CMAKE_INSTALL_PREFIX           ${CMAKE_HOME_DIRECTORY}/build/install CACHE PATH ".."  FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS        ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_HOME_DIRECTORY}/build/install")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_HOME_DIRECTORY}/lib")
####################################################
file(GLOB_RECURSE All_SOURCES 
  ${CMAKE_CURRENT_LIST_DIR}/src/*.mm
  ${CMAKE_CURRENT_LIST_DIR}/src/*.m
  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/*.c
  ${CMAKE_CURRENT_LIST_DIR}/src/*.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/*.hh
  ${CMAKE_CURRENT_LIST_DIR}/src/*.h
  ${CMAKE_CURRENT_LIST_DIR}/src/*.hpp
)
source_group_by_dir(All_SOURCES)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src/common)


add_executable(${PROJECT_NAME} ${All_SOURCES})
####################################################


target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
add_compile_options(-Wno-unused-variable)
add_definitions(-Wno-unused-parameter)
add_definitions(-Wno-shorten-64-to-32)

####################################################
if(APPLE)
  target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/lib/zim/mac/ZIM.xcframework/macos-arm64_x86_64/Headers
    ${CMAKE_CURRENT_LIST_DIR}/lib/zim/mac/ZIM.xcframework/macos-arm64_x86_64/Headers/internal/include
  )
  # find_library(
  #   VarZegoZIM
  #       ZIM
  #       ${CMAKE_CURRENT_LIST_DIR}/lib/zim/mac/ZIM.xcframework/macos-arm64_x86_64/
  #   NO_DEFAULT_PATH
  # )
  # message("**find_library zim : ${VarZegoZIM}")
  
  target_link_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/lib/zim/mac/ZIM.xcframework/macos-arm64_x86_64
  )
  target_link_libraries(${PROJECT_NAME} libZIM.dylib)

  target_compile_options(${PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header -Wno-comma>)
####################################################
else()
  target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/lib/zim/linux/include
  )  
  target_link_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/lib/zim/linux/x86_64
  )
  # find_library(
  #   VarZegoZIM
  #       ZIM
  #       ${CMAKE_CURRENT_LIST_DIR}/lib/zim/linux/x86_64
  #   NO_DEFAULT_PATH
  # )
  # message("**find_library zim : ${VarZegoZIM}")
  target_link_libraries(${PROJECT_NAME} libZIM.so)
endif()


####################################################