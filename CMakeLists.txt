cmake_minimum_required(VERSION 3.18.4)
project(Game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui/)
add_library(imgui STATIC)
target_sources(imgui
                PRIVATE
                    ${IMGUI_DIR}/imgui_demo.cpp
                    ${IMGUI_DIR}/imgui_draw.cpp
                    ${IMGUI_DIR}/imgui_tables.cpp
                    ${IMGUI_DIR}/imgui_widgets.cpp
                    ${IMGUI_DIR}/imgui.cpp
)

target_include_directories( imgui
                            PUBLIC ${IMGUI_DIR}
                            PUBLIC ${IMGUI_DIR}/backends
                            PUBLIC ${SDL2_DIR}/include
)

add_executable(Game
        main.cpp
        src/Engine/Core/AssetManager.cpp
        src/Engine/Core/Systems/rlights.h
        third_party/rlImGui/rlImGui.cpp
        )

target_include_directories(Game
        PRIVATE
        "${CMAKE_SOURCE_DIR}/third_party/entt/src"
        "${CMAKE_SOURCE_DIR}/third_party/rlImGui"
        "${CMAKE_SOURCE_DIR}/src/Engine/Core/Systems/"
        "${CMAKE_SOURCE_DIR}/src")

set(CUSTOMIZE_BUILD ON)
set(SUPPORT_FILEFORMAT_JPG ON)
add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/raylib")
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

if (WIN32)
  target_link_libraries(${PROJECT_NAME} raylib imgui Winmm Imm32 SDL2)
else()
  target_link_libraries(${PROJECT_NAME} raylib imgui m dl pthread SDL2)
endif()


#add_custom_command(TARGET Game PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/third_party/entt/src/entt ${CMAKE_SOURCE_DIR})

#add_custom_command(TARGET Game POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/media/build $<TARGET_FILE_DIR:Game>)