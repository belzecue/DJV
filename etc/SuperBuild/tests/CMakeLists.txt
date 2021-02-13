cmake_minimum_required(VERSION 3.18)

project(DJV-third-party-tests)

enable_testing()

foreach(DEP ${DJV_THIRD_PARTY_DEPS})
    find_package(${DEP})
    if(${DEP}_FOUND)
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${DEP}Test)
            add_subdirectory(${DEP}Test)
        else()
            message(WARNING "${DEP}Test not found")
        endif()
    endif()
endforeach()

