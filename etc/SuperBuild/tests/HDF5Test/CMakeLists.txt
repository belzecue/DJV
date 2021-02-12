find_package(HDF5 REQUIRED)

set(source main.cpp)

add_executable(HDF5Test ${header} ${source})
target_link_libraries(HDF5Test HDF5)

add_test(HDF5Test HDF5Test)
