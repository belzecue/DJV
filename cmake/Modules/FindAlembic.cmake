# Find the Alembic library.
#
# This module defines the following variables:
#
# * Alembic_FOUND
# * Alembic_INCLUDE_DIRS
# * Alembic_LIBRARIES
#
# This module defines the following imported targets:
#
# * Alembic::Alembic
#
# This module defines the following interfaces:
#
# * Alembic

find_package(HDF5 REQUIRED)
find_package(Boost REQUIRED)
find_package(IlmBase REQUIRED)

find_path(Alembic_INCLUDE_DIR NAMES Alembic/Abc/All.h)
set(Alembic_INCLUDE_DIRS
    ${Alembic_INCLUDE_DIR}
    ${HDF5_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${IlmBase_INCLUDE_DIRS})

find_library(Alembic_LIBRARY NAMES Alembic)
set(Alembic_LIBRARIES
    ${Alembic_LIBRARY}
    ${IlmBase_LIBRARIES}
    ${Boost_LIBRARIES}
    ${HDF5_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    Alembic
    REQUIRED_VARS Alembic_INCLUDE_DIR Alembic_LIBRARY)
mark_as_advanced(Alembic_INCLUDE_DIR Alembic_LIBRARY)

if(Alembic_FOUND AND NOT TARGET Alembic::Alembic)
    add_library(Alembic::Alembic UNKNOWN IMPORTED)
    set_target_properties(Alembic::Alembic PROPERTIES
        IMPORTED_LOCATION "${Alembic_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "Alembic_FOUND"
        INTERFACE_INCLUDE_DIRECTORIES "${Alembic_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "IlmBase;Boost::boost;HDF5")
endif()
if(Alembic_FOUND AND NOT TARGET Alembic)
    add_library(Alembic INTERFACE)
    target_link_libraries(Alembic INTERFACE Alembic::Alembic)
endif()
