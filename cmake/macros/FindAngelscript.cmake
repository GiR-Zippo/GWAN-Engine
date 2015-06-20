#
# Find the AngelScript client includes and library
#

set( ANGELSCRIPT_FOUND 0 )

#if ( UNIX )
  if (NOT ANGELSCRIPT_INCLUDE_DIR)
    FIND_PATH( ANGELSCRIPT_INCLUDE_DIR
      NAMES
        angelscript.h
      HINTS
        "${ANGELSCRIPT_ROOT_DIR}"
      PATH_SUFFIXES
        angelscript
        angelscript/include
      PATHS
        $ENV{ANGELSCRIPT_ROOT}
        $ENV{ANGELSCRIPT_ROOT}/include
        ${CMAKE_SOURCE_DIR}/dep/angelscript
        ${CMAKE_SOURCE_DIR}/dep/angelscript/include
    DOC
      "Specify include-directories that might contain angelscript.h here."
    )
  endif()

  if (NOT ANGELSCRIPT_LIBRARY)
    FIND_LIBRARY( ANGELSCRIPT_LIBRARY
      NAMES
        angelscript Angelscript
      PATHS
        $ENV{ANGELSCRIPT_ROOT}/lib
        $ENV{ANGELSCRIPT_ROOT}
        ${CMAKE_SOURCE_DIR}/dep/angelscript/lib
      DOC "Specify library-locations that might contain the ANGELSCRIPT library here."
    )

  endif()

  if ( ANGELSCRIPT_LIBRARY )
    if ( ANGELSCRIPT_INCLUDE_DIR )
      message( STATUS "Found ANGELSCRIPT library: ${ANGELSCRIPT_LIBRARY}")
      message( STATUS "Found ANGELSCRIPT headers: ${ANGELSCRIPT_INCLUDE_DIR}")
    else ( ANGELSCRIPT_INCLUDE_DIR )
      message(FATAL_ERROR "Could not find ANGELSCRIPT headers! Please install ANGELSCRIPT libraries and headers")
    endif ( ANGELSCRIPT_INCLUDE_DIR )
  endif ( ANGELSCRIPT_LIBRARY )

  mark_as_advanced( ANGELSCRIPT_FOUND ANGELSCRIPT_LIBRARY ANGELSCRIPT_EXTRA_LIBRARIES ANGELSCRIPT_INCLUDE_DIR )
#endif (UNIX)
