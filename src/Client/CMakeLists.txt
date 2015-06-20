########### client ###############

file(GLOB_RECURSE   sources_BackgroundDownloader BackgroundDownloader/*.cpp BackgroundDownloader/*.h BackgroundDownloader/*.hpp)
file(GLOB_RECURSE   sources_Downloading Downloading/*.cpp Downloading/*.h Downloading/*.hpp)
file(GLOB_RECURSE   sources_GameObjects GameObjects/*.cpp GameObjects/*.h GameObjects/*.hpp)
file(GLOB_RECURSE   sources_GameObjectsGFXType GameObjects/GraphicTypes/*.cpp GameObjects/GraphicTypes/*.hpp)
file(GLOB_RECURSE   sources_Launcher Launcher/*.cpp Launcher/*.hpp Launcher/*.h)
file(GLOB_RECURSE   sources_Misc Misc/*.cpp Misc/*.h Misc/*.hpp)
file(GLOB_RECURSE   sources_Map Map/*.cpp Map/*.h Map/*.hpp)
file(GLOB_RECURSE   sources_Objects Objects/*.cpp Objects/*.h Objects/*.hpp)
file(GLOB_RECURSE   sources_Objects_3D Objects/3DModels/*.cpp Objects/3DModels/*.hpp)
file(GLOB_RECURSE   sources_Objects_Loading Objects/LoadingObject/*.cpp Objects/LoadingObject/*.hpp)
file(GLOB_RECURSE   sources_Objects_Plane Objects/Plane/*.cpp Objects/Plane/*.hpp)
file(GLOB_RECURSE   sources_Objects_Sprite Objects/Sprite/*.cpp Objects/Sprite/*.hpp)
file(GLOB_RECURSE   sources_Objects_Text Objects/Text/*.cpp Objects/Text/*.hpp)
file(GLOB_RECURSE   sources_Objects_Tilemap Objects/Tilemap/*.cpp Objects/Tilemap/*.hpp)
file(GLOB_RECURSE   sources_ScriptSystem ScriptSystem/*.cpp ScriptSystem/*.h ScriptSystem/*.hpp)
file(GLOB_RECURSE   sources_TextureLoader TextureLoader/*.cpp TextureLoader/*.hpp)
file(GLOB           sources_localdir *.cpp *.h *.hpp)

source_group(Downloading                FILES ${sources_BackgroundDownloader})
source_group(Downloading                FILES ${sources_Downloading})
source_group(GameObjects                FILES ${sources_GameObjects})
source_group(Launcher                   FILES ${sources_Launcher})
source_group(Map                        FILES ${sources_Map})
source_group(Misc                       FILES ${sources_Misc})
source_group(Objects                    FILES ${sources_Objects})
source_group(ScriptSystem               FILES ${sources_ScriptSystem})
source_group(\\                         FILES ${sources_localdir})


if (USE_COREPCH)
  set(client_PCH_HDR PrecompiledHeaders/clientPCH.h)
  set(client_PCH_SRC PrecompiledHeaders/clientPCH.cpp)
endif()

set(client_SRCS
  ${client_SRCS}
  ${sources_BackgroundDownloader}
  ${sources_Downloading}
  ${sources_GameObjects}
  ${sources_Objects_3D}
  ${sources_Objects_Loading}
  ${sources_Objects_Plane}
  ${sources_Objects_Sprite}
  ${sources_Objects_Text}
  ${sources_Objects_Tilemap}
  ${sources_GameObjectsGFXType}
  ${sources_Launcher}
  ${sources_Map}
  ${sources_Misc}
  ${sources_Objects}
  ${sources_ScriptSystem}
  ${sources_TextureLoader}
  ${sources_localdir}
)

if( WIN32 )
  set(client_SRCS
    ${client_SRCS}
    ${sources_windows_Debugging}
  )
  if ( MSVC )
    set(client_SRCS
      ${client_SRCS}
      client.rc
    )
  endif ()
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/alure/include
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on/aatc
  ${CMAKE_SOURCE_DIR}/dep/angelscript/include
  ${CMAKE_SOURCE_DIR}/dep/angelscript/source
  ${CMAKE_SOURCE_DIR}/dep/freetype/include
  ${CMAKE_SOURCE_DIR}/dep/libjpeg
  ${CMAKE_SOURCE_DIR}/dep/libpng
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_SOURCE_DIR}/src/Shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/Shared/Cryptography
  ${CMAKE_SOURCE_DIR}/src/Shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/Shared/Logging
  ${CMAKE_SOURCE_DIR}/src/Shared/Network
  ${CMAKE_SOURCE_DIR}/src/Shared/Network/Protocol
  ${CMAKE_SOURCE_DIR}/src/Shared/OpcodeHandling
  ${CMAKE_SOURCE_DIR}/src/Shared/Packet
  ${CMAKE_SOURCE_DIR}/src/Shared/Threading
  ${CMAKE_SOURCE_DIR}/src/Shared/XML
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/BackgroundDownloader
  ${CMAKE_CURRENT_SOURCE_DIR}/Downloading
  ${CMAKE_CURRENT_SOURCE_DIR}/GameObjects
  ${CMAKE_CURRENT_SOURCE_DIR}/GameObjects/GraphicTypes
  ${CMAKE_CURRENT_SOURCE_DIR}/Launcher
  ${CMAKE_CURRENT_SOURCE_DIR}/Objects
  ${CMAKE_CURRENT_SOURCE_DIR}/Objects/3DModels
  ${CMAKE_CURRENT_SOURCE_DIR}/Objects/LoadingObject
  ${CMAKE_CURRENT_SOURCE_DIR}/Objects/Plane
  ${CMAKE_CURRENT_SOURCE_DIR}/Objects/Sprite
  ${CMAKE_CURRENT_SOURCE_DIR}/Objects/Text
  ${CMAKE_CURRENT_SOURCE_DIR}/Objects/Tilemap
  ${CMAKE_CURRENT_SOURCE_DIR}/Misc
  ${CMAKE_CURRENT_SOURCE_DIR}/Map
  ${CMAKE_CURRENT_SOURCE_DIR}/ScriptSystem
  ${CMAKE_CURRENT_SOURCE_DIR}/TextureLoader
  ${GLEW_INCLUDE_DIR}
  ${GLUT_INCLUDE_DIR}
  ${ACE_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${ANGELSCRIPT_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

add_executable(client
  ${client_SRCS}
  ${client_PCH_SRC}
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on/scriptbuilder/scriptbuilder.cpp
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on/scripthandle/scripthandle.cpp
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on/scriptarray/scriptarray.cpp
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on/scriptmath/scriptmath.cpp
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on/scriptmath/scriptmathcomplex.cpp
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on/scriptstdstring/scriptstdstring.cpp
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on/scriptstdstring/scriptstdstring_utils.cpp
  ${CMAKE_SOURCE_DIR}/dep/angelscript/add_on/weakref/weakref.cpp
)

add_dependencies(client revision.h)

#if( NOT WIN32 )
#  set_target_properties(client PROPERTIES
#    COMPILE_DEFINITIONS _TRINITY_REALM_CONFIG="${CONF_DIR}/client.conf"
#  )
#endif()

set(Client_LINK_FLAGS "")
if( UNIX )
    set(Client_LINK_FLAGS "-pthread ${Client_LINK_FLAGS}")
endif()
set_target_properties(client PROPERTIES LINK_FLAGS "${Client_LINK_FLAGS}")

target_link_libraries(client
  shared
  png
  Angelscript
  freetype
  alure
  ${CMAKE_THREAD_LIBS_INIT}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARY}
  ${GLUT_LIBRARY}
  ${OPENAL_LIBRARY}
  ${ACE_LIBRARY}
  ${ZLIB_LIBRARIES}
)

#if( UNIX )
#target_link_libraries(client jpeg)
#endif()

#Ein bisschen Aufräumen
if( UNIX )
  install(TARGETS client DESTINATION bin)
  #install(FILES client.conf.dist DESTINATION ${CONF_DIR})
elseif( WIN32 )
  install(TARGETS client DESTINATION "${CMAKE_INSTALL_PREFIX}")
  #install(FILES client.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
  add_custom_command(TARGET client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/dep/win/freeglut/bin/freeglut.dll" $<TARGET_FILE_DIR:client>)
  add_custom_command(TARGET client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/dep/win/glew-1.10.0/bin/Release/Win32/glew32.dll" $<TARGET_FILE_DIR:client>)
  add_custom_command(TARGET client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/dep/win/libflac/lib/libFLAC.dll" $<TARGET_FILE_DIR:client>)
  add_custom_command(TARGET client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/dep/win/libmpg123/lib/libmpg123.dll" $<TARGET_FILE_DIR:client>)
  add_custom_command(TARGET client POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/dep/win/OpenAL/OpenAL32.dll" $<TARGET_FILE_DIR:client>)
  add_custom_target(CopyAndRename COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/modplug.dll ${CMAKE_BINARY_DIR}/libmodplug.dll
)
endif()

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(client ${client_PCH_HDR} ${client_PCH_SRC})
endif()