if(CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
  add_definitions(-fno-delete-null-pointer-checks)
endif()

#add_subdirectory(Client)
#add_subdirectory(Server)
add_subdirectory(Shared)