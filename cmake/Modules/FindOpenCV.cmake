# Find the OpenCV library.
#
# This module defines the following variables:
#
# * OpenCV_FOUND
# * OpenCV_INCLUDE_DIRS
# * OpenCV_INCLUDES
# * OpenCV_LIBRARIES
#
# This module defines the following imported targets:
#
# * OpenCV::calib3d
# * OpenCV::core
# * OpenCV::dnn
# * OpenCV::features2d
# * OpenCV::flann
# * OpenCV::gapi
# * OpenCV::imgproc
# * OpenCV::ml
# * OpenCV::objdetect
# * OpenCV::photo
# * OpenCV::stitching
# * OpenCV::videoio
#
# This module defines the following interfaces:
#
# * OpenCV

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)

find_path(OpenCV_INCLUDE_DIR NAMES opencv2/cvconfig.h PATH_SUFFIXES opencv4)
set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIR})
set(OpenCV_INCLUDES ${OpenCV_INCLUDE_DIR})

if(WIN32)
    set(OpenCV_VERSION 450)
    set(OpenCV_LIBRARY_SUFFIXES x64/vc16/lib x64/vc16/staticlib)
    if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
        set(OpenCV_LIBRARY_SUFFIX d)
    endif()
endif()
find_library(OpenCV_calib3d_LIBRARY
    NAMES opencv_calib3d${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_core_LIBRARY
    NAMES opencv_core${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_dnn_LIBRARY
    NAMES opencv_dnn${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_features2d_LIBRARY
    NAMES opencv_features2d${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_flann_LIBRARY
    NAMES opencv_flann${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_gapi_LIBRARY
    NAMES opencv_gapi${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_imgproc_LIBRARY
    NAMES opencv_imgproc${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_ml_LIBRARY
    NAMES opencv_ml${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_objdetect_LIBRARY
    NAMES opencv_objdetect${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_photo_LIBRARY
    NAMES opencv_photo${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_stitching_LIBRARY
    NAMES opencv_stitching${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
find_library(OpenCV_videoio_LIBRARY
    NAMES opencv_videoio${OpenCV_VERSION}${OpenCV_LIBRARY_SUFFIX}
    PATH_SUFFIXES ${OpenCV_LIBRARY_SUFFIXES})
set(OpenCV_LIBRARIES
    ${OpenCV_calib3d_LIBRARY}
    ${OpenCV_dnn_LIBRARY}
    ${OpenCV_features2d_LIBRARY}
    ${OpenCV_flann_LIBRARY}
    ${OpenCV_gapi_LIBRARY}
    ${OpenCV_imgproc_LIBRARY}
    ${OpenCV_ml_LIBRARY}
    ${OpenCV_objdetect_LIBRARY}
    ${OpenCV_photo_LIBRARY}
    ${OpenCV_stitching_LIBRARY}
    ${OpenCV_videoio_LIBRARY}
    ${OpenCV_core_LIBRARY}
    ${PNG_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    OpenCV
    REQUIRED_VARS
        OpenCV_INCLUDE_DIR
        OpenCV_calib3d_LIBRARY
        OpenCV_core_LIBRARY
        OpenCV_dnn_LIBRARY
        OpenCV_features2d_LIBRARY
        OpenCV_flann_LIBRARY
        OpenCV_gapi_LIBRARY
        OpenCV_imgproc_LIBRARY
        OpenCV_ml_LIBRARY
        OpenCV_objdetect_LIBRARY
        OpenCV_photo_LIBRARY
        OpenCV_stitching_LIBRARY
        OpenCV_videoio_LIBRARY)
mark_as_advanced(
    OpenCV_INCLUDE_DIR
    OpenCV_calib3d_LIBRARY
    OpenCV_core_LIBRARY
    OpenCV_dnn_LIBRARY
    OpenCV_features2d_LIBRARY
    OpenCV_flann_LIBRARY
    OpenCV_gapi_LIBRARY
    OpenCV_imgproc_LIBRARY
    OpenCV_ml_LIBRARY
    OpenCV_objdetect_LIBRARY
    OpenCV_photo_LIBRARY
    OpenCV_stitching_LIBRARY
    OpenCV_videoio_LIBRARY)

if(OpenCV_FOUND AND NOT TARGET OpenCV::core)
    add_library(OpenCV::core UNKNOWN IMPORTED)
    set_target_properties(OpenCV::core PROPERTIES
        IMPORTED_LOCATION "${OpenCV_core_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS OpenCV_FOUND
        INTERFACE_INCLUDE_DIRECTORIES "${OpenCV_INCLUDE_DIR}")
    set_property(TARGET OpenCV::core APPEND PROPERTY INTERFACE_LINK_LIBRARIES PNG ZLIB ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::calib3d)
    add_library(OpenCV::calib3d UNKNOWN IMPORTED)
    set_target_properties(OpenCV::calib3d PROPERTIES
        IMPORTED_LOCATION "${OpenCV_calib3d_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::imgproc;OpenCV::features2d;OpenCV::flann")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::dnn)
    add_library(OpenCV::dnn UNKNOWN IMPORTED)
    set_target_properties(OpenCV::dnn PROPERTIES
        IMPORTED_LOCATION "${OpenCV_dnn_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::core")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::features2d)
    add_library(OpenCV::features2d UNKNOWN IMPORTED)
    set_target_properties(OpenCV::features2d PROPERTIES
        IMPORTED_LOCATION "${OpenCV_features2d_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::imgproc")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::flann)
    add_library(OpenCV::flann UNKNOWN IMPORTED)
    set_target_properties(OpenCV::flann PROPERTIES
        IMPORTED_LOCATION "${OpenCV_flann_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::core")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::gapi)
    add_library(OpenCV::gapi UNKNOWN IMPORTED)
    set_target_properties(OpenCV::gapi PROPERTIES
        IMPORTED_LOCATION "${OpenCV_gapi_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::core")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::imgproc)
    add_library(OpenCV::imgproc UNKNOWN IMPORTED)
    set_target_properties(OpenCV::imgproc PROPERTIES
        IMPORTED_LOCATION "${OpenCV_imgproc_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::core")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::ml)
    add_library(OpenCV::ml UNKNOWN IMPORTED)
    set_target_properties(OpenCV::ml PROPERTIES
        IMPORTED_LOCATION "${OpenCV_ml_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::core")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::objdetect)
    add_library(OpenCV::objdetect UNKNOWN IMPORTED)
    set_target_properties(OpenCV::objdetect PROPERTIES
        IMPORTED_LOCATION "${OpenCV_objdetect_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::imgproc;OpenCV::calib3d")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::photo)
    add_library(OpenCV::photo UNKNOWN IMPORTED)
    set_target_properties(OpenCV::photo PROPERTIES
        IMPORTED_LOCATION "${OpenCV_photo_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::imgproc")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::stitching)
    add_library(OpenCV::stitching UNKNOWN IMPORTED)
    set_target_properties(OpenCV::stitching PROPERTIES
        IMPORTED_LOCATION "${OpenCV_stitching_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::imgproc;OpenCV::features2d;OpenCV::calib3d;OpenCV::flann")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV::videoio)
    add_library(OpenCV::videoio UNKNOWN IMPORTED)
    set_target_properties(OpenCV::videoio PROPERTIES
        IMPORTED_LOCATION "${OpenCV_videoio_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "OpenCV::core")
endif()
if(OpenCV_FOUND AND NOT TARGET OpenCV)
    add_library(OpenCV INTERFACE)
    target_link_libraries(OpenCV INTERFACE OpenCV::core)
    target_link_libraries(OpenCV INTERFACE OpenCV::calib3d)
    target_link_libraries(OpenCV INTERFACE OpenCV::dnn)
    target_link_libraries(OpenCV INTERFACE OpenCV::features2d)
    target_link_libraries(OpenCV INTERFACE OpenCV::flann)
    target_link_libraries(OpenCV INTERFACE OpenCV::gapi)
    target_link_libraries(OpenCV INTERFACE OpenCV::imgproc)
    target_link_libraries(OpenCV INTERFACE OpenCV::ml)
    target_link_libraries(OpenCV INTERFACE OpenCV::objdetect)
    target_link_libraries(OpenCV INTERFACE OpenCV::photo)
    target_link_libraries(OpenCV INTERFACE OpenCV::stitching)
    target_link_libraries(OpenCV INTERFACE OpenCV::videoio)
endif()

