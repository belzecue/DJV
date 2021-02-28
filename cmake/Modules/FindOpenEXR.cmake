# Find the OpenEXR library.
#
# This module defines the following variables:
#
# * OpenEXR_VERSION
# * OpenEXR_DEFINITIONS
# * OpenEXR_INCLUDE_DIRS
# * OpenEXR_LIBRARIES
#
# * OPENEXR_VERSION
# * OPENEXR_DEFINITIONS
# * OPENEXR_INCLUDE_DIRS
# * OPENEXR_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenEXR::IlmImf
#
# This module defines the following interfaces:
#
# * OpenEXR

set(OpenEXR_VERSION 2.5)

find_package(ZLIB)
find_package(IlmBase)

find_path(OpenEXR_INCLUDE_DIR NAMES Iex.h PATH_SUFFIXES OpenEXR)
set(OpenEXR_INCLUDE_DIRS
    ${OpenEXR_INCLUDE_DIR}
    ${IlmBase_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS})

if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
    find_library(OpenEXR_IlmImf_LIBRARY NAMES IlmImf-2_5_d IlmImf-2_5)
else()
    find_library(OpenEXR_IlmImf_LIBRARY NAMES IlmImf-2_5)
endif()
set(OpenEXR_LIBRARIES
    ${OpenEXR_IlmImf_LIBRARY}
    ${IlmBase_LIBRARIES}
    ${ZLIB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenEXR
    REQUIRED_VARS OpenEXR_INCLUDE_DIR OpenEXR_IlmImf_LIBRARY)
mark_as_advanced(OpenEXR_INCLUDE_DIR OpenEXR_IlmImf_LIBRARY)

set(OpenEXR_DEFINITIONS -DOpenEXR_FOUND)
set(OpenEXR_COMPILE_DEFINITIONS OpenEXR_FOUND)
if(BUILD_SHARED_LIBS)
    list(APPEND OpenEXR_DEFINITIONS -DOPENEXR_DLL)
    list(APPEND OpenEXR_COMPILE_DEFINITIONS OPENEXR_DLL)
endif()

if(OpenEXR_FOUND AND NOT TARGET OpenEXR::IlmImf)
    add_library(OpenEXR::IlmImf UNKNOWN IMPORTED)
    set_target_properties(OpenEXR::IlmImf PROPERTIES
        IMPORTED_LOCATION "${OpenEXR_IlmImf_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "${OpenEXR_COMPILE_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${OpenEXR_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "IlmBase;ZLIB")
endif()
if(OpenEXR_FOUND AND NOT TARGET OpenEXR)
    add_library(OpenEXR INTERFACE)
    target_link_libraries(OpenEXR INTERFACE OpenEXR::IlmImf)
endif()

set(OPENEXR_VERSION ${OpenEXR_VERSION})
set(OPENEXR_DEFINITIONS ${OpenEXR_DEFINITIONS})
set(OPENEXR_INCLUDE_DIRS ${OpenEXR_INCLUDE_DIRS})
set(OPENEXR_LIBRARIES ${OpenEXR_LIBRARIES})
