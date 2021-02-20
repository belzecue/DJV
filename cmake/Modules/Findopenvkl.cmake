# Find the OpenVKL library.
#
# This module defines the following variables:
#
# * openvkl_FOUND
# * openvkl_INCLUDE_DIRS
# * openvkl_LIBRARIES
#
# This module defines the following imported targets:
#
# * openvkl::openvkl
#
# This module defines the following interfaces:
#
# * openvkl

find_package(ISPC REQUIRED)
find_package(OpenVDB REQUIRED)
find_package(embree REQUIRED)
find_package(rkcommon REQUIRED)

find_path(openvkl_INCLUDE_DIR NAMES openvkl/openvkl.h)
set(openvkl_INCLUDE_DIRS
    ${openvkl_INCLUDE_DIR}
    ${rkcommon_INCLUDE_DIRS}
    ${embree_INCLUDE_DIRS}
    ${OpenVDB_INCLUDE_DIRS})

find_library(openvkl_module_ispc_driver_16_LIBRARY NAMES openvkl_module_ispc_driver_16)
find_library(openvkl_module_ispc_driver_8_LIBRARY NAMES openvkl_module_ispc_driver_8)
find_library(openvkl_module_ispc_driver_4_LIBRARY NAMES openvkl_module_ispc_driver_4)
find_library(openvkl_module_ispc_driver_LIBRARY NAMES openvkl_module_ispc_driver)
find_library(openvkl_LIBRARY NAMES openvkl)
set(openvkl_LIBRARIES
    ${openvkl_LIBRARY}
    ${openvkl_module_ispc_driver_LIBRARY}
    ${openvkl_module_ispc_driver_4_LIBRARY}
    ${openvkl_module_ispc_driver_8_LIBRARY}
    ${openvkl_module_ispc_driver_16_LIBRARY}
    ${rkcommon_LIBRARIES}
    ${embree_LIBRARIES}
    ${OpenVDB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    openvkl
    REQUIRED_VARS
        openvkl_INCLUDE_DIR
        openvkl_LIBRARY
        openvkl_module_ispc_driver_LIBRARY
        openvkl_module_ispc_driver_4_LIBRARY
        openvkl_module_ispc_driver_8_LIBRARY
        openvkl_module_ispc_driver_16_LIBRARY)
mark_as_advanced(
    openvkl_INCLUDE_DIR
    openvkl_LIBRARY
    openvkl_module_ispc_driver_LIBRARY
    openvkl_module_ispc_driver_4_LIBRARY
    openvkl_module_ispc_driver_8_LIBRARY
    openvkl_module_ispc_driver_16_LIBRARY)

if(openvkl_FOUND AND NOT TARGET openvkl::openvkl)
    add_library(openvkl::openvkl UNKNOWN IMPORTED)
    set_target_properties(openvkl::openvkl PROPERTIES
        IMPORTED_LOCATION "${openvkl_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "openvkl_FOUND;NOMINMAX"
        INTERFACE_INCLUDE_DIRECTORIES "${openvkl_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${openvkl_module_ispc_driver_LIBRARY};${openvkl_module_ispc_driver_4_LIBRARY};${openvkl_module_ispc_driver_8_LIBRARY};${openvkl_module_ispc_driver_16_LIBRARY};rkcommon;embree;OpenVDB")
endif()
if(openvkl_FOUND AND NOT TARGET openvkl)
    add_library(openvkl INTERFACE)
    target_link_libraries(openvkl INTERFACE openvkl::openvkl)
endif()

