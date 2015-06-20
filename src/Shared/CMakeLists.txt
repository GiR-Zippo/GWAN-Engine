file(GLOB_RECURSE   sources_Config    Configuration/*.cpp Configuration/*.hpp)
file(GLOB_RECURSE   sources_Cryptography Cryptography/*.cpp Cryptography/*.hpp)
file(GLOB_RECURSE   sources_Debugging Debugging/*.cpp Debugging/*.hpp Debugging/*.h)
file(GLOB_RECURSE   sources_Dynamic   Dynamic/*.cpp Dynamic/*.hpp)
file(GLOB_RECURSE   sources_Logging   Logging/*.cpp Logging/*.hpp)
file(GLOB_RECURSE   sources_Misc      Misc/*.cpp Misc/*.hpp)
file(GLOB           sources_Network Network/*.cpp Network/*.hpp)
file(GLOB_RECURSE   sources_Network_Protocol Network/Protocol/*.cpp Network/Protocol/*.hpp)
file(GLOB_RECURSE   sources_OpcodeHandling OpcodeHandling/*.cpp OpcodeHandling/*.hpp)
file(GLOB_RECURSE   sources_Packet    Packet/*.cpp Packet/*.hpp)
file(GLOB_RECURSE   sources_Threading Threading/*.cpp Threading/*.hpp)
file(GLOB_RECURSE   sources_XML XML/*.cpp XML/*.hpp)
file(GLOB           sources_localdir  *.cpp *.hpp)

source_group(Configuration          FILES ${sources_Config})
source_group(Cryptography           FILES ${sources_Cryptography})
source_group(Debugging              FILES ${sources_Debugging})
source_group(Dynamic                FILES ${sources_Dynamic})
source_group(Logging                FILES ${sources_Logging})
source_group(Network                FILES ${sources_Network})
source_group(Network\\Protocol      FILES ${sources_Network_Protocol})
source_group(OpcodeHandling         FILES ${sources_OpcodeHandling})
source_group(Packet                 FILES ${sources_Packet})
source_group(Threading              FILES ${sources_Threading})
source_group(\\                     FILES ${sources_localdir})

set(shared_STAT_SRCS
  ${shared_STAT_SRCS}
  ${sources_Config}
  ${sources_Cryptography}
  ${sources_Debugging}
  ${sources_Dynamic}
  ${sources_Logging}
  ${sources_Network}
  ${sources_Network_Protocol}
  ${sources_OpcodeHandling}
  ${sources_Packet}
  ${sources_Threading}
  ${sources_XML}
  ${sources_localdir}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Configuration
  ${CMAKE_CURRENT_SOURCE_DIR}/Cryptography
  ${CMAKE_CURRENT_SOURCE_DIR}/Debugging
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamic
  ${CMAKE_CURRENT_SOURCE_DIR}/Logging
  ${CMAKE_CURRENT_SOURCE_DIR}/Network
  ${CMAKE_CURRENT_SOURCE_DIR}/Network/Protocol
  ${CMAKE_CURRENT_SOURCE_DIR}/OpcodeHandling
  ${CMAKE_CURRENT_SOURCE_DIR}/Packet
  ${CMAKE_CURRENT_SOURCE_DIR}/Threading
  ${CMAKE_CURRENT_SOURCE_DIR}/XML
  ${ACE_INCLUDE_DIR}
#  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

add_library(shared STATIC ${shared_STAT_SRCS})

target_link_libraries(shared
  ${ACE_LIBRARY}
)
