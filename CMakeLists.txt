# Set projectname (must be done AFTER setting configurationtypes)
project(GWAN-Client)

# CMake policies (can not be handled elsewhere)
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0005 OLD)

# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set macro-directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

# build in Release-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)
include(CheckFileOffsetBits)

# set default buildoptions and print them
include(cmake/options.cmake)

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if( NOPCH )
  set(USE_COREPCH 0)
  set(USE_SCRIPTPCH 0)
endif()

include(CheckPlatform)

# basic packagesearching and setup (further support will be needed, this is a preliminary release!)
set(OPENSSL_EXPECTED_VERSION 1.0.0)
set(ACE_EXPECTED_VERSION 5.8.3)

find_package(OpenSSL REQUIRED)
find_package(PCHSupport)
find_package(ACE REQUIRED)

#########################################################
# FIND OPENGL
#########################################################
message( STATUS "Looking for OpenGL...")
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
else()
    message( STATUS "Found OpenGL ;)")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND Glut or use internal
#########################################################
message( STATUS "Looking for GLUT...")
find_package(GLUT)
if(NOT GLUT_FOUND)
    if( WIN32 )
        message( STATUS "Using internal GLUT...")
        SET(GLUT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/win/freeglut/include)
        SET(GLUT_LIBRARY ${CMAKE_SOURCE_DIR}/dep/win/freeglut/lib/freeglut.lib)
     else()
        message(ERROR " GLUT not found!")
     endif()
else()
    message( STATUS "Found GLUT ;)")
endif(NOT GLUT_FOUND)

#########################################################
# FIND GLEW or use internal
#########################################################
message( STATUS "Looking for GLEW...")
find_package(GLEW)
if(NOT GLEW_FOUND)
    if( WIN32 )
        message( STATUS "Using internal GLEW...")
        SET(GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/win/glew-1.10.0/include)
        SET(GLEW_LIBRARY ${CMAKE_SOURCE_DIR}/dep/win/glew-1.10.0/lib/Release/Win32/glew32.lib)
        SET(GLEW_FOUND 1)
     else()
        message(ERROR " GLEW not found!")
     endif()
else()
    message( STATUS "Found GLEW ;)")
endif(NOT GLEW_FOUND)

#########################################################
# FIND OPENAL
#########################################################
message( STATUS "Looking for OPENAL...")
find_package(OpenAL)
if(NOT OPENAL_FOUND)
    if( WIN32 )
        message( STATUS "Using internal OpenAL...")
        SET(OPENAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/win/openAL/include/AL)
        SET(OPENAL_LIBRARY ${CMAKE_SOURCE_DIR}/dep/win/openAL/OpenAL32.lib)
     else(NOT WIN32)
        message(ERROR " OpenAL not found!")
     endif(WIN32 )
else()
    message( STATUS "Found OpenAL ;)")
endif(NOT OPENAL_FOUND)

find_package(Threads REQUIRED)
#find_package(Angelscript)

if( UNIX )
  find_package(Readline)
  find_package(ZLIB)
  find_package(BZip2)
endif()

if(NOT WITHOUT_GIT)
  find_package(Git)
endif()

# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)

# print out the results before continuing
include(cmake/showoptions.cmake)

# add dependencies
add_subdirectory(dep)

# add core sources
add_subdirectory(src)
