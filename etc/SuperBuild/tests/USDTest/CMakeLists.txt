find_package(USD REQUIRED)

set(source main.cpp)

add_executable(USDTest ${header} ${source})
target_link_libraries(USDTest USD)

add_test(USDTest USDTest)
