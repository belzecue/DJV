# Find the OpenImageIO library.
#
# This module defines the following variables:
#
# * OpenImageIO_FOUND
# * OpenImageIO_INCLUDE_DIRS
# * OpenImageIO_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenImageIO::OpenImageIO
#
# This module defines the following interfaces:
#
# * OpenImageIO

find_package(ZLIB REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem thread iostreams)
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

find_path(OpenImageIO_INCLUDE_DIR NAMES OpenImageIO/imageio.h)
set(OpenImageIO_INCLUDE_DIRS
    ${OpenImageIO_INCLUDE_DIR}
    ${OpenEXR_INCLUDE_DIRS}
    ${TIFF_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS})
if(JPEG_FOUND)
    set(OpenImageIO_INCLUDE_DIRS ${OpenImageIO_INCLUDE_DIRS} ${JPEG_INCLUDE_DIRS})
endif()
if(PNG_FOUND)
    set(OpenImageIO_INCLUDE_DIRS ${OpenImageIO_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
endif()
if(FreeType_FOUND)
    set(OpenImageIO_INCLUDE_DIRS ${OpenImageIO_INCLUDE_DIRS} ${FreeType_INCLUDE_DIRS})
endif()
if(HDF5_FOUND)
    set(OpenImageIO_INCLUDE_DIRS ${OpenImageIO_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})
endif()
if(OpenCV_FOUND)
    set(OpenImageIO_INCLUDE_DIRS ${OpenImageIO_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
endif()
if(TBB_FOUND)
    set(OpenImageIO_INCLUDE_DIRS ${OpenImageIO_INCLUDE_DIRS} ${TBB_INCLUDE_DIRS})
endif()
if(OpenVDB_FOUND)
    set(OpenImageIO_INCLUDE_DIRS ${OpenImageIO_INCLUDE_DIRS} ${OpenVDB_INCLUDE_DIRS})
endif()
if(Ptex_FOUND)
    set(OpenImageIO_INCLUDE_DIRS ${OpenImageIO_INCLUDE_DIRS} ${Ptex_INCLUDE_DIRS})
endif()

find_library(OpenImageIO_LIBRARY NAMES OpenImageIO)
set(OpenImageIO_LIBRARIES
    ${OpenImageIO_LIBRARY}
    ${OpenEXR_LIBRARIES}
    ${TIFF_LIBRARIES}
    ${Boost_LIBRARIES}
    ${ZLIB_LIBRARIES})
if(JPEG_FOUND)
    set(OpenImageIO_LIBRARIES ${OpenImageIO_LIBRARIES} ${JPEG_LIBRARIES})
endif()
if(PNG_FOUND)
    set(OpenImageIO_LIBRARIES ${OpenImageIO_LIBRARIES} ${PNG_LIBRARIES})
endif()
if(FreeType_FOUND)
    set(OpenImageIO_LIBRARIES ${OpenImageIO_LIBRARIES} ${FreeType_LIBRARIES})
endif()
if(HDF5_FOUND)
    set(OpenImageIO_LIBRARIES ${OpenImageIO_LIBRARIES} ${HDF5_LIBRARIES})
endif()
if(OpenCV_FOUND)
    set(OpenImageIO_LIBRARIES ${OpenImageIO_LIBRARIES} ${OpenCV_LIBRARIES})
endif()
if(TBB_FOUND)
    set(OpenImageIO_LIBRARIES ${OpenImageIO_LIBRARIES} ${TBB_LIBRARIES})
endif()
if(OpenVDB_FOUND)
    set(OpenImageIO_LIBRARIES ${OpenImageIO_LIBRARIES} ${OpenVDB_LIBRARIES})
endif()
if(Ptex_FOUND)
    set(OpenImageIO_LIBRARIES ${OpenImageIO_LIBRARIES} ${Ptex_LIBRARIES})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenImageIO
    REQUIRED_VARS OpenImageIO_INCLUDE_DIR OpenImageIO_LIBRARY)
mark_as_advanced(OpenImageIO_INCLUDE_DIR OpenImageIO_LIBRARY)

if(OpenImageIO_FOUND AND NOT TARGET OpenImageIO::OpenImageIO)
    add_library(OpenImageIO::OpenImageIO UNKNOWN IMPORTED)
    set(OpenImageIO_LINK_LIBRARIES
        ${OpenEXR_LIBRARIES}
        ${TIFF_LIBRARIES}
        ${ZLIB_LIBRARIES}
        Boost::boost
        Boost::filesystem
        Boost::thread
        Boost::iostreams)
    if(JPEG_FOUND)
        set(OpenImageIO_LINK_LIBRARIES ${OpenImageIO_LINK_LIBRARIES} JPEG)
    endif()
    if(PNG_FOUND)
        set(OpenImageIO_LINK_LIBRARIES ${OpenImageIO_LINK_LIBRARIES} PNG)
    endif()
    if(FreeType_FOUND)
        set(OpenImageIO_LINK_LIBRARIES ${OpenImageIO_LINK_LIBRARIES} FreeType)
    endif()
    if(HDF5_FOUND)
        set(OpenImageIO_LINK_LIBRARIES ${OpenImageIO_LINK_LIBRARIES} HDF5)
    endif()
    if(OpenCV_FOUND)
        set(OpenImageIO_LINK_LIBRARIES ${OpenImageIO_LINK_LIBRARIES} OpenCV)
    endif()
    if(TBB_FOUND)
        set(OpenImageIO_LINK_LIBRARIES ${OpenImageIO_LINK_LIBRARIES} TBB)
    endif()
    if(OpenVDB_FOUND)
        set(OpenImageIO_LINK_LIBRARIES ${OpenImageIO_LINK_LIBRARIES} OpenVDB)
    endif()
    if(Ptex_FOUND)
        set(OpenImageIO_LINK_LIBRARIES ${OpenImageIO_LINK_LIBRARIES} Ptex)
    endif()
    set_target_properties(OpenImageIO::OpenImageIO PROPERTIES
        IMPORTED_LOCATION "${OpenImageIO_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "OpenImageIO_FOUND"
        INTERFACE_INCLUDE_DIRECTORIES "${OpenImageIO_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${OpenImageIO_LINK_LIBRARIES}")
endif()
if(OpenImageIO_FOUND AND NOT TARGET OpenImageIO)
    add_library(OpenImageIO INTERFACE)
    target_link_libraries(OpenImageIO INTERFACE OpenImageIO::OpenImageIO)
endif()

