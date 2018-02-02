cmake_minimum_required(VERSION 2.6)
project(glreversi)

add_executable(glreversi main.cpp board.cpp glad.c)

#find_package(glfw3 REQUIRED)

target_link_libraries(glreversi glfw GL X11 pthread Xrandr Xi m dl)

install(TARGETS glreversi RUNTIME DESTINATION bin)
