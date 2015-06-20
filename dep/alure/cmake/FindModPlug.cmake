# - Find ModPlug
# Find the native ModPlug includes and library
#
# MODPLUG_INCLUDE_DIR - where to find dumb.h
# MODPLUG_LIBRARIES - List of libraries when using dumb
# MODPLUG_FOUND - True if dumb found.

set( MODPLUG_FOUND 0 )

if (WIN32)
    SET(MODPLUG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/win/libmodplug/src)
    SET(MODPLUG_LIBRARY ${CMAKE_SOURCE_DIR}/dep/win/libmodplug/lib/${CMAKE_BUILD_TYPE}/modplug.lib)
    message( STATUS "Found MODPLUG library: ${MODPLUG_LIBRARY}")
    message( STATUS "Found MODPLUG headers: ${MODPLUG_INCLUDE_DIR}")
    set( MODPLUG_FOUND 1 )
elseif (NOT WIN32)
  if (NOT MODPLUG_INCLUDE_DIR)
    FIND_PATH( MODPLUG_INCLUDE_DIR
      NAMES
        libmodplug/modplug.h
      PATHS
        /usr/include
        /usr/include/modplug
        /usr/local/include
        /usr/local/include/modplug
        ${MODPLUG_DIR}
        $ENV{MODPLUG_ROOT}
        $ENV{MODPLUG_ROOT}/include
    DOC
      "Specify include-directories that might contain angelscript.h here."
    )
  endif()

  if (NOT MODPLUG_LIBRARY)
    FIND_LIBRARY( MODPLUG_LIBRARY
      NAMES
        modplug
        modplug.lib
      PATH_SUFFIXES
        lib
      PATHS
        ${MODPLUG_DIR}
        ${CMAKE_INSTALL_PREFIX}/lib
        $ENV{MODPLUG_ROOT}/lib
        $ENV{MODPLUG_ROOT}
        ${CMAKE_SOURCE_DIR}/dep/win/libmodplug/lib/modplug.so
      DOC "Specify library-locations that might contain the MODPLUG library here."
      
    )
  endif()

  if ( MODPLUG_LIBRARY )
    if ( MODPLUG_INCLUDE_DIR )
      message( STATUS "Found MODPLUG library: ${MODPLUG_LIBRARY}")
      message( STATUS "Found MODPLUG headers: ${MODPLUG_INCLUDE_DIR}")
      set( MODPLUG_FOUND 1 )
    else ( MODPLUG_INCLUDE_DIR )
      message(FATAL_ERROR "Could not find MODPLUG headers! Please install MODPLUG libraries and headers")
    endif ( MODPLUG_INCLUDE_DIR )
  endif ( MODPLUG_LIBRARY )

endif (WIN32)
