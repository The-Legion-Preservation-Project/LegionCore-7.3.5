# Copyright (C) 2008-2025 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#

FUNCTION(LC_SUBDIRLIST result curdir recursive includeRoot)
  # glob recurse seem's doesn't work
  FILE(GLOB children RELATIVE ${curdir} "${curdir}/[^\\.]*")
  if (${includeRoot})
    SET(dirlist "${curdir}")
  else()
    SET(dirlist "")
  endif()

  FOREACH(child ${children})
    IF(IS_DIRECTORY "${curdir}/${child}")
        if (${recursive})
          LC_SUBDIRLIST(sub_Dirs "${curdir}/${child}" TRUE FALSE)
          SET(dirlist "${curdir}/${child}" ${sub_Dirs} ${dirlist})
        else()
          SET(dirlist "${curdir}/${child}" ${dirlist})
        endif()
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist} PARENT_SCOPE)
ENDFUNCTION(LC_SUBDIRLIST result curdir recursive)

#
# LC_SET_GLOBAL
#
MACRO(LC_SET_GLOBAL name val)
  set_property ( GLOBAL PROPERTY ${name} ${val})
  # after set , create the variable for current scope
  LC_GET_GLOBAL(${name})
ENDMACRO()

MACRO(LC_ADD_GLOBAL name val)
  LC_GET_GLOBAL(${name})

  set_property ( GLOBAL PROPERTY ${name}
      ${${name}}
      ${val}
  )
  # after set , create the variable for current scope
  LC_GET_GLOBAL(${name})
ENDMACRO()

#
# LC_GET_GLOBAL
#
MACRO(LC_GET_GLOBAL name)
  get_property(${name} GLOBAL PROPERTY ${name})
ENDMACRO()

#
# LC_SET_CACHE
#
MACRO(LC_SET_CACHE name val)
  set(${name} ${val} CACHE INTERNAL "FC Var")
ENDMACRO()

#
# LC_LIST_ADD_CACHE
#
MACRO(LC_LIST_ADD_CACHE name val)

    # avoid duplicates
    if (";${${name}};" MATCHES ";${val};")
      # nothing to do for now
    else()
        set(${name} ${val} ${${name}} CACHE INTERNAL "FC Var")
    endif()
ENDMACRO()


#
# LC_SET_PATH
#
MACRO(LC_SET_PATH name val)
  LC_SET_CACHE(${name} ${val})

  LC_ADD_INC_PATH(${val})
ENDMACRO()

#
# LC_ADD_INC_PATH
#
MACRO(LC_ADD_INC_PATH val)

    if (";${LC_INC_PATHS};" MATCHES ";${val};")
      # nothing to do for now
    else()
        set(LC_INC_PATHS
            ${LC_INC_PATHS}
            ${val}
        )

        #update cache
        LC_SET_CACHE("LC_INC_PATHS" "${LC_INC_PATHS}")
        include_directories(${val})
    endif()
ENDMACRO()


#
# LC_LOAD_INC_PATHS
#
MACRO(LC_LOAD_INC_PATHS)
  include_directories(${LC_INC_PATHS})
ENDMACRO()

#
# LC_SET_PARENT
#
MACRO(LC_SET_PARENT name val)
  set(${name} ${val} PARENT_SCOPE)
ENDMACRO()


MACRO(LC_ADD_HOOK hook_name value)
  LC_ADD_GLOBAL(${hook_name} "${value}")
ENDMACRO()

MACRO(LC_RUN_HOOK hook_name)
  LC_GET_GLOBAL(${hook_name})
  message(STATUS "Running cmake hook: ${hook_name}")
  if (${hook_name})
      set(HOOK_ARRAY ${${hook_name}})
      FOREACH (hook_file ${HOOK_ARRAY})
          message("Including ${hook_file}")
          include("${hook_file}")
      ENDFOREACH()
  else()
      message(STATUS "No hooks registered for ${hook_name}")
  endif()
ENDMACRO()