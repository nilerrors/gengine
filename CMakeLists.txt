cmake_minimum_required(VERSION 3.5)

project(engine)

############################################################
# Set compiler flags
############################################################
# If supported by your compiler, you can add the -Wall, -Wextra, –fstack-protector-all and -g3 flags here.
set(OWN_GXX_FLAGS "-std=c++14 -Wall -Wextra -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${OWN_GXX_FLAGS}")

############################################################
# List all sources
############################################################
set(engine_sources
		lib/easy_image.cc
	engine.cc
		lib/ini_configuration.cc
		lib/l_parser.cc
		lib/vector3d.cc
		gengine/utils.h
		gengine/line.cpp
		gengine/line.h
		gengine/utils.cpp
		gengine/line3d.cpp
		gengine/figures3d.cpp
		gengine/figures3d.h
		gengine/zbuffer.cpp
		gengine/zbuffer.h
		gengine/zbufwireframe.cpp
		gengine/zbufwireframe.h
		gengine/zbuffering.cpp
		gengine/zbuffering.h
)

############################################################
# Create an executable
############################################################
set(exe_name "engine")
add_executable( ${exe_name} ${engine_sources} )
install( TARGETS ${exe_name} DESTINATION ${PROJECT_SOURCE_DIR}/ )
