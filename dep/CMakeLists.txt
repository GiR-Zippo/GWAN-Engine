if( CMAKE_COMPILER_IS_GNUCXX )
  add_definitions(--no-warnings)
elseif( MSVC )
  add_definitions(/W0)
endif()

add_subdirectory(angelscript)
 
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  add_subdirectory(libjpeg)  
  if(SERVERS AND NOT NOJEM)
    add_subdirectory(jemalloc)
  endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  add_subdirectory(win) 
  add_subdirectory(acelite)
  add_subdirectory(libpng)
  add_subdirectory(zlib)
endif()

add_subdirectory(alure)
add_subdirectory(freetype)