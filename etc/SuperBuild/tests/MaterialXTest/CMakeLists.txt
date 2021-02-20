find_package(MaterialX REQUIRED)

set(source main.cpp)

add_executable(MaterialXTest ${header} ${source})
target_link_libraries(MaterialXTest MaterialX)

add_test(MaterialXTest MaterialXTest)
