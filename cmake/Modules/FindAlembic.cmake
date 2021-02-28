# Find the Alembic library.
#
# This module defines the following variables:
#
# * ALEMBIC_INCLUDE_DIRS
# * ALEMBIC_LIBRARIES
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

find_path(ALEMBIC_INCLUDE_DIR NAMES Alembic/Abc/All.h)
set(ALEMBIC_INCLUDE_DIRS
    ${ALEMBIC_INCLUDE_DIR}
    ${HDF5_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${IlmBase_INCLUDE_DIRS})

find_library(ALEMBIC_LIBRARY NAMES Alembic)
set(ALEMBIC_LIBRARIES
    ${ALEMBIC_LIBRARY}
    ${IlmBase_LIBRARIES}
    ${Boost_LIBRARIES}
    ${HDF5_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    Alembic
    REQUIRED_VARS ALEMBIC_INCLUDE_DIR ALEMBIC_LIBRARY)
mark_as_advanced(ALEMBIC_INCLUDE_DIR ALEMBIC_LIBRARY)

if(Alembic_FOUND AND NOT TARGET Alembic::Alembic)
    add_library(Alembic::Alembic UNKNOWN IMPORTED)
    set_target_properties(Alembic::Alembic PROPERTIES
        IMPORTED_LOCATION "${ALEMBIC_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "Alembic_FOUND"
        INTERFACE_INCLUDE_DIRECTORIES "${ALEMBIC_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "IlmBase;Boost::boost;HDF5")
endif()
if(Alembic_FOUND AND NOT TARGET Alembic)
    add_library(Alembic INTERFACE)
    target_link_libraries(Alembic INTERFACE Alembic::Alembic)
endif()
