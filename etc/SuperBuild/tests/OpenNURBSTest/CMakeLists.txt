find_package(OpenNURBS REQUIRED)

set(source main.cpp)

add_executable(OpenNURBSTest ${header} ${source})
target_link_libraries(OpenNURBSTest OpenNURBS)

add_test(OpenNURBSTest OpenNURBSTest)
