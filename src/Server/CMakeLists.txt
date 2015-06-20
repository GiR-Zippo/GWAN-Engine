file(GLOB_RECURSE   sources_Configuration Configuration/*.cpp Configuration/*.hpp)
file(GLOB_RECURSE   sources_Downloading Downloading/*.cpp Downloading/*.hpp)
file(GLOB_RECURSE   sources_Server_Client Server/Client/*.cpp Server/Client/*.hpp)
file(GLOB           sources_Server Server/*.cpp Server/*.hpp)
file(GLOB           sources_localdir *.cpp *.hpp *.rc)

source_group(Configuration          FILES ${sources_Configuration})
source_group(Downloading            FILES ${sources_Downloading})
source_group(Server                 FILES ${sources_Server})
source_group(Server\\Client         FILES ${sources_Server_Client})
source_group(\\                     FILES ${sources_localdir})

set(server_SRCS
  ${server_SRCS}
  ${sources_Configuration}
  ${sources_Downloading}
  ${sources_Server}
  ${sources_Server_Client}
  ${sources_localdir}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_SOURCE_DIR}/src/Shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/Shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/Shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/Shared/Logging
  ${CMAKE_SOURCE_DIR}/src/Shared/Network
  ${CMAKE_SOURCE_DIR}/src/Shared/Network/Protocol
  ${CMAKE_SOURCE_DIR}/src/Shared/Packet
  ${CMAKE_SOURCE_DIR}/src/Shared/Threading
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Configuration
  ${CMAKE_CURRENT_SOURCE_DIR}/Downloading
  ${CMAKE_CURRENT_SOURCE_DIR}/Server
  ${CMAKE_CURRENT_SOURCE_DIR}/Server/Client
  ${ACE_INCLUDE_DIR}
  ${PHP5_INCLUDE_PATH}
  ${OPENSSL_INCLUDE_DIR}
)

set(server_LINK_FLAGS "")

add_executable(server ${server_SRCS})

if( NOT WIN32 )
  add_definitions(-DSERVER_CONFIG='"${CONF_DIR}/server.conf"')
endif()

add_dependencies(server revision.h)

if( UNIX )
  set(server_LINK_FLAGS "-pthread ${server_LINK_FLAGS}")
endif()

set_target_properties(server PROPERTIES LINK_FLAGS "${server_LINK_FLAGS}")

target_link_libraries(server
  shared
#  ${JEMALLOC_LIBRARY}
  ${READLINE_LIBRARY}
  ${TERMCAP_LIBRARY}
  ${ACE_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${ZLIB_LIBRARIES}
)

if( WIN32 )
  add_custom_command(TARGET server
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/server.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )
endif()

if( UNIX )
  install(TARGETS server DESTINATION bin)
  install(FILES server.conf.dist DESTINATION etc)
elseif( WIN32 )
  install(TARGETS server DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES server.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()


