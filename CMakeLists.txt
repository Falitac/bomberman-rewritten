cmake_minimum_required(VERSION 3.10)

project(Bomberman VERSION 0.01)

add_subdirectory(fmt)
add_subdirectory(freetype)

#find_package(Freetype REQUIRED)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.cc")
add_executable(Bomberman ${SRC})
set_property(TARGET Bomberman PROPERTY CXX_STANDARD 20)

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

# google tests:
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  ${PROJECT_NAME}-test
  "tests/Tests.cc"
)
target_link_libraries(
  ${PROJECT_NAME}-test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-test)