# Find the OpenSubdiv library.
#
# This module defines the following variables:
#
# * OpenSubdiv_FOUND
# * OpenSubdiv_INCLUDE_DIRS
# * OpenSubdiv_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenSubdiv::OpenSubdiv
#
# This module defines the following interfaces:
#
# * OpenSubdiv

find_package(TBB REQUIRED)

find_path(OpenSubdiv_INCLUDE_DIR NAMES opensubdiv/version.h)
set(OpenSubdiv_INCLUDE_DIRS
    ${OpenSubdiv_INCLUDE_DIR}
    ${TBB_INCLUDE_DIRS})

find_library(OpenSubdiv_LIBRARY NAMES osdCPU)
set(OpenSubdiv_LIBRARIES
    ${OpenSubdiv_LIBRARY}
    ${TBB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenSubdiv
    REQUIRED_VARS OpenSubdiv_INCLUDE_DIR OpenSubdiv_LIBRARY)
mark_as_advanced(OpenSubdiv_INCLUDE_DIR OpenSubdiv_LIBRARY)

if(OpenSubdiv_FOUND AND NOT TARGET OpenSubdiv::OpenSubdiv)
    add_library(OpenSubdiv::OpenSubdiv UNKNOWN IMPORTED)
    set_target_properties(OpenSubdiv::OpenSubdiv PROPERTIES
        IMPORTED_LOCATION "${OpenSubdiv_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "OpenSubdiv_FOUND"
        INTERFACE_INCLUDE_DIRECTORIES "${OpenSubdiv_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "TBB")
endif()
if(OpenSubdiv_FOUND AND NOT TARGET OpenSubdiv)
    add_library(OpenSubdiv INTERFACE)
    target_link_libraries(OpenSubdiv INTERFACE OpenSubdiv::OpenSubdiv)
endif()
