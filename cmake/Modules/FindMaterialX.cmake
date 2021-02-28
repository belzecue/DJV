# Find the MaterialX library.
#
# This module defines the following variables:
#
# * MATERIALX_INCLUDE_DIRS
# * MATERIALX_LIBRARIES
#
# This module defines the following imported targets:
#
# * MaterialX::MaterialX
#
# This module defines the following interfaces:
#
# * MaterialX

find_path(MATERIALX_INCLUDE_DIR NAMES MaterialXCore/Util.h)
set(MATERIALX_INCLUDE_DIRS
    ${MATERIALX_INCLUDE_DIR})

find_library(MATERIALX_CORE_LIBRARY NAMES MaterialXCore)
find_library(MATERIALX_FORMAT_LIBRARY NAMES MaterialXFormat)
find_library(MATERIALX_GENSHADER_LIBRARY NAMES MaterialXGenShader)
find_library(MATERIALX_GENGLSL_LIBRARY NAMES MaterialXGenGlsl)
find_library(MATERIALX_GENOSL_LIBRARY NAMES MaterialXGenOsl)
find_library(MATERIALX_RENDER_LIBRARY NAMES MaterialXRender)
find_library(MATERIALX_RENDERHW_LIBRARY NAMES MaterialXRenderHw)
find_library(MATERIALX_RENDERGLSL_LIBRARY NAMES MaterialXRenderGlsl)
find_library(MATERIALX_RENDEROSL_LIBRARY NAMES MaterialXRenderOsl)
set(MATERIALX_LIBRARIES
    ${MATERIALX_GENGLSL_LIBRARY}
    ${MATERIALX_GENOSL_LIBRARY}
    ${MATERIALX_GENSHADER_LIBRARY}
    ${MATERIALX_RENDERGLSL_LIBRARY}
    ${MATERIALX_RENDEROSL_LIBRARY}
    ${MATERIALX_RENDERHW_LIBRARY}
    ${MATERIALX_RENDER_LIBRARY}
    ${MATERIALX_FORMAT_LIBRARY}
    ${MATERIALX_CORE_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    MaterialX
    REQUIRED_VARS MATERIALX_INCLUDE_DIR MATERIALX_CORE_LIBRARY)
mark_as_advanced(MATERIALX_INCLUDE_DIR MATERIALX_CORE_LIBRARY)

if(MaterialX_FOUND AND NOT TARGET MaterialX::MaterialX)
    add_library(MaterialX::MaterialX UNKNOWN IMPORTED)
    set(MATERIALX_LINK_LIBRARIES
        ${MATERIALX_GENOSL_LIBRARY}
        ${MATERIALX_GENSHADER_LIBRARY}
        ${MATERIALX_RENDERGLSL_LIBRARY}
        ${MATERIALX_RENDEROSL_LIBRARY}
        ${MATERIALX_RENDERHW_LIBRARY}
        ${MATERIALX_RENDER_LIBRARY}
        ${MATERIALX_FORMAT_LIBRARY}
        ${MATERIALX_CORE_LIBRARY})
    set_target_properties(MaterialX::MaterialX PROPERTIES
        IMPORTED_LOCATION "${MATERIALX_GENGLSL_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "MaterialX_FOUND"
        INTERFACE_INCLUDE_DIRECTORIES "${MATERIALX_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${MATERIALX_LINK_LIBRARIES}")
endif()
if(MaterialX_FOUND AND NOT TARGET MaterialX)
    add_library(MaterialX INTERFACE)
    target_link_libraries(MaterialX INTERFACE MaterialX::MaterialX)
endif()
