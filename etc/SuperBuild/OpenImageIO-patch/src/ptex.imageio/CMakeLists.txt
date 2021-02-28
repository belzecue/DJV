# Copyright 2008-present Contributors to the OpenImageIO project.
# SPDX-License-Identifier: BSD-3-Clause
# https://github.com/OpenImageIO/oiio/blob/master/LICENSE.md

if (Ptex_FOUND)
    add_oiio_plugin (ptexinput.cpp
                     INCLUDE_DIRS ${PTEX_INCLUDE_DIRS}
                     LINK_LIBRARIES ${PTEX_LIBRARIES}
                     DEFINITIONS "-DUSE_PTEX;${PTEX_COMPILE_DEFINITIONS}")
endif ()
