cmake_minimum_required(VERSION 3.7)
project(maze_survival)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -ggdb -O0 -pthread -Wall -Werror -lElegance -lIrrlicht")# -lGL -lXxf86vm -lXext -lX11 -lXcursor")
set(ELEGANCE_SRC_PATH "../../Elegance/")
set(ELEGANCE_LIB_PATH "../../Elegance/lib/linux64/" "lib/Linux/" )

file(GLOB_RECURSE ALL_FILES src/*.cpp)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(X11 REQUIRED)

FIND_LIBRARY(XF86VM_LIBRARY Xxf86vm)

LINK_DIRECTORIES(${ELEGANCE_LIB_PATH} lib/Linux/)

add_executable(maze_survival ${ALL_FILES})

target_include_directories(maze_survival PUBLIC ${ELEGANCE_SRC_PATH} include/ src/ )

target_link_libraries(maze_survival libElegance.a libIrrlicht.a ${OPENGL_gl_LIBRARY} ${X11_LIBRARIES} ${XF86VM_LIBRARY}) #X11_LIBRARIES
