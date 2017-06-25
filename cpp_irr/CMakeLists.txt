cmake_minimum_required(VERSION 3.3)
project(maze_survival)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -ggdb -O0 -pthread -Wall -Werror -lIrrlicht -lGL -lXxf86vm -lXext -lX11")
set(ELEGANCE_SRC_PATH "../../Elegance/src/")
set(ELEGANCE_LIB_PATH "../../Elegance/build/" "lib/Linux" )
#set(ELEGANCE_LIB_PATH "../../elegance_build") # for vbox slackware

file(GLOB_RECURSE ALL_FILES src/*.cpp)

LINK_DIRECTORIES(${ELEGANCE_LIB_PATH} lib/Linux/ /usr/lib/x86_64-linux-gnu)

add_executable(maze_survival ${ALL_FILES})

find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )# ${GLUT_INCLUDE_DIRS} )
target_link_libraries(maze_survival ${OPENGL_LIBRARIES})# ${GLUT_LIBRARY} )

target_include_directories(maze_survival PUBLIC ${ELEGANCE_SRC_PATH} ${IRRLICHT_INCLUDE_DIR} include/ src/ /usr/X11R6/include)

target_link_libraries(maze_survival ${IRRLICHT_LIBRARY} libElegance.a libIrrlicht.a)
