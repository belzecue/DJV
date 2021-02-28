# Find the Ptex library.
#
# This module defines the following variables:
#
# * PTEX_DEFINITIONS
# * PTEX_INCLUDE_DIRS
# * PTEX_LIBRARIES
#
# This module defines the following imported targets:
#
# * Ptex::Ptex
#
# This module defines the following interfaces:
#
# * Ptex

find_package(ZLIB REQUIRED)

find_path(PTEX_INCLUDE_DIR NAMES PtexVersion.h)
set(PTEX_INCLUDE_DIRS
    ${PTEX_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS})

find_library(PTEX_LIBRARY NAMES Ptex)
set(PTEX_LIBRARIES
    ${PTEX_LIBRARY}
    ${ZLIB_LIBRARIES})

set(PTEX_DEFINITIONS -DPtex_FOUND)
set(PTEX_COMPILE_DEFINITIONS Ptex_FOUND)
if(NOT BUILD_SHARED_LIBS)
    set(PTEX_DEFINITIONS ${PTEX_DEFINITIONS} -DPTEX_STATIC)
    set(PTEX_COMPILE_DEFINITIONS ${PTEX_COMPILE_DEFINITIONS} PTEX_STATIC)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    Ptex
    REQUIRED_VARS PTEX_INCLUDE_DIR PTEX_LIBRARY)
mark_as_advanced(PTEX_INCLUDE_DIR PTEX_LIBRARY)

if(Ptex_FOUND AND NOT TARGET Ptex::Ptex)
    add_library(Ptex::Ptex UNKNOWN IMPORTED)
    set_target_properties(Ptex::Ptex PROPERTIES
        IMPORTED_LOCATION "${PTEX_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "${PTEX_COMPILE_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${PTEX_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "ZLIB")
endif()
if(Ptex_FOUND AND NOT TARGET Ptex)
    add_library(Ptex INTERFACE)
    target_link_libraries(Ptex INTERFACE Ptex::Ptex)
endif()
