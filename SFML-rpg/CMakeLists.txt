cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Custom cmake option
set(RPG_FETCH_LIBS 0 CACHE BOOL "Fetch libraries from upstream instead of using local libraries on the machine")

project(SFML-rpg)

add_executable(SFML-rpg main.cpp stdafx.cpp Animator.cpp
		Map/Chunk.cpp Map/Layer.cpp
		Map/Map.cpp Map/TextureTile.cpp Map/Tileset.cpp
		Entity/Entity.cpp Entity/Player.cpp)

IF (RPG_FETCH_LIBS)
	option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

	include(FetchContent)
	message("Fetching SFML")
	FetchContent_Declare(SFML
		GIT_REPOSITORY https://github.com/SFML/SFML.git
		GIT_TAG 2.6.x
		GIT_SHALLOW ON
		EXCLUDE_FROM_ALL
		SYSTEM)
	FetchContent_MakeAvailable(SFML)
	
	message("Fetching JSONCPP")
	FetchContent_Declare(
			jsoncpp
			GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
			GIT_TAG        master
			GIT_SHALLOW ON
	)
	FetchContent_MakeAvailable(jsoncpp)

	# Link sfml
	message("Linking SFML")
	target_link_libraries(${PROJECT_NAME}
		PRIVATE sfml-window
		PRIVATE sfml-system
		PRIVATE sfml-graphics)
	# Link jsoncpp
	message("Linking JSONCPP")
	IF (BUILD_SHARED_LIBS)
		set(RPG_INTERNAL_JSONCPP jsoncpp_lib)
	ELSE()
		set(RPG_INTERNAL_JSONCPP jsoncpp_static)
	ENDIF()

	target_link_libraries(${PROJECT_NAME} PRIVATE ${RPG_INTERNAL_JSONCPP})
ELSE()	
	IF (WIN32)
		# Make visible in cmake gui
		set(RPG_USE_RELATIVE_PATH 0 CACHE BOOL "Whether to use a relative path or not")
		set(RPG_SFML_LIB_DIR 0 CACHE PATH "SFML library location")
		set(RPG_SFML_INC_DIR 0 CACHE PATH "SFML include location")
		set(RPG_JSONCPP_INC_DIR 0 CACHE PATH "JSONCPP include location")
		set(RPG_JSONCPP_LIB_DIR 0 CACHE PATH "JSONCPP library location")

		IF (RPG_USE_RELATIVE_PATH)
			set(RPG_INTERNAL_SFML_LIB_DIR ${CMAKE_SOURCE_DIR}/${RPG_SFML_LIB_DIR})
			set(RPG_INTERNAL_SFML_INC_DIR ${CMAKE_SOURCE_DIR}/${RPG_SFML_INC_DIR})
			set(RPG_INTERNAL_JSONCPP_LIB_DIR ${CMAKE_SOURCE_DIR}/${RPG_JSONCPP_LIB_DIR})
			set(RPG_INTERNAL_JSONCPP_INC_DIR ${CMAKE_SOURCE_DIR}/${RPG_JSONCPP_INC_DIR})
		ELSE()
			set(RPG_INTERNAL_SFML_LIB_DIR ${RPG_SFML_LIB_DIR})
			set(RPG_INTERNAL_SFML_INC_DIR ${RPG_SFML_INC_DIR})
			set(RPG_INTERNAL_JSONCPP_LIB_DIR ${RPG_JSONCPP_LIB_DIR})
			set(RPG_INTERNAL_JSONCPP_INC_DIR ${RPG_JSONCPP_INC_DIR})
		ENDIF()

		message("Using relative path: ${RPG_USE_RELATIVE_PATH}")
		message("SFML absolute link path: ${RPG_INTERNAL_SFML_LIB_DIR}")
		message("SFML absolute include path: ${RPG_INTERNAL_SFML_INC_DIR}")
		message("JSONCPP absolute link path: ${RPG_INTERNAL_JSONCPP_LIB_DIR}")
		message("JSONCPP absolute include path: ${RPG_INTERNAL_JSONCPP_INC_DIR}")
		# Linking
		# Using cmake-generator-expressions to automatically link the correct libraries
		target_link_directories(${PROJECT_NAME}
			PRIVATE ${RPG_INTERNAL_SFML_LIB_DIR}/$<CONFIG>
			PRIVATE ${RPG_INTERNAL_JSONCPP_LIB_DIR}/$<CONFIG>)

		# Including
		target_include_directories(${PROJECT_NAME}
			PRIVATE ${RPG_INTERNAL_SFML_INC_DIR}
			PRIVATE ${RPG_INTERNAL_JSONCPP_INC_DIR})
	ENDIF()
	# Link sfml
	# This uses cmake-generator-expressions to append "-d" at the end if it is a debug build on windows
	set(RPG_INTERNAL_ADD_POSTFIX_EXPR "$<$<AND:$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>,$<BOOL:${WIN32}>>:-d>")
	target_link_libraries(${PROJECT_NAME}
		PRIVATE sfml-window${RPG_INTERNAL_ADD_POSTFIX_EXPR}
		PRIVATE sfml-system${RPG_INTERNAL_ADD_POSTFIX_EXPR}
		PRIVATE sfml-graphics${RPG_INTERNAL_ADD_POSTFIX_EXPR})
	# Link jsoncpp
	target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp)
ENDIF()

target_precompile_headers(SFML-rpg PUBLIC stdafx.h)



target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_SOURCE_DIR}
	PRIVATE ${CMAKE_SOURCE_DIR}/Map
	PRIVATE ${CMAKE_SOURCE_DIR}/Map/Layers
	PRIVATE ${CMAKE_SOURCE_DIR}/Entity)


