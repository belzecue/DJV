find_package(OpenSubdiv REQUIRED)

set(source main.cpp)

add_executable(OpenSubdivTest ${header} ${source})
target_link_libraries(OpenSubdivTest OpenSubdiv)

add_test(OpenSubdivTest OpenSubdivTest)
