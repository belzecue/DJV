# Find the OpenColorIO library.
#
# This module defines the following variables:
#
# * OpenColorIO_FOUND
# * OCIO_DEFINITIONS
# * OCIO_INCLUDE_DIRS
# * OCIO_LIBRARIES
#
# This module defines the following imported targets:
#
# * OCIO::OpenColorIO
#
# This module defines the following interfaces:
#
# * OCIO

find_path(OCIO_INCLUDE_DIR NAMES OpenColorIO/OpenColorIO.h)
set(OCIO_INCLUDE_DIRS ${OCIO_INCLUDE_DIR})

find_library(OCIO_LIBRARY NAMES OpenColorIO PATH_SUFFIXES static)
set(OCIO_LIBRARIES ${OCIO_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenColorIO
    REQUIRED_VARS OCIO_INCLUDE_DIR OCIO_LIBRARY)
mark_as_advanced(OCIO_INCLUDE_DIR OCIO_LIBRARY)

set(OCIO_DEFINITIONS -DOpenColorIO_FOUND)
set(OCIO_COMPILE_DEFINITIONS OOpenColorIO_FOUND)
if(NOT BUILD_SHARED_LIBS)
    list(APPEND OCIO_DEFINITIONS -DOpenColorIO_STATIC)
    list(APPEND OCIO_COMPILE_DEFINITIONS OpenColorIO_STATIC)
endif()

if(OpenColorIO_FOUND AND NOT TARGET OCIO::OpenColorIO)
    add_library(OCIO::OpenColorIO UNKNOWN IMPORTED)
    set_target_properties(OCIO::OpenColorIO PROPERTIES
        IMPORTED_LOCATION "${OCIO_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "${OCIO_COMPILE_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${OCIO_INCLUDE_DIR}")
endif()
if(OpenColorIO_FOUND AND NOT TARGET OCIO)
    add_library(OCIO INTERFACE)
    target_link_libraries(OCIO INTERFACE OCIO::OpenColorIO)
endif()
