# Find the embree library.
#
# This module defines the following variables:
#
# * embree_FOUND
# * embree_INCLUDE_DIRS
# * embree_LIBRARIES
#
# This module defines the following imported targets:
#
# * embree::tasking
# * embree::lexers
# * embree::simd
# * embree::math
# * embree::sys
# * embree::avx512skx
# * embree::avx2
# * embree::avx
# * embree::sse42
# * embree::embree
#
# This module defines the following interfaces:
#
# * embree

find_package(Threads REQUIRED)
find_package(ISPC REQUIRED)
find_package(TBB REQUIRED)

find_path(embree_INCLUDE_DIR NAMES embree3/rtcore.h)
set(embree_INCLUDE_DIRS
    ${embree_INCLUDE_DIR}
    ${TBB_INCLUDE_DIRS})

find_library(embree_tasking_LIBRARY NAMES tasking)
find_library(embree_lexers_LIBRARY NAMES lexers)
find_library(embree_simd_LIBRARY NAMES simd)
find_library(embree_math_LIBRARY NAMES math)
find_library(embree_sys_LIBRARY NAMES sys)
find_library(embree_avx512skx_LIBRARY NAMES embree_avx512skx)
find_library(embree_avx2_LIBRARY NAMES embree_avx2)
find_library(embree_avx_LIBRARY NAMES embree_avx)
find_library(embree_sse42_LIBRARY NAMES embree_sse42)
find_library(embree_LIBRARY NAMES embree3)
set(embree_LIBRARIES
    ${embree_LIBRARY}
    ${embree_sse42_LIBRARY}
    ${embree_avx_LIBRARY}
    ${embree_avx2_LIBRARY}
    ${embree_avx512skx_LIBRARY}
    ${embree_sys_LIBRARY}
    ${embree_math_LIBRARY}
    ${embree_simd_LIBRARY}
    ${embree_lexers_LIBRARY}
    ${embree_tasking_LIBRARY}
    ${TBB_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    embree
    REQUIRED_VARS
        embree_INCLUDE_DIR
        embree_tasking_LIBRARY
        embree_lexers_LIBRARY
        embree_simd_LIBRARY
        embree_math_LIBRARY
        embree_sys_LIBRARY
        embree_avx512skx_LIBRARY
        embree_avx2_LIBRARY
        embree_avx_LIBRARY
        embree_sse42_LIBRARY
        embree_LIBRARY)
mark_as_advanced(
    embree_INCLUDE_DIR
    embree_tasking_LIBRARY
    embree_lexers_LIBRARY
    embree_simd_LIBRARY
    embree_math_LIBRARY
    embree_sys_LIBRARY
    embree_avx512skx_LIBRARY
    embree_avx2_LIBRARY
    embree_avx_LIBRARY
    embree_sse42_LIBRARY
    embree_LIBRARY)

if(embree_FOUND AND NOT TARGET embree::embree)
    add_library(embree::embree UNKNOWN IMPORTED)
    set(embree_COMPILE_DEFINITIONS
        embree_FOUND
        EMBREE_VERSION=3.12.1
        EMBREE_VERSION_MAJOR=3
        EMBREE_VERSION_MINOR=12
        EMBREE_VERSION_PATCH=1
        EMBREE_VERSION_NOTE=""
        EMBREE_MAX_ISA=NONE
        EMBREE_ISA_SSE2=ON
        EMBREE_ISA_SSE42=ON
        EMBREE_ISA_AVX=ON 
        EMBREE_ISA_AVX2=ON
        EMBREE_ISA_AVX512KNL=OFF
        EMBREE_ISA_AVX512SKX=ON
        EMBREE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        EMBREE_ISPC_SUPPORT=ON
        EMBREE_STATIC_LIB=ON
        EMBREE_TUTORIALS=OFF
        EMBREE_RAY_MASK=OFF
        EMBREE_STAT_COUNTERS=OFF
        EMBREE_BACKFACE_CULLING=OFF
        EMBREE_FILTER_FUNCTION=ON
        EMBREE_IGNORE_INVALID_RAYS=OFF
        EMBREE_TASKING_SYSTEM=TBB
        EMBREE_COMPACT_POLYS=OFF
        EMBREE_GEOMETRY_TRIANGLE=ON
        EMBREE_GEOMETRY_QUAD=ON
        EMBREE_GEOMETRY_CURVE=ON
        EMBREE_GEOMETRY_SUBDIVISION=ON
        EMBREE_GEOMETRY_USER=ON
        EMBREE_GEOMETRY_POINT=ON
        EMBREE_RAY_PACKETS=ON
        EMBREE_MAX_INSTANCE_LEVEL_COUNT=1
        EMBREE_CURVE_SELF_INTERSECTION_AVOIDANCE_FACTOR=2.0
        EMBREE_MIN_WIDTH=OFF)
    set(embree_LINK_LIBRARIES
        ${embree_sse42_LIBRARY}
        ${embree_avx_LIBRARY}
        ${embree_avx2_LIBRARY}
        ${embree_avx512skx_LIBRARY}
        ${embree_sys_LIBRARY}
        ${embree_math_LIBRARY}
        ${embree_simd_LIBRARY}
        ${embree_lexers_LIBRARY}
        ${embree_tasking_LIBRARY}
        TBB
        ${CMAKE_THREAD_LIBS_INIT})
    set_target_properties(embree::embree PROPERTIES
        IMPORTED_LOCATION "${embree_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "${embree_COMPILE_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${embree_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${embree_LINK_LIBRARIES}")
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("embree_COMPILE_DEFINITIONS: "${embree_COMPILE_DEFINITIONS})
endif()
if(embree_FOUND AND NOT TARGET embree)
    add_library(embree INTERFACE)
    target_link_libraries(embree INTERFACE embree::embree)
endif()

