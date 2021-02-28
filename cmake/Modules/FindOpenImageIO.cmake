# Find the OpenImageIO library.
#
# This module defines the following variables:
#
# * OIIO_INCLUDE_DIRS
# * OIIO_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenImageIO::OpenImageIO
#
# This module defines the following interfaces:
#
# * OpenImageIO

find_package(ZLIB REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options iostreams filesystem system thread)
find_package(TIFF REQUIRED)
find_package(OpenEXR REQUIRED)
find_package(JPEG)
find_package(PNG)
find_package(FreeType)
find_package(HDF5)
find_package(OpenCV)
find_package(TBB)
find_package(OpenVDB)
find_package(Ptex)

find_path(OIIO_INCLUDE_DIR NAMES OpenImageIO/imageio.h)
set(OIIO_INCLUDE_DIRS
    ${OIIO_INCLUDE_DIR}
    ${OpenEXR_INCLUDE_DIRS}
    ${TIFF_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS})
if(JPEG_FOUND)
    list(APPEND OIIO_INCLUDE_DIRS ${JPEG_INCLUDE_DIRS})
endif()
if(PNG_FOUND)
    list(APPEND OIIO_INCLUDE_DIRS ${PNG_INCLUDE_DIRS})
endif()
if(FreeType_FOUND)
    list(APPEND OIIO_INCLUDE_DIRS ${FreeType_INCLUDE_DIRS})
endif()
if(HDF5_FOUND)
    list(APPEND OIIO_INCLUDE_DIRS ${HDF5_INCLUDE_DIRS})
endif()
if(OpenCV_FOUND)
    list(APPEND OIIO_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
endif()
if(TBB_FOUND)
    list(APPEND OIIO_INCLUDE_DIRS ${TBB_INCLUDE_DIRS})
endif()
if(OpenVDB_FOUND)
    list(APPEND OIIO_INCLUDE_DIRS ${OpenVDB_INCLUDE_DIRS})
endif()
if(Ptex_FOUND)
    list(APPEND OIIO_INCLUDE_DIRS ${Ptex_INCLUDE_DIRS})
endif()

find_library(OIIO_LIBRARY NAMES OpenImageIO)
set(OIIO_LIBRARIES
    ${OIIO_LIBRARY}
    ${OpenEXR_LIBRARIES}
    ${TIFF_LIBRARIES}
    ${Boost_LIBRARIES}
    ${ZLIB_LIBRARIES})
if(JPEG_FOUND)
    list(APPEND OIIO_LIBRARIES ${JPEG_LIBRARIES})
endif()
if(PNG_FOUND)
    list(APPEND OIIO_LIBRARIES ${PNG_LIBRARIES})
endif()
if(FreeType_FOUND)
    list(APPEND OIIO_LIBRARIES ${FreeType_LIBRARIES})
endif()
if(HDF5_FOUND)
    list(APPEND OIIO_LIBRARIES ${HDF5_LIBRARIES})
endif()
if(OpenCV_FOUND)
    list(APPEND OIIO_LIBRARIES ${OpenCV_LIBRARIES})
endif()
if(TBB_FOUND)
    list(APPEND OIIO_LIBRARIES ${TBB_LIBRARIES})
endif()
if(OpenVDB_FOUND)
    list(APPEND OIIO_LIBRARIES ${OpenVDB_LIBRARIES})
endif()
if(Ptex_FOUND)
    list(APPEND OIIO_LIBRARIES ${Ptex_LIBRARIES})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenImageIO
    REQUIRED_VARS OIIO_INCLUDE_DIR OIIO_LIBRARY)
mark_as_advanced(OIIO_INCLUDE_DIR OIIO_LIBRARY)

if(OpenImageIO_FOUND AND NOT TARGET OpenImageIO::OpenImageIO)
    add_library(OpenImageIO::OpenImageIO UNKNOWN IMPORTED)
    set(OIIO_COMPILE_DEFINITIONS OpenImageIO_FOUND)
    if(NOT BUILD_SHARED_LIBS)
        list(APPEND OIIO_COMPILE_DEFINITIONS OIIO_STATIC_DEFINE)
    endif()
    set(OIIO_LINK_LIBRARIES
        ${OpenEXR_LIBRARIES}
        ${TIFF_LIBRARIES}
        ${ZLIB_LIBRARIES}
        Boost::boost
        Boost::filesystem
        Boost::system
        Boost::thread)
    if(JPEG_FOUND)
        list(APPEND OIIO_LINK_LIBRARIES JPEG)
    endif()
    if(PNG_FOUND)
        list(APPEND OIIO_LINK_LIBRARIES PNG)
    endif()
    if(FreeType_FOUND)
        list(APPEND OIIO_LINK_LIBRARIES FreeType)
    endif()
    if(HDF5_FOUND)
        list(APPEND OIIO_LINK_LIBRARIES HDF5)
    endif()
    if(OpenCV_FOUND)
        list(APPEND OIIO_LINK_LIBRARIES OpenCV)
    endif()
    if(TBB_FOUND)
        list(APPEND OIIO_LINK_LIBRARIES TBB)
    endif()
    if(OpenVDB_FOUND)
        list(APPEND OIIO_LINK_LIBRARIES OpenVDB)
    endif()
    if(Ptex_FOUND)
        list(APPEND OIIO_LINK_LIBRARIES Ptex)
    endif()
    set_target_properties(OpenImageIO::OpenImageIO PROPERTIES
        IMPORTED_LOCATION "${OIIO_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "${OIIO_COMPILE_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${OIIO_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${OIIO_LINK_LIBRARIES}")
endif()
if(OpenImageIO_FOUND AND NOT TARGET OpenImageIO)
    add_library(OpenImageIO INTERFACE)
    target_link_libraries(OpenImageIO INTERFACE OpenImageIO::OpenImageIO)
endif()

