# Find the Ptex library.
#
# This module defines the following variables:
#
# * Ptex_FOUND
# * Ptex_INCLUDE_DIRS
# * Ptex_LIBRARIES
#
# This module defines the following imported targets:
#
# * Ptex::Ptex
#
# This module defines the following interfaces:
#
# * Ptex

find_package(ZLIB REQUIRED)

find_path(Ptex_INCLUDE_DIR NAMES PtexVersion.h)
set(Ptex_INCLUDE_DIRS
    ${Ptex_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS})

find_library(Ptex_LIBRARY NAMES Ptex)
set(Ptex_LIBRARIES
    ${Ptex_LIBRARY}
    ${ZLIB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    Ptex
    REQUIRED_VARS Ptex_INCLUDE_DIR Ptex_LIBRARY)
mark_as_advanced(Ptex_INCLUDE_DIR Ptex_LIBRARY)

if(Ptex_FOUND AND NOT TARGET Ptex::Ptex)
    add_library(Ptex::Ptex UNKNOWN IMPORTED)
    set_target_properties(Ptex::Ptex PROPERTIES
        IMPORTED_LOCATION "${Ptex_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS Ptex_FOUND
        INTERFACE_INCLUDE_DIRECTORIES "${Ptex_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "ZLIB")
endif()
if(Ptex_FOUND AND NOT TARGET Ptex)
    add_library(Ptex INTERFACE)
    target_link_libraries(Ptex INTERFACE Ptex::Ptex)
endif()
