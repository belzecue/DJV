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

find_library(embree_LIBRARY NAMES embree3)
find_library(embree_avx_LIBRARY NAMES embree_avx)
find_library(embree_avx2_LIBRARY NAMES embree_avx2)
find_library(embree_avx512skx_LIBRARY NAMES embree_avx512skx)
find_library(embree_sse42_LIBRARY NAMES embree_sse42)
find_library(embree_tasking_LIBRARY NAMES tasking)
find_library(embree_lexers_LIBRARY NAMES lexers)
find_library(embree_simd_LIBRARY NAMES simd)
find_library(embree_math_LIBRARY NAMES math)
find_library(embree_sys_LIBRARY NAMES sys)
set(embree_LIBRARIES ${embree_LIBRARY})
if(embree_avx_LIBRARY)
    set(embree_LIBRARIES ${embree_LIBRARIES} ${embree_avx_LIBRARY})
endif()
if(embree_avx2_LIBRARY)
    set(embree_LIBRARIES ${embree_LIBRARIES} ${embree_avx2_LIBRARY})
endif()
if(embree_avx512skx_LIBRARY)
    set(embree_LIBRARIES ${embree_LIBRARIES} ${embree_avx512skx_LIBRARY})
endif()
if(embree_sse42_LIBRARY)
    set(embree_LIBRARIES ${embree_LIBRARIES} ${embree_sse42_LIBRARY})
endif()
set(embree_LIBRARIES
    ${embree_LIBRARIES}
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
        embree_LIBRARY
        embree_tasking_LIBRARY
        embree_lexers_LIBRARY
        embree_simd_LIBRARY
        embree_math_LIBRARY
        embree_sys_LIBRARY)
mark_as_advanced(
    embree_INCLUDE_DIR
    embree_LIBRARY
    embree_tasking_LIBRARY
    embree_lexers_LIBRARY
    embree_simd_LIBRARY
    embree_math_LIBRARY
    embree_sys_LIBRARY
    embree_avx_LIBRARY
    embree_avx2_LIBRARY
    embree_avx512skx_LIBRARY
    embree_sse42_LIBRARY)

if(embree_FOUND AND NOT TARGET embree::embree)
    add_library(embree::embree UNKNOWN IMPORTED)
    set(embree_LINK_LIBRARIES
        ${embree_sys_LIBRARY}
        ${embree_math_LIBRARY}
        ${embree_simd_LIBRARY}
        ${embree_lexers_LIBRARY}
        ${embree_tasking_LIBRARY})
    if(embree_avx_LIBRARY)
        set(embree_LINK_LIBRARIES ${embree_LINK_LIBRARIES} ${embree_avx_LIBRARY})
    endif()
    if(embree_avx2_LIBRARY)
        set(embree_LINK_LIBRARIES ${embree_LINK_LIBRARIES} ${embree_avx2_LIBRARY})
    endif()
    if(embree_avx512skx_LIBRARY)
        set(embree_LINK_LIBRARIES ${embree_LINK_LIBRARIES} ${embree_avx512skx_LIBRARY})
    endif()
    if(embree_sse42_LIBRARY)
        set(embree_LINK_LIBRARIES ${embree_LINK_LIBRARIES} ${embree_sse42_LIBRARY})
    endif()
    set(embree_LINK_LIBRARIES
        ${embree_LINK_LIBRARIES}
        TBB
        ${CMAKE_THREAD_LIBS_INIT})
    set_target_properties(embree::embree PROPERTIES
        IMPORTED_LOCATION "${embree_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "embree_FOUND"
        INTERFACE_INCLUDE_DIRECTORIES "${embree_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${embree_LINK_LIBRARIES}")
endif()
if(embree_FOUND AND NOT TARGET embree)
    add_library(embree INTERFACE)
    target_link_libraries(embree INTERFACE embree::embree)
endif()
