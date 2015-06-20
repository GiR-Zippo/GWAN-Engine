# Package overloads - Linux
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  if (NOT NOJEM)
    set(JEMALLOC_LIBRARY "jemalloc")
    message(STATUS "UNIX: Using jemalloc")
  endif()
endif()

set(ANGELSCRIPT_INCLUDE_DIR 
${CMAKE_SOURCE_DIR}/dep/angelscript/include
${CMAKE_SOURCE_DIR}/dep/angelscript/lib
${CMAKE_SOURCE_DIR}/dep/angelscript/source)
set(ANGELSCRIPT_LIBRARY "${CMAKE_SOURCE_DIR}/dep/angelscript/lib/libAngelscript.a")

# set default configuration directory
if( NOT CONF_DIR )
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/etc)
  message(STATUS "UNIX: Using default configuration directory")
endif()

# set default library directory
if( NOT LIBSDIR )
  set(LIBSDIR ${CMAKE_INSTALL_PREFIX}/lib)
  message(STATUS "UNIX: Using default library directory")
endif()

# configure uninstaller
configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/platform/cmake_uninstall.in.cmake"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
)
message(STATUS "UNIX: Configuring uninstall target")

# create uninstaller target (allows for using "make uninstall")
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
)
message(STATUS "UNIX: Created uninstall target")

message(STATUS "UNIX: Detected compiler: ${CMAKE_C_COMPILER_ID}")
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/gcc/settings.cmake)
elseif(CMAKE_C_COMPILER_ID MATCHES "Intel")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/icc/settings.cmake)
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/clang/settings.cmake)
else()
add_definitions(-D_BUILD_DIRECTIVE='"${CMAKE_BUILD_TYPE}"')
endif()
