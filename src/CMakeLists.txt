
set(BINARY ${CMAKE_PROJECT_NAME})
add_executable(${BINARY}_run
        src/main.cpp src/bmp.h src/bmp.cpp test/test-bmp.cpp)
add_library(${BINARY}_lib STATIC src/main.cpp src/bmp.h src/bmp.cpp test/test-bmp.cpp)