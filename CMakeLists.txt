cmake_minimum_required(VERSION 3.18.4)
project(Game)

set(CMAKE_CXX_STANDARD 20)

add_executable(Game main.cpp)

target_include_directories(Game
        PRIVATE
        "${CMAKE_SOURCE_DIR}/third_party/entt/src"
        "${CMAKE_SOURCE_DIR}/src")

set(CUSTOMIZE_BUILD On)
set(SUPPORT_FILEFORMAT_JPG On)
add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/raylib")
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
if (WIN32)
  target_link_libraries(${PROJECT_NAME} raylib Winmm Imm32 SDL2)
else()
  target_link_libraries(${PROJECT_NAME} raylib m dl pthread SDL2)
endif()

#add_custom_command(TARGET Game PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/third_party/entt/src/entt ${CMAKE_SOURCE_DIR})

#add_custom_command(TARGET Game POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/media/build $<TARGET_FILE_DIR:Game>)