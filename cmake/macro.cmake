cmake_minimum_required(VERSION 3.15)
# define the macro
macro(source_group_by_dir source_files)
  set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR}/)

  foreach(sgbd_file ${${source_files}})
    string(REGEX REPLACE ${sgbd_cur_dir} "" sgbd_fpath ${sgbd_file})
    string(REGEX REPLACE "[^/\\\\]+$" "" sgbd_group_name ${sgbd_fpath})

    if(sgbd_group_name)
      string(REPLACE "/" "\\\\" sgbd_group_name_normal ${sgbd_group_name})

      # message(STATUS ${sgbd_group_name_normal})
      source_group(${sgbd_group_name_normal} FILES ${sgbd_file})
    endif(sgbd_group_name)
  endforeach(sgbd_file)
endmacro(source_group_by_dir)

MACRO(group_by_folder sources basedir)
  SET(SOURCE_GROUP_DELIMITER "/")
  SET(last_dir "")
  SET(files "")
  FOREACH(file ${${sources}})
    file(RELATIVE_PATH relative_file "${basedir}" ${file})
    GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
    IF (NOT "${dir}" STREQUAL "${last_dir}")
      IF (files)
        SOURCE_GROUP("${last_dir}" FILES ${files})
      ENDIF (files)
      SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
  ENDFOREACH(file)
  IF (files)
    SOURCE_GROUP("${last_dir}" FILES ${files})
  ENDIF (files)
ENDMACRO(group_by_folder)


MACRO(copy_debug_dllv2 pjname from  to patten)
    FILE(GLOB copy_file_list RELATIVE ${from} ${from}/${patten})
    FOREACH(copy_file_name ${copy_file_list})
    MESSAGE(STATUS "post_build : will copy debug resources : ${copy_file_name} from ${from}")
        add_custom_command(TARGET ${pjname} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "copy ${copy_file_name} to ${to}/${copy_file_name} ..."
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${from}/${copy_file_name} "${to}/${copy_file_name}"
        )
    ENDFOREACH()
ENDMACRO()

MACRO(copy_debug_dll from  to patten)
    FILE(GLOB copy_file_list RELATIVE ${from} ${from}/${patten})
    FOREACH(copy_file_name ${copy_file_list})
    MESSAGE(STATUS "post_build : will copy debug resources : ${copy_file_name} from ${from}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "copy ${copy_file_name} to ${to}/${copy_file_name} ..."
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${from}/${copy_file_name} "${to}/${copy_file_name}"
        )
    ENDFOREACH()
ENDMACRO()

MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)
#ADD_MSVC_PRECOMPILED_HEADER("precompiled.h" "precompiled.cpp" MySources)
#ADD_LIBRARY(MyLibrary ${MySources})
