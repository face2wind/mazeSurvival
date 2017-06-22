cmake_minimum_required(VERSION 3.3)
project(maze_survival)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -ggdb -O0 -pthread -lGL -lXxf86vm -lXext -lX11 -lXcursor -Wall -Werror")
set(ELEGANCE_SRC_PATH "../../Elegance/src/")
set(ELEGANCE_LIB_PATH "../../Elegance/build/" lib/Linux )
#set(ELEGANCE_LIB_PATH "../../elegance_build") # for vbox slackware

file(GLOB_RECURSE ALL_FILES src/*.cpp)

LINK_DIRECTORIES(${ELEGANCE_LIB_PATH} lib/Linux/)

add_executable(maze_survival ${ALL_FILES})

target_include_directories(maze_survival PUBLIC ${ELEGANCE_SRC_PATH} include/ src/ /usr/X11R6/lib64/)

target_link_libraries(maze_survival libElegance.so libIrrlicht.a)



