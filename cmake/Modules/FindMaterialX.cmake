# Find the MaterialX library.
#
# This module defines the following variables:
#
# * MaterialX_FOUND
# * MaterialX_INCLUDE_DIRS
# * MaterialX_LIBRARIES
#
# This module defines the following imported targets:
#
# * MaterialX::MaterialX
#
# This module defines the following interfaces:
#
# * MaterialX

find_path(MaterialX_INCLUDE_DIR NAMES MaterialXCore/Util.h)
set(MaterialX_INCLUDE_DIRS
    ${MaterialX_INCLUDE_DIR})

find_library(MaterialX_Core_LIBRARY NAMES MaterialXCore)
find_library(MaterialX_Format_LIBRARY NAMES MaterialXFormat)
find_library(MaterialX_GenShader_LIBRARY NAMES MaterialXGenShader)
find_library(MaterialX_GenGlsl_LIBRARY NAMES MaterialXGenGlsl)
find_library(MaterialX_GenOsl_LIBRARY NAMES MaterialXGenOsl)
find_library(MaterialX_Render_LIBRARY NAMES MaterialXRender)
find_library(MaterialX_RenderHw_LIBRARY NAMES MaterialXRenderHw)
find_library(MaterialX_RenderGlsl_LIBRARY NAMES MaterialXRenderGlsl)
find_library(MaterialX_RenderOsl_LIBRARY NAMES MaterialXRenderOsl)
set(MaterialX_LIBRARIES
    ${MaterialX_GenGlsl_LIBRARY}
    ${MaterialX_GenOsl_LIBRARY}
    ${MaterialX_GenShader_LIBRARY}
    ${MaterialX_RenderGlsl_LIBRARY}
    ${MaterialX_RenderOsl_LIBRARY}
    ${MaterialX_RenderHw_LIBRARY}
    ${MaterialX_Render_LIBRARY}
    ${MaterialX_Format_LIBRARY}
    ${MaterialX_Core_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    MaterialX
    REQUIRED_VARS MaterialX_INCLUDE_DIR MaterialX_Core_LIBRARY)
mark_as_advanced(MaterialX_INCLUDE_DIR MaterialX_Core_LIBRARY)

if(MaterialX_FOUND AND NOT TARGET MaterialX::MaterialX)
    add_library(MaterialX::MaterialX UNKNOWN IMPORTED)
    set(MaterialX_LINK_LIBRARIES
        ${MaterialX_GenOsl_LIBRARY}
        ${MaterialX_GenShader_LIBRARY}
        ${MaterialX_RenderGlsl_LIBRARY}
        ${MaterialX_RenderOsl_LIBRARY}
        ${MaterialX_RenderHw_LIBRARY}
        ${MaterialX_Render_LIBRARY}
        ${MaterialX_Format_LIBRARY}
        ${MaterialX_Core_LIBRARY})
    set_target_properties(MaterialX::MaterialX PROPERTIES
        IMPORTED_LOCATION "${MaterialX_GenGlsl_LIBRARY}"
        INTERFACE_COMPILE_DEFINITIONS "MaterialX_FOUND"
        INTERFACE_INCLUDE_DIRECTORIES "${MaterialX_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${MaterialX_LINK_LIBRARIES}")
endif()
if(MaterialX_FOUND AND NOT TARGET MaterialX)
    add_library(MaterialX INTERFACE)
    target_link_libraries(MaterialX INTERFACE MaterialX::MaterialX)
endif()
