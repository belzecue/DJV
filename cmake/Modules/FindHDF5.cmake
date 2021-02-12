# Find the HDF5 library.
#
# This module defines the following variables:
#
# * HDF5_FOUND
# * HDF5_INCLUDE_DIRS
# * HDF5_LIBRARIES
#
# This module defines the following imported targets:
#
# * HDF5::hdf5_hl_cpp
# * HDF5::hdf5_cpp
# * HDF5::hdf5_hl
# * HDF5::hdf5
#
# This module defines the following interfaces:
#
# * HDF5

find_package(ZLIB)

find_path(HDF5_INCLUDE_DIR NAMES hdf5.h)
set(HDF5_INCLUDE_DIRS
    ${HDF5_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS})

if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
    find_library(hdf5_hl_cpp_LIBRARY NAMES hdf5_hl_cpp_D libhdf5_hl_cpp_D hdf5_hl_cpp libhdf5_hl_cpp)
    find_library(hdf5_cpp_LIBRARY NAMES hdf5_cpp_D libhdf5_cpp_D hdf5_cpp libhdf5_cpp)
    find_library(hdf5_hl_LIBRARY NAMES hdf5_hl_D libhdf5_hl_D hdf5_hl libhdf5_hl)
    find_library(hdf5_LIBRARY NAMES hdf5_D libhdf5_D hdf5 libhdf5)
else()
    find_library(hdf5_hl_cpp_LIBRARY NAMES hdf5_hl_cpp libhdf5_hl_cpp)
    find_library(hdf5_cpp_LIBRARY NAMES hdf5_cpp libhdf5_cpp)
    find_library(hdf5_hl_LIBRARY NAMES hdf5_hl libhdf5_hl)
    find_library(hdf5_LIBRARY NAMES hdf5 libhdf5)
endif()
set(HDF5_LIBRARIES
    ${hdf5_hl_cpp_LIBRARY}
    ${hdf5_cpp_LIBRARY}
    ${hdf5_hl_LIBRARY}
    ${hdf5_LIBRARY}
    ${ZLIB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    HDF5
    REQUIRED_VARS
        HDF5_INCLUDE_DIR
        hdf5_hl_cpp_LIBRARY
        hdf5_cpp_LIBRARY
        hdf5_hl_LIBRARY
        hdf5_LIBRARY)
mark_as_advanced(
    HDF5_INCLUDE_DIR
    hdf5_hl_cpp_LIBRARY
    hdf5_cpp_LIBRARY
    hdf5_hl_LIBRARY
    hdf5_LIBRARY)

if(HDF5_FOUND AND NOT TARGET HDF5::hdf5_hl_cpp)
    add_library(HDF5::hdf5_hl_cpp UNKNOWN IMPORTED)
    set_target_properties(HDF5::hdf5_hl_cpp PROPERTIES
        IMPORTED_LOCATION "${hdf5_hl_cpp_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS HDF5_hdf5_hl_cpp_FOUND
        INTERFACE_INCLUDE_DIRECTORIES "${HDF5_INCLUDE_DIR}")
endif()
if(HDF5_FOUND AND NOT TARGET HDF5::hdf5_cpp)
    add_library(HDF5::hdf5_cpp UNKNOWN IMPORTED)
    set_target_properties(HDF5::hdf5_cpp PROPERTIES
        IMPORTED_LOCATION "${hdf5_cpp_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS HDF5_hdf5_cpp_FOUND
        INTERFACE_INCLUDE_DIRECTORIES "${HDF5_INCLUDE_DIR}")
endif()
if(HDF5_FOUND AND NOT TARGET HDF5::hdf5_hl)
    add_library(HDF5::hdf5_hl UNKNOWN IMPORTED)
    set_target_properties(HDF5::hdf5_hl PROPERTIES
        IMPORTED_LOCATION "${hdf5_hl_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS HDF5_hdf5_hl_FOUND
        INTERFACE_INCLUDE_DIRECTORIES "${HDF5_INCLUDE_DIR}")
endif()
if(HDF5_FOUND AND NOT TARGET HDF5::hdf5)
    add_library(HDF5::hdf5 UNKNOWN IMPORTED)
    set_target_properties(HDF5::hdf5 PROPERTIES
        IMPORTED_LOCATION "${hdf5_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS HDF5_hdf5_FOUND
        INTERFACE_INCLUDE_DIRECTORIES "${HDF5_INCLUDE_DIR}")
endif()
if(HDF5_FOUND AND NOT TARGET HDF5::hdf5)
    add_library(HDF5::hdf5 UNKNOWN IMPORTED)
    set_target_properties(HDF5::hdf5 PROPERTIES
        IMPORTED_LOCATION "${hdf5_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "HDF5_hdf5_FOUND;HDF5_FOUND"
        INTERFACE_INCLUDE_DIRECTORIES "${HDF5_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "ZLIB")
endif()
if(HDF5_FOUND AND NOT TARGET HDF5)
    add_library(HDF5 INTERFACE)
    target_link_libraries(HDF5 INTERFACE HDF5::hdf5_hl_cpp)
    target_link_libraries(HDF5 INTERFACE HDF5::hdf5_cpp)
    target_link_libraries(HDF5 INTERFACE HDF5::hdf5_hl)
    target_link_libraries(HDF5 INTERFACE HDF5::hdf5)
endif()
