# Find the OpenVKL library.
#
# This module defines the following variables:
#
# * OPENVKL_INCLUDE_DIRS
# * OPENVKL_LIBRARIES
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

find_path(OPENVKL_INCLUDE_DIR NAMES openvkl/openvkl.h)
set(OPENVKL_INCLUDE_DIRS
    ${OPENVKL_INCLUDE_DIR}
    ${rkcommon_INCLUDE_DIRS}
    ${EMBREE_INCLUDE_DIRS}
    ${OpenVDB_INCLUDE_DIRS})

find_library(OPENVKL_MODULE_ISPC_DRIVER_16_LIBRARY NAMES openvkl_module_ispc_driver_16)
find_library(OPENVKL_MODULE_ISPC_DRIVER_8_LIBRARY NAMES openvkl_module_ispc_driver_8)
find_library(OPENVKL_MODULE_ISPC_DRIVER_4_LIBRARY NAMES openvkl_module_ispc_driver_4)
find_library(OPENVKL_MODULE_ISPC_DRIVER_LIBRARY NAMES openvkl_module_ispc_driver)
find_library(OPENVKL_LIBRARY NAMES openvkl)
set(OPENVKL_LIBRARIES
    ${OPENVKL_LIBRARY}
    ${OPENVKL_MODULE_ISPC_DRIVER_LIBRARY}
    ${OPENVKL_MODULE_ISPC_DRIVER_4_LIBRARY}
    ${OPENVKL_MODULE_ISPC_DRIVER_8_LIBRARY}
    ${OPENVKL_MODULE_ISPC_DRIVER_16_LIBRARY}
    ${rkcommon_LIBRARIES}
    ${EMBREE_LIBRARIES}
    ${OpenVDB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    openvkl
    REQUIRED_VARS
        OPENVKL_INCLUDE_DIR
        OPENVKL_LIBRARY
        OPENVKL_MODULE_ISPC_DRIVER_LIBRARY
        OPENVKL_MODULE_ISPC_DRIVER_4_LIBRARY
        OPENVKL_MODULE_ISPC_DRIVER_8_LIBRARY
        OPENVKL_MODULE_ISPC_DRIVER_16_LIBRARY)
mark_as_advanced(
    OPENVKL_INCLUDE_DIR
    OPENVKL_LIBRARY
    OPENVKL_MODULE_ISPC_DRIVER_LIBRARY
    OPENVKL_MODULE_ISPC_DRIVER_4_LIBRARY
    OPENVKL_MODULE_ISPC_DRIVER_8_LIBRARY
    OPENVKL_MODULE_ISPC_DRIVER_16_LIBRARY)

if(openvkl_FOUND AND NOT TARGET openvkl::openvkl)
    add_library(openvkl::openvkl UNKNOWN IMPORTED)
    set_target_properties(openvkl::openvkl PROPERTIES
        IMPORTED_LOCATION "${OPENVKL_LIBRARY}"
        IMPORTED_CONFIGURATIONS "${CMAKE_BUILD_TYPE}"
        IMPORTED_LOCATION_${CMAKE_BUILD_TYPE} "${OPENVKL_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "OPENVKL_FOUND;NOMINMAX"
        INTERFACE_INCLUDE_DIRECTORIES "${OPENVKL_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${OPENVKL_MODULE_ISPC_DRIVER_LIBRARY};${OPENVKL_MODULE_ISPC_DRIVER_4_LIBRARY};${OPENVKL_MODULE_ISPC_DRIVER_8_LIBRARY};${OPENVKL_MODULE_ISPC_DRIVER_16_LIBRARY};rkcommon;embree;OpenVDB")
endif()
if(openvkl_FOUND AND NOT TARGET openvkl)
    add_library(openvkl INTERFACE)
    target_link_libraries(openvkl INTERFACE openvkl::openvkl)
endif()

