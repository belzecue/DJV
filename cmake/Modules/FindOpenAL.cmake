# Find the OpenAL library.
#
# This module defines the following variables:
#
# * OpenAL_FOUND
# * OpenAL_INCLUDE_DIRS
# * OpenAL_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenAL::openal
#
# This module defines the following interfaces:
#
# * OpenAL

if(APPLE)
    find_library(AUDIO_TOOLBOX AudioToolbox)
    find_library(CORE_AUDIO CoreAudio)
endif()

find_path(OPENAL_INCLUDE_DIR NAMES AL/al.h)
set(OPENAL_INCLUDE_DIRS ${OPENAL_INCLUDE_DIR})

find_library(OPENAL_LIBRARY NAMES openal OpenAL32)
if(WIN32)
    set(OPENAL_LIBRARIES ${OPENAL_LIBRARY} Winmm)
elseif(APPLE)
    find_library(AUDIO_TOOLBOX AudioToolbox)
    find_library(CORE_AUDIO CoreAudio)
    set(OPENAL_LIBRARIES
        ${OPENAL_LIBRARY}
        ${AUDIO_TOOLBOX}
        ${CORE_AUDIO})
else()
    set(OPENAL_LIBRARIES ${OPENAL_LIBRARY})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenAL
    REQUIRED_VARS OpenAL_INCLUDE_DIR OpenAL_LIBRARY)
mark_as_advanced(OpenAL_INCLUDE_DIR OpenAL_LIBRARY)

if(OpenAL_FOUND AND NOT TARGET OpenAL::openal)
    add_library(OpenAL::openal UNKNOWN IMPORTED)
    set(OpenAL_COMPILE_DEFINITIONS OpenAL_FOUND)
    set(OpenAL_INCLUDE_DIRECTORIES ${OpenAL_INCLUDE_DIR})
    set(OpenAL_LINK_LIBRARIES)
    if(NOT BUILD_SHARED_LIBS)
		set(OpenAL_COMPILE_DEFINITIONS ${OpenAL_COMPILE_DEFINITIONS} AL_LIBTYPE_STATIC)
    endif()
    if(WIN32)
		set(OpenAL_LINK_LIBRARIES ${OpenAL_LINK_LIBRARIES} Winmm)
    elseif(APPLE)
		set(OpenAL_LINK_LIBRARIES ${OpenAL_LINK_LIBRARIES} ${AUDIO_TOOLBOX} ${CORE_AUDIO})
    endif()
    set_target_properties(OpenAL::openal PROPERTIES
        IMPORTED_LOCATION "${OpenAL_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "${OpenAL_COMPILE_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${OpenAL_INCLUDE_DIRECTORIES}"
        INTERFACE_LINK_LIBRARIES "${OpenAL_LINK_LIBRARIES}")
endif()
if(OPENAL_FOUND AND NOT TARGET OpenAL)
    add_library(OpenAL INTERFACE)
    target_link_libraries(OpenAL INTERFACE OpenAL::openal)
endif()
