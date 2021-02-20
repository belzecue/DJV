# Find the USD library.
#
# This module defines the following variables:
#
# * USD_FOUND
# * USD_DEFINITIONS
# * USD_INCLUDE_DIRS
# * USD_LIBRARIES
#
# This module defines the following imported targets:
#
# * USD::USD
#
# This module defines the following interfaces:
#
# * USD

find_package(TBB REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options filesystem iostreams)
find_package(OpenSubdiv REQUIRED)
find_package(OpenEXR REQUIRED)
find_package(Ptex REQUIRED)
find_package(OpenColorIO REQUIRED)
find_package(OpenVDB REQUIRED)
find_package(OpenImageIO REQUIRED)
find_package(Alembic REQUIRED)
find_package(MaterialX REQUIRED)
find_package(embree REQUIRED)

find_path(USD_INCLUDE_DIR NAMES pxr/pxr.h)
set(USD_INCLUDE_DIRS
    ${embree_INCLUDE_DIR}
    ${MaterialX_INCLUDE_DIR}
    ${Alembic_INCLUDE_DIR}
    ${OpenImageIO_INCLUDE_DIR}
    ${OpenVDB_INCLUDE_DIR}
    ${OpenColorIO_INCLUDE_DIR}
    ${Ptex_INCLUDE_DIRS}
    ${OpenEXR_INCLUDE_DIRS}
    ${OpenSubdiv_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${TBB_INCLUDE_DIRS})

find_library(USD_arch_LIBRARY NAMES arch)
find_library(USD_tf_LIBRARY NAMES tf)
find_library(USD_gf_LIBRARY NAMES gf)
find_library(USD_js_LIBRARY NAMES js)
find_library(USD_trace_LIBRARY NAMES trace)
find_library(USD_work_LIBRARY NAMES work)
find_library(USD_plug_LIBRARY NAMES plug)
find_library(USD_vt_LIBRARY NAMES vt)
find_library(USD_ar_LIBRARY NAMES ar)
find_library(USD_kind_LIBRARY NAMES kind)
find_library(USD_sdf_LIBRARY NAMES sdf)
find_library(USD_ndr_LIBRARY NAMES ndr)
find_library(USD_sdr_LIBRARY NAMES sdr)
find_library(USD_pcp_LIBRARY NAMES pcp)
find_library(USD_LIBRARY NAMES usd)
find_library(USD_Geom_LIBRARY NAMES usdGeom)
find_library(USD_Vol_LIBRARY NAMES usdVol)
find_library(USD_Media_LIBRARY NAMES usdMedia)
find_library(USD_Shade_LIBRARY NAMES usdShade)
find_library(USD_Lux_LIBRARY NAMES usdLux)
find_library(USD_Render_LIBRARY NAMES usdRender)
find_library(USD_Hydra_LIBRARY NAMES usdHydra)
find_library(USD_Ri_LIBRARY NAMES usdRi)
find_library(USD_Skel_LIBRARY NAMES usdSkel)
find_library(USD_UI_LIBRARY NAMES usdUI)
find_library(USD_Utils_LIBRARY NAMES usdUtils)
find_library(USD_garch_LIBRARY NAMES garch)
find_library(USD_hf_LIBRARY NAMES hf)
find_library(USD_hio_LIBRARY NAMES hio)
find_library(USD_cameraUtil_LIBRARY NAMES cameraUtil)
find_library(USD_pxOsd_LIBRARY NAMES pxOsd)
find_library(USD_glf_LIBRARY NAMES glf)
find_library(USD_hgi_LIBRARY NAMES hgi)
find_library(USD_hgiGL_LIBRARY NAMES hgiGL)
find_library(USD_hgiInterop_LIBRARY NAMES hgiInterop)
find_library(USD_hd_LIBRARY NAMES hd)
find_library(USD_hdSt_LIBRARY NAMES hdSt)
find_library(USD_hdx_LIBRARY NAMES hdx)
find_library(USD_Imaging_LIBRARY NAMES usdImaging)
find_library(USD_ImagingGL_LIBRARY NAMES usdImagingGL)
find_library(USD_RiImaging_LIBRARY NAMES usdRiImaging)
find_library(USD_SkelImaging_LIBRARY NAMES usdSkelImaging)
find_library(USD_VolImaging_LIBRARY NAMES usdVolImaging)
find_library(USD_AppUtils_LIBRARY NAMES usdAppUtils)
set(USD_LIBRARIES
    ${USD_AppUtils_LIBRARY}
    ${USD_VolImaging_LIBRARY}
    ${USD_SkelImaging_LIBRARY}
    ${USD_RiImaging_LIBRARY}
    ${USD_ImagingGL_LIBRARY}
    ${USD_Imaging_LIBRARY}
    ${USD_hdx_LIBRARY}
    ${USD_hdSt_LIBRARY}
    ${USD_hd_LIBRARY}
    ${USD_hgiInterop_LIBRARY}
    ${USD_hgiGL_LIBRARY}
    ${USD_hgi_LIBRARY}
    ${USD_glf_LIBRARY}
    ${USD_pxOsd_LIBRARY}
    ${USD_cameraUtil_LIBRARY}
    ${USD_hio_LIBRARY}
    ${USD_hf_LIBRARY}
    ${USD_garch_LIBRARY}
    ${USD_Utils_LIBRARY}
    ${USD_UI_LIBRARY}
    ${USD_Skel_LIBRARY}
    ${USD_Ri_LIBRARY}
    ${USD_Hydra_LIBRARY}
    ${USD_Render_LIBRARY}
    ${USD_Lux_LIBRARY}
    ${USD_Shade_LIBRARY}
    ${USD_Media_LIBRARY}
    ${USD_Vol_LIBRARY}
    ${USD_Geom_LIBRARY}
    ${USD_LIBRARY}
    ${USD_pcp_LIBRARY}
    ${USD_sdr_LIBRARY}
    ${USD_ndr_LIBRARY}
    ${USD_sdf_LIBRARY}
    ${USD_kind_LIBRARY}
    ${USD_ar_LIBRARY}
    ${USD_vt_LIBRARY}
    ${USD_plug_LIBRARY}
    ${USD_work_LIBRARY}
    ${USD_trace_LIBRARY}
    ${USD_js_LIBRARY}
    ${USD_gf_LIBRARY}
    ${USD_tf_LIBRARY}
    ${USD_arch_LIBRARY}
    ${MaterialX_LIBRARIES}
    ${Alembic_LIBRARIES}
    ${OpenImageIO_LIBRARIES}
    ${OpenVDB_LIBRARIES}
    ${OCIO_LIBRARIES}
    ${Ptex_LIBRARIES}
    ${OpenEXR_LIBRARIES}
    ${OpenSubdiv_LIBRARIES}
    ${Boost_LIBRARIES}
    ${TBB_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    USD
    REQUIRED_VARS USD_INCLUDE_DIR USD_LIBRARY)
mark_as_advanced(USD_INCLUDE_DIR USD_LIBRARY)

set(USD_DEFINITIONS
    -DUSD_FOUND
    ${OpenVDB_DEFINITIONS}
    ${OpenColorIO_DEFINITIONS})
set(USD_COMPILE_DEFINITIONS
    USD_FOUND
    ${OpenVDB_COMPILE_DEFINITIONS}
    ${OpenColorIO_COMPILE_DEFINITIONS})
if(NOT BUILD_SHARED_LIBS)
    set(USD_DEFINITIONS ${USD_DEFINITIONS} -DPXR_STATIC)
    set(USD_COMPILE_DEFINITIONS ${USD_COMPILE_DEFINITIONS} PXR_STATIC)
endif()

if(USD_FOUND AND NOT TARGET USD::USD)
    add_library(USD::USD UNKNOWN IMPORTED)
    set(USD_LINK_LIBRARIES
        ${USD_VolImaging_LIBRARY}
        ${USD_SkelImaging_LIBRARY}
        ${USD_RiImaging_LIBRARY}
        ${USD_ImagingGL_LIBRARY}
        ${USD_Imaging_LIBRARY}
        ${USD_hdx_LIBRARY}
        ${USD_hdSt_LIBRARY}
        ${USD_hd_LIBRARY}
        ${USD_hgiInterop_LIBRARY}
        ${USD_hgiGL_LIBRARY}
        ${USD_hgi_LIBRARY}
        ${USD_glf_LIBRARY}
        ${USD_pxOsd_LIBRARY}
        ${USD_cameraUtil_LIBRARY}
        ${USD_hio_LIBRARY}
        ${USD_hf_LIBRARY}
        ${USD_garch_LIBRARY}
        ${USD_Utils_LIBRARY}
        ${USD_UI_LIBRARY}
        ${USD_Skel_LIBRARY}
        ${USD_Ri_LIBRARY}
        ${USD_Hydra_LIBRARY}
        ${USD_Render_LIBRARY}
        ${USD_Lux_LIBRARY}
        ${USD_Shade_LIBRARY}
        ${USD_Media_LIBRARY}
        ${USD_Vol_LIBRARY}
        ${USD_Geom_LIBRARY}
        ${USD_LIBRARY}
        ${USD_pcp_LIBRARY}
        ${USD_sdr_LIBRARY}
        ${USD_ndr_LIBRARY}
        ${USD_sdf_LIBRARY}
        ${USD_kind_LIBRARY}
        ${USD_ar_LIBRARY}
        ${USD_vt_LIBRARY}
        ${USD_plug_LIBRARY}
        ${USD_work_LIBRARY}
        ${USD_trace_LIBRARY}
        ${USD_js_LIBRARY}
        ${USD_gf_LIBRARY}
        ${USD_tf_LIBRARY}
        ${USD_arch_LIBRARY}
        embree
        MaterialX
        Alembic
        OpenImageIO
        OpenVDB
        OCIO
        Ptex
        OpenEXR
        OpenSubdiv
        Boost::boost
        Boost::program_options
        Boost::filesystem
        Boost::iostreams
        TBB)
    set_target_properties(USD::USD PROPERTIES
        IMPORTED_LOCATION "${USD_AppUtils_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "${USD_COMPILE_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${USD_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${USD_LINK_LIBRARIES}")
endif()
if(USD_FOUND AND NOT TARGET USD)
    add_library(USD INTERFACE)
    target_link_libraries(USD INTERFACE USD::USD)
endif()
