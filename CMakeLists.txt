cmake_minimum_required(VERSION 3.18)
project(RockyEngine)

# set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(Game src/Game/EntryPoint.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# include(${CMAKE_CURRENT_SOURCE_DIR}/Rocky/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(Bullet CONFIG REQUIRED)
# # Note: 2 target(s) were omitted.
target_link_libraries(Game PRIVATE LinearMath Bullet3Common BulletDynamics BulletSoftBody)

# find_package(EnTT CONFIG REQUIRED)
# target_link_libraries(Game PRIVATE EnTT::EnTT)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Game PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static)

find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)
# Note: 17 target(s) were omitted.
target_link_libraries(Game PRIVATE OgreMain OgreBites Codec_STBI)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Game PRIVATE imgui::imgui)

# find_package(pugixml CONFIG REQUIRED)
# target_link_libraries(Game PRIVATE pugixml pugixml::static pugixml::pugixml)

# find_package(OpenEXR CONFIG REQUIRED)
# target_link_libraries(Game PRIVATE OpenEXR::IlmImf OpenEXR::IlmImfUtil OpenEXR::IlmImfConfig)

