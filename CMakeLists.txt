# BSD 3-Clause License
#
# Copyright (c) 2020, bodand
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


## CMake shenanigans
cmake_minimum_required(VERSION 3.14)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
enable_language(CXX)

## Modules #####################################################################
include(utils)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(CMakeDependentOption)

## Project #####################################################################
project(MikroTikApi
        VERSION 1.1.1
        LANGUAGES CXX)

## Project configuration #######################################################

## Check if main project
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(${PROJECT_NAME}_MAIN On)
else ()
    set(${PROJECT_NAME}_MAIN Off)
endif ()

option(${PROJECT_NAME}_BUILD_TESTS
       "Build the ${PROJECT_NAME} test suite [Off as dependency]" ${${PROJECT_NAME}_MAIN})
option(${PROJECT_NAME}_BUILD_EXAMPLES
       "Build the ${PROJECT_NAME} usage examples [Off]" Off)
option(${PROJECT_NAME}_BUILD_DOCS
       "Build the ${PROJECT_NAME} documentation (Requires: 'Doxygen', 'Sphinx', and the 'breathe' and 'sphinx_rtd_theme' pip packages) [Off]" Off)
cmake_dependent_option(${PROJECT_NAME}_BUILD_SHARED
                       "Build ${PROJECT_NAME} as shared library [Off when testing]" On
                       "NOT ${PROJECT_NAME}_BUILD_TESTS" Off)
NameOption(${${PROJECT_NAME}_BUILD_SHARED} "SHARED;STATIC" ${PROJECT_NAME}_TARGET_TYPE)
message(STATUS "[${PROJECT_NAME}] Building ${${PROJECT_NAME}_TARGET_TYPE} library")

## Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

## Project level macros
set(${PROJECT_NAME}_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(${PROJECT_NAME}_HEADER_PATH "${${PROJECT_NAME}_INCLUDE_DIR}/mikrotik/api")
set(${PROJECT_NAME}_TARGET mikrotikapi)
set(${PROJECT_NAME}_NAMESPACE mikrotik::mikrotikapi)

## MikroTikAPI library target ##################################################

## Create target
add_library(${${PROJECT_NAME}_TARGET} ${${PROJECT_NAME}_TARGET_TYPE}
            src/api_handler.cpp

            src/command.cpp
            src/sentence.cpp
            src/attribute.cpp
            src/query.cpp
            src/reply.cpp

            src/ip_address.cpp
            src/calc_len.cpp
            src/split.cpp

            src/bad_ip_format.cpp
            src/bad_word.cpp
            src/bad_socket.cpp

            src/sockets.common.cpp
            src/sockets.$<IF:$<PLATFORM_ID:Windows>,winsock,posix>.cpp
            )
add_library(${${PROJECT_NAME}_NAMESPACE} ALIAS ${${PROJECT_NAME}_TARGET})

## Create shared export header
generate_export_header(${${PROJECT_NAME}_TARGET}
                       BASE_NAME MIKROTIK_API)
if (NOT ${PROJECT_NAME}_BUILD_SHARED)
    target_compile_definitions(${${PROJECT_NAME}_TARGET} PUBLIC -DMIKROTIK_API_STATIC_DEFINE)
else ()
    target_compile_options(${${PROJECT_NAME}_TARGET} PRIVATE
                           $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fvisibility=hidden>)
endif ()

## Require C++17
target_compile_features(${${PROJECT_NAME}_TARGET} PUBLIC cxx_std_17)

## Check warnings
include(warnings-${PROJECT_NAME})
target_compile_options(${${PROJECT_NAME}_TARGET} PRIVATE
                       ${${PROJECT_NAME}_WARNINGS}
                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall>
                       )

# Dependencies
include(dependencies-${PROJECT_NAME})

## Link to dependencies
target_link_libraries(${${PROJECT_NAME}_TARGET}
                      PRIVATE fmt::fmt
                      PRIVATE $<$<PLATFORM_ID:Windows>:ws2_32>
                      )

## Set target properties
target_include_directories(${${PROJECT_NAME}_TARGET} PUBLIC
                           $<INSTALL_INTERFACE:include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<BUILD_INTERFACE:${${PROJECT_NAME}_INCLUDE_DIR}>
                           )

## Optional targets ############################################################

## Optionally enable tests
if (${PROJECT_NAME}_BUILD_TESTS)
    include(CTest)
    enable_testing()

    message(STATUS "[${PROJECT_NAME}] Building tests")
    add_subdirectory(test)
endif ()

## Optionally build examples
if (${PROJECT_NAME}_BUILD_EXAMPLES)
    add_subdirectory(example)
endif ()

## Optionally build docs
if (${PROJECT_NAME}_BUILD_DOCS)
    add_subdirectory(docs)
endif ()

## Install targets #############################################################

install(TARGETS ${${PROJECT_NAME}_TARGET}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/mikrotik_api_export.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        )

install(DIRECTORY
        "${${PROJECT_NAME}_INCLUDE_DIR}/mikrotik"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.hpp")

write_basic_package_version_file(
        ${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE mikrotik::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        )

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
               "${PROJECT_NAME}Config.cmake"
               @ONLY)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        )

## CPack distribution ##########################################################

## Windows Installer GUID generator
include(WinInstallerGUID)

## CPack settings
set(CPACK_GENERATOR "TXZ")
set(CPACK_SOURCE_GENERATOR "TXZ")
# Binary packages
set(CPACK_PACKAGE_NAME "MikroTikAPI")
set(CPACK_PACKAGE_VENDOR "bodand")
set(CPACK_PACKAGE_DESCRIPTION "A Modern C++ wrapper for the MikroTik API")
set(CPACK_PACKAGE_CHECKSUM SHA3_256)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME
    "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${${PROJECT_NAME}_TARGET_TYPE}-${CMAKE_CXX_COMPILER_ID}")
# Source packages
set(CPACK_SOURCE_PACKAGE_FILE_NAME "mikrotikapi-src-${PROJECT_VERSION}")
## CPack WIX
WinInstallerGUID(UP_GUID PROD_GUID)
set(CPACK_WIX_UPGRADE_GUID "${UP_GUID}")
set(CPACK_WIX_PRODUCT_GUID "${PROD_GUID}")
set(CPACK_WIX_LICENSE_RTF "${CMAKE_SOURCE_DIR}/LICENSE.rtf")
set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY MikroTikAPI)
set(CPACK_WIX_PROPERTY_ARPHELPLINK "https://github.com/bodand/MikroTikAPI")
set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "https://mikrotikapi.rtfd.io/")
set(CPACK_WIX_PROPERTY_UPDATEINFO "https://github.com/bodand/MikroTikAPI/releases")
## Register CPack
include(CPack)
