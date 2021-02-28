# Find the OpenSubdiv library.
#
# This module defines the following variables:
#
# * OPENSUBDIV_INCLUDE_DIRS
# * OPENSUBDIV_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenSubdiv::OpenSubdiv
#
# This module defines the following interfaces:
#
# * OpenSubdiv

find_package(TBB REQUIRED)

find_path(OPENSUBDIV_INCLUDE_DIR NAMES opensubdiv/version.h)
set(OPENSUBDIV_INCLUDE_DIRS
    ${OPENSUBDIV_INCLUDE_DIR}
    ${TBB_INCLUDE_DIRS})

find_library(OPENSUBDIV_CPU_LIBRARY NAMES osdCPU)
find_library(OPENSUBDIV_GPU_LIBRARY NAMES osdGPU)
set(OPENSUBDIV_LIBRARIES
    ${OPENSUBDIV_CPU_LIBRARY}
    ${OPENSUBDIV_GPU_LIBRARY}
    ${TBB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenSubdiv
    REQUIRED_VARS
        OPENSUBDIV_INCLUDE_DIR
        OPENSUBDIV_CPU_LIBRARY
        OPENSUBDIV_GPU_LIBRARY)
mark_as_advanced(
    OPENSUBDIV_INCLUDE_DIR
    OPENSUBDIV_CPU_LIBRARY
    OPENSUBDIV_GPU_LIBRARY)

if(OpenSubdiv_FOUND AND NOT TARGET OpenSubdiv::OpenSubdiv)
    add_library(OpenSubdiv::OpenSubdiv UNKNOWN IMPORTED)
    set_target_properties(OpenSubdiv::OpenSubdiv PROPERTIES
        IMPORTED_LOCATION "${OPENSUBDIV_CPU_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "OPENSUBDIV_FOUND"
        INTERFACE_INCLUDE_DIRECTORIES "${OPENSUBDIV_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${OPENSUBDIV_GPU_LIBRARY};TBB")
endif()
if(OpenSubdiv_FOUND AND NOT TARGET OpenSubdiv)
    add_library(OpenSubdiv INTERFACE)
    target_link_libraries(OpenSubdiv INTERFACE OpenSubdiv::OpenSubdiv)
endif()
