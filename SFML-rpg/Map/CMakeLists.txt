add_library(libMap Map.cpp TextureTile.cpp Tileset.cpp)

target_link_libraries(libMap libEntity libLayer)
target_include_directories(libMap PUBLIC 
						${CMAKE_SOURCE_DIR}
						${CMAKE_SOURCE_DIR}/Entity)
add_subdirectory(Layers)
