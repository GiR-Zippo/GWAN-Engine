set( ALUT_FOUND 0 )
if ( UNIX )
if (NOT ALUT_INCLUDE_DIR)
    FIND_PATH( ALUT_INCLUDE_DIR
      NAMES
        alut.h
      HINTS
        "${ALUT_ROOT_DIR}"
      PATH_SUFFIXES
        AL
        alut
        OpenAL
        include
        include/alut
        include/freealut
        include/AL
        include/OpenAL
      PATHS
        /usr/local
        /opt/local
        /sw
        $ENV{ALUT_ROOT}
        $ENV{ALUT_ROOT}/ALUT
        $ENV{ALUT_ROOT}/include
        ${CMAKE_SOURCE_DIR}/dep/freeAlut
    DOC
      "Specify include-directories that might contain ALUT.h here."
    )
endif()

if (NOT ALUT_LIBRARY)
    FIND_LIBRARY( ALUT_LIBRARY
      NAMES
        NAMES
        alut
      HINTS
        "${ALUT_ROOT_DIR}"
      PATH_SUFFIXES
        lib
        lib64
      PATHS
        /usr/local
        /opt/local
        /sw
        $ENV{ALUT_ROOT}/lib
        $ENV{ALUT_ROOT}
      DOC "Specify library-locations that might contain the ALUT library here."
    )
endif()

if ( ALUT_LIBRARY )
      message( STATUS "Found ALUT library: ${ALUT_LIBRARY}")
      message( STATUS "Found ALUT headers: ${ALUT_INCLUDE_DIR}")
endif ( ALUT_LIBRARY )
mark_as_advanced( ALUT_FOUND ALUT_LIBRARY ALUT_INCLUDE_DIR )
endif (UNIX)