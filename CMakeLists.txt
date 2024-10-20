# CMakeList.txt : CMake project for snake, include source and define
# project specific logic here.
#

# Add source to this project's executable.
find_package(SFML 2.6 COMPONENTS window graphics system audio REQUIRED)
add_executable (snake "snake.cpp"  "controls.cpp")
target_link_libraries(snake sfml-graphics sfml-window sfml-system sfml-audio)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET snake PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.


