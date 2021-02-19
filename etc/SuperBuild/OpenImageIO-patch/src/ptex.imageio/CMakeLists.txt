# Copyright 2008-present Contributors to the OpenImageIO project.
# SPDX-License-Identifier: BSD-3-Clause
# https://github.com/OpenImageIO/oiio/blob/master/LICENSE.md

if (Ptex_FOUND)
    add_oiio_plugin (ptexinput.cpp
                     INCLUDE_DIRS ${Ptex_INCLUDE_DIRS}
                     LINK_LIBRARIES ${Ptex_LIBRARIES}
                     DEFINITIONS "-DUSE_PTEX;${Ptex_COMPILE_DEFINITIONS}")
endif ()
