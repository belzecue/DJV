# Find the OpenVKL library.
#
# This module defines the following variables:
#
# * OpenVKL_FOUND
# * OpenVKL_INCLUDE_DIRS
# * OpenVKL_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenVKL::OpenVKL
#
# This module defines the following interfaces:
#
# * OpenVKL

find_package(ISPC REQUIRED)
find_package(OpenVDB REQUIRED)
find_package(embree REQUIRED)
find_package(rkcommon REQUIRED)

find_path(OpenVKL_INCLUDE_DIR NAMES openvkl/openvkl.h)
set(OpenVKL_INCLUDE_DIRS
    ${OpenVKL_INCLUDE_DIR}
    ${rkcommon_INCLUDE_DIRS}
    ${embree_INCLUDE_DIRS}
    ${OpenVDB_INCLUDE_DIRS})

find_library(OpenVKL_module_ispc_driver_16_LIBRARY NAMES openvkl_module_ispc_driver_16)
find_library(OpenVKL_module_ispc_driver_8_LIBRARY NAMES openvkl_module_ispc_driver_8)
find_library(OpenVKL_module_ispc_driver_4_LIBRARY NAMES openvkl_module_ispc_driver_4)
find_library(OpenVKL_module_ispc_driver_LIBRARY NAMES openvkl_module_ispc_driver)
find_library(OpenVKL_LIBRARY NAMES openvkl)
set(OpenVKL_LIBRARIES
    ${OpenVKL_LIBRARY}
    ${OpenVKL_module_ispc_driver_LIBRARY}
    ${OpenVKL_module_ispc_driver_4_LIBRARY}
    ${OpenVKL_module_ispc_driver_8_LIBRARY}
    ${OpenVKL_module_ispc_driver_16_LIBRARY}
    ${rkcommon_LIBRARIES}
    ${embree_LIBRARIES}
    ${OpenVDB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenVKL
    REQUIRED_VARS
        OpenVKL_INCLUDE_DIR
        OpenVKL_LIBRARY
        OpenVKL_module_ispc_driver_LIBRARY
        OpenVKL_module_ispc_driver_4_LIBRARY
        OpenVKL_module_ispc_driver_8_LIBRARY
        OpenVKL_module_ispc_driver_16_LIBRARY)
mark_as_advanced(
    OpenVKL_INCLUDE_DIR
    OpenVKL_LIBRARY
    OpenVKL_module_ispc_driver_LIBRARY
    OpenVKL_module_ispc_driver_4_LIBRARY
    OpenVKL_module_ispc_driver_8_LIBRARY
    OpenVKL_module_ispc_driver_16_LIBRARY)

if(OpenVKL_FOUND AND NOT TARGET OpenVKL::OpenVKL)
    add_library(OpenVKL::OpenVKL UNKNOWN IMPORTED)
    set_target_properties(OpenVKL::OpenVKL PROPERTIES
        IMPORTED_LOCATION "${OpenVKL_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "OpenVKL_FOUND;NOMINMAX"
        INTERFACE_INCLUDE_DIRECTORIES "${OpenVKL_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${OpenVKL_module_ispc_driver_LIBRARY};${OpenVKL_module_ispc_driver_4_LIBRARY};${OpenVKL_module_ispc_driver_8_LIBRARY};${OpenVKL_module_ispc_driver_16_LIBRARY};rkcommon;embree;OpenVDB")
endif()
if(OpenVKL_FOUND AND NOT TARGET OpenVKL)
    add_library(OpenVKL INTERFACE)
    target_link_libraries(OpenVKL INTERFACE OpenVKL::OpenVKL)
endif()

