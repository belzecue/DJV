# Find the OpenVDB library.
#
# This module defines the following variables:
#
# * OpenVDB_DEFINITIONS
# * OpenVDB_INCLUDE_DIRS
# * OpenVDB_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenVDB::OpenVDB
#
# This module defines the following interfaces:
#
# * OpenVDB

find_package(Blosc REQUIRED)
find_package(TBB REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options iostreams filesystem system thread)
find_package(IlmBase REQUIRED)
find_package(OpenEXR REQUIRED)

find_path(OpenVDB_INCLUDE_DIR NAMES openvdb/version.h)
set(OpenVDB_INCLUDE_DIRS
    ${OpenVDB_INCLUDE_DIR}
    ${OpenEXR_INCLUDE_DIRS}
    ${IlmBase_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${TBB_INCLUDE_DIRS}
    ${Blosc_INCLUDE_DIRS})

find_library(OpenVDB_LIBRARY NAMES openvdb libopenvdb)
set(OpenVDB_LIBRARIES
    ${OpenVDB_LIBRARY}
    ${OpenEXR_LIBRARIES}
    ${IlmBase_LIBRARIES}
    ${Boost_LIBRARIES}
    ${TBB_LIBRARIES}
    ${Blosc_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenVDB
    REQUIRED_VARS OpenVDB_INCLUDE_DIR OpenVDB_LIBRARY)
mark_as_advanced(OpenVDB_INCLUDE_DIR OpenVDB_LIBRARY)

set(OpenVDB_DEFINITIONS -DOpenVDB_FOUND)
set(OpenVDB_COMPILE_DEFINITIONS OpenVDB_FOUND)
if(NOT BUILD_SHARED_LIBS)
    set(OpenVDB_DEFINITIONS ${OpenVDB_DEFINITIONS} -DOPENVDB_STATICLIB -DOPENVDB_OPENEXR_STATICLIB)
    set(OpenVDB_COMPILE_DEFINITIONS ${OpenVDB_COMPILE_DEFINITIONS} OPENVDB_STATICLIB OPENVDB_OPENEXR_STATICLIB)
endif()
if(WIN32)
    set(OpenVDB_DEFINITIONS ${OpenVDB_DEFINITIONS} -D_USE_MATH_DEFINES)
    set(OpenVDB_COMPILE_DEFINITIONS ${OpenVDB_COMPILE_DEFINITIONS} _USE_MATH_DEFINES)
endif()

if(OpenVDB_FOUND AND NOT TARGET OpenVDB::OpenVDB)
    add_library(OpenVDB::OpenVDB UNKNOWN IMPORTED)
    set_target_properties(OpenVDB::OpenVDB PROPERTIES
        IMPORTED_LOCATION "${OpenVDB_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "${OpenVDB_COMPILE_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${OpenVDB_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "OpenEXR;IlmBase;Boost::boost;Boost::iostreams;TBB;Blosc")
endif()
if(OpenVDB_FOUND AND NOT TARGET OpenVDB)
    add_library(OpenVDB INTERFACE)
    target_link_libraries(OpenVDB INTERFACE OpenVDB::OpenVDB)
endif()
