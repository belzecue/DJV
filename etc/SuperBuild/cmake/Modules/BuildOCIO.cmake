include(ExternalProject)

set(OCIO_ARGS
    -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_INSTALL_LIBDIR=lib
    -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_POLICY_DEFAULT_CMP0091:STRING=NEW
    -DCMAKE_FIND_FRAMEWORK=${CMAKE_FIND_FRAMEWORK}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    -DCMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -DOCIO_BUILD_APPS=OFF
    -DOCIO_BUILD_NUKE=OFF
    -DOCIO_BUILD_DOCS=OFF
    -DOCIO_BUILD_TESTS=OFF
    -DOCIO_BUILD_PYGLUE=OFF
    -DOCIO_STATIC_JNIGLUE=OFF
    -DYAML_CPP_OBJECT_LIB_EMBEDDED=ON
    -DTINYXML_OBJECT_LIB_EMBEDDED=ON)
if(BUILD_SHARED_LIBS)
    set(OCIO_ARGS ${OCIO_ARGS} -DOCIO_BUILD_SHARED=ON -DOCIO_BUILD_STATIC=OFF)
else()
    set(OCIO_ARGS ${OCIO_ARGS} -DOCIO_BUILD_SHARED=OFF -DOCIO_BUILD_STATIC=ON)
endif()
if(CMAKE_CXX_STANDARD)
    set(OCIO_ARGS ${OCIO_ARGS} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD})
endif()
cmake_host_system_information(RESULT HAS_SSE2 QUERY HAS_SSE2)
set(OCIO_ARGS ${OCIO_ARGS} -DOCIO_USE_SSE=${HAS_SSE2})
if(APPLE)
    execute_process(
        COMMAND uname -m
        RESULT_VARIABLE result
        OUTPUT_VARIABLE OCIO_OSX_NATIVE_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(OCIO_ARGS ${OCIO_ARGS} -DCMAKE_OSX_ARCHITECTURES=${OCIO_OSX_NATIVE_ARCH})
endif()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/OCIO/src/OCIO-build/ext)

ExternalProject_Add(
    OCIO
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/OCIO
    URL "http://github.com/imageworks/OpenColorIO/archive/v1.1.1.tar.gz"
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/OCIO-patch/src/core/CMakeLists.txt
        ${CMAKE_CURRENT_BINARY_DIR}/OCIO/src/OCIO/src/core/CMakeLists.txt
    CMAKE_ARGS ${OCIO_ARGS})

