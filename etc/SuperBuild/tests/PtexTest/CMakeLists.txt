find_package(Ptex REQUIRED)

set(source main.cpp)

add_executable(PtexTest ${header} ${source})
target_link_libraries(PtexTest Ptex)

add_test(PtexTest PtexTest)
