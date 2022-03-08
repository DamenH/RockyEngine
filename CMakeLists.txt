cmake_minimum_required(VERSION 3.18.4)
project(Game)

set(CMAKE_CXX_STANDARD 20)

add_executable(Game main.cpp)

target_include_directories(Game
        PRIVATE
        "${CMAKE_SOURCE_DIR}/third_party/entt/src"
        "${CMAKE_SOURCE_DIR}/src")


add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/raylib")

#add_custom_command(TARGET Game PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/third_party/entt/src/entt ${CMAKE_SOURCE_DIR})

add_custom_command(TARGET Game POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/media/build $<TARGET_FILE_DIR:Game>)