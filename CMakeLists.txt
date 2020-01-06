cmake_minimum_required(VERSION 3.15)
project(Wet2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wall -pedantic-errors -g -Werror -DNDEBUG")
add_executable(Wet2 main.cpp Server.cpp Server.h AVLTree.h DataCenter.cpp DataCenter.h LinkedList.h library2.cpp library2.h DataCenterManager.cpp DataCenterManager.h UF.cpp UF.h DynamicHashTable.cpp DynamicHashTable.h)