cmake_minimum_required(VERSION 3.18)
project(RockyEngine)

add_executable(Game src/Game/EntryPoint.cpp)


# set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
#   CACHE STRING "Vcpkg toolchain file")

# message(VCPKG_ROOT = "${CMAKE_TOOLCHAIN_FILE}")

# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
# find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

# if(COMMAND cmake_policy)
# 	cmake_policy(SET CMP0003 NEW)
# endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)


find_package(Bullet CONFIG REQUIRED)
# Note: 2 target(s) were omitted.
target_link_libraries(Game PRIVATE LinearMath Bullet3Common BulletDynamics BulletSoftBody)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(Game PRIVATE EnTT::EnTT)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Game PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static)

find_package(OGRE CONFIG REQUIRED)
# Note: 17 target(s) were omitted.
target_link_libraries(Game PRIVATE OgreMain Codec_EXR OgreBites Codec_STBI)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Game PRIVATE imgui::imgui)

find_package(pugixml CONFIG REQUIRED)
target_link_libraries(Game PRIVATE pugixml pugixml::static pugixml::pugixml)

find_package(OpenEXR CONFIG REQUIRED)
target_link_libraries(Game PRIVATE OpenEXR::IlmImf OpenEXR::IlmImfUtil OpenEXR::IlmImfConfig)



# add the source files as usual

 
# this also sets the includes and pulls third party dependencies
# target_link_libraries(Game OgreMain Codec_EXR OgreBites Codec_STBI SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static EnTT::EnTT LinearMath Bullet3Common BulletDynamics BulletSoftBody)

