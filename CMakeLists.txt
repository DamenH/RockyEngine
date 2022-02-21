cmake_minimum_required(VERSION 3.18)
project(RockyEngine)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(Game src/Game/EntryPoint.cpp)

include_directories(src)

add_library(entt INTERFACE)
target_include_directories(entt INTERFACE lib)
target_link_libraries(Game PRIVATE entt)

find_package(Bullet CONFIG REQUIRED)
target_link_libraries(Game PRIVATE LinearMath Bullet3Common BulletDynamics BulletSoftBody)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Game PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static)

find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)
target_link_libraries(Game PRIVATE OgreMain OgreBites Codec_STBI)

# Copy resources.cfg out of media submodule
add_custom_command(TARGET Game POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/media/build $<TARGET_FILE_DIR:Game>)
