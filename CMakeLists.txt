# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(src/Engine/Core)

# add the source files as usual
add_executable(GameEngine src/Engine/Core/EntryPoint.cpp)
 
# this also sets the includes and pulls third party dependencies
target_link_libraries(GameEngine OgreBites)