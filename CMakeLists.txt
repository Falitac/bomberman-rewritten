cmake_minimum_required(VERSION 3.10)

project(Bomberman VERSION 0.01)

add_subdirectory(fmt)
add_subdirectory(freetype)

#find_package(Freetype REQUIRED)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.cc")
add_executable(Bomberman ${SRC})
set_property(TARGET Bomberman PROPERTY CXX_STANDARD 20)

find_package(fmt)
target_link_libraries(Bomberman fmt::fmt)

if(WIN32)
  target_link_libraries(Bomberman glfw3)
  target_link_libraries(Bomberman glew32)
  target_link_libraries(Bomberman opengl32)
  target_link_libraries(Bomberman assimp)
endif()

if(UNIX)
  target_link_libraries(Bomberman glfw)
  target_link_libraries(Bomberman GLEW)
  target_link_libraries(Bomberman GL)
  target_link_libraries(Bomberman assimp)
endif()
