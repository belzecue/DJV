find_package(OpenVKL REQUIRED)

set(source main.cpp)

add_executable(OpenVKLTest ${header} ${source})
target_link_libraries(OpenVKLTest OpenVKL)

add_test(OpenVKLTest OpenVKLTest)
