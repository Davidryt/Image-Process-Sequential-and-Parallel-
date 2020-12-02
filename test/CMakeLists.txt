set(BINARY ${CMAKE_PROJECT_NAME}_tst)
add_executable(${BINARY} test-bmp.cpp)
add_test(${BINARY} COMMAND ${BINARY})
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)