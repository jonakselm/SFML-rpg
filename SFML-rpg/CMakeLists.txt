cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Custom cmake option
set(RPG_FETCH_UPSTREAM 0 CACHE BOOL "Fetch libraries from upstream instead of using local libraries from the machine")

project(SFML-rpg)

add_executable(SFML-rpg main.cpp stdafx.cpp Animator.cpp
		Map/Chunk.cpp Map/Layer.cpp
		Map/Map.cpp Map/TextureTile.cpp Map/Tileset.cpp
		Entity/Entity.cpp Entity/Player.cpp)

IF (RPG_FETCH_UPSTREAM)
	option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

	include(FetchContent)
	FetchContent_Declare(SFML
		GIT_REPOSITORY https://github.com/SFML/SFML.git
		GIT_TAG 2.6.x
		GIT_SHALLOW ON
		EXCLUDE_FROM_ALL
		SYSTEM)
	FetchContent_MakeAvailable(SFML)
	FetchContent_Declare(
			jsoncpp
			GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
			GIT_TAG        master
	)
	FetchContent_MakeAvailable(jsoncpp)

	# Link sfml
	target_link_libraries(${PROJECT_NAME} PRIVATE sfml-window
		sfml-system
		sfml-graphics)
	# Link jsoncpp
	IF (BUILD_SHARED_LIBS)
		target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_lib)
	ELSE()
		target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_static)
	ENDIF()
ELSE()
	IF (WIN32)
		# Make visible in cmake gui
		set(SFML_LIB_DIR 0 CACHE STRING "SFML library location")
		set(JSONCPP_LIB_DIR 0 CACHE STRING "JSONCPP library location")
		set(SFML_INC_DIR 0 CACHE STRING "SFML include location")
		set(JSONCPP_INC_DIR 0 CACHE STRING "JSONCPP include location")


		# Linking
		target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_LIB_DIR}/$(Configuration)
			PRIVATE ${JSONCPP_LIB_DIR}/$(Configuration))

		# Including
		target_include_directories(${PROJECT_NAME}
			PRIVATE ${CMAKE_SOURCE_DIR}/${SFML_INC_DIR}
			PRIVATE ${CMAKE_SOURCE_DIR}/${JSONCPP_INC_DIR})
	ELSE()
		# Link sfml
		target_link_libraries(${PROJECT_NAME} PRIVATE sfml-window
			sfml-system
			sfml-graphics)
		# Link jsoncpp
		target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp)
	ENDIF()
ENDIF()

target_precompile_headers(SFML-rpg PUBLIC stdafx.h)



target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_SOURCE_DIR}
	PRIVATE ${CMAKE_SOURCE_DIR}/Map
	PRIVATE ${CMAKE_SOURCE_DIR}/Map/Layers
	PRIVATE ${CMAKE_SOURCE_DIR}/Entity)


