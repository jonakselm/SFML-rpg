cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
IF (WIN32)
	set(SFML_LIB_DIR 0 CACHE STRING "SFML library location")
	set(JSONCPP_LIB_DIR 0 CACHE STRING "JSONCPP library location")
	set(SFML_INC_DIR 0 CACHE STRING "SFML include location")
	set(JSONCPP_INC_DIR 0 CACHE STRING "JSONCPP include location")
ENDIF()

project(SFML-rpg)

add_executable(SFML-rpg main.cpp stdafx.cpp Animator.cpp
		Map/Chunk.cpp Map/Layer.cpp
		Map/Map.cpp Map/TextureTile.cpp Map/Tileset.cpp
		Entity/Entity.cpp Entity/Player.cpp)
target_precompile_headers(SFML-rpg PUBLIC stdafx.h)
IF (WIN32)
	target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_LIB_DIR}/$(Configuration)
		PRIVATE ${JSONCPP_LIB_DIR}/$(Configuration))
ENDIF()

# Link sfml
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-window
	sfml-system
	sfml-graphics)
# Link jsoncpp
target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp)


target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_SOURCE_DIR}
	PRIVATE ${CMAKE_SOURCE_DIR}/Map
	PRIVATE ${CMAKE_SOURCE_DIR}/Map/Layers
	PRIVATE ${CMAKE_SOURCE_DIR}/Entity)
IF (WIN32)
	target_include_directories(${PROJECT_NAME}
		PRIVATE ${CMAKE_SOURCE_DIR}/${SFML_INC_DIR}
		PRIVATE ${CMAKE_SOURCE_DIR}/${JSONCPP_INC_DIR})
ENDIF()
