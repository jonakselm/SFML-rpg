cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(SFML-rpg)

add_subdirectory(Map)
add_subdirectory(Entity)
add_executable(SFML-rpg main.cpp)
target_precompile_headers(SFML-rpg PUBLIC stdafx.h)
# Link my subdirectories
target_link_libraries(${PROJECT_NAME} PUBLIC libEntity libMap)
# Link jsoncpp
target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp)
# Link sfml
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-window
										sfml-system
										sfml-graphics)
target_include_directories(${PROJECT_NAME}
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Map
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Map/Layers
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Entity)
