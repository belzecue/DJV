find_package(OpenImageIO REQUIRED)

set(source main.cpp)

add_executable(OpenImageIOTest ${header} ${source})
target_link_libraries(OpenImageIOTest OpenImageIO)

add_test(OpenImageIOTest OpenImageIOTest)
