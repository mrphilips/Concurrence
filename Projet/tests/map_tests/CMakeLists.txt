include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(mapTest cell_test.cpp entity_test.cpp)

target_link_libraries(mapTest gtest gtest_main)
target_link_libraries(mapTest project)