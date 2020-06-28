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

## Catch CMake support
include("${CMAKE_BINARY_DIR}/_deps/catch2-src/contrib/Catch.cmake")

## Create project
set(TESTED_PROJECT_NAME ${PROJECT_NAME})
project(${TESTED_PROJECT_NAME}_Test CXX)

## Create target
add_executable(${TESTED_PROJECT_NAME}_test
               test.main.cpp
               test.bad_ip_format.cpp
               test.ip_address.cpp
               test.bad_word.cpp
               test.calc_len.cpp
               test.command.cpp
               test.sentence.cpp test.attribute.cpp test.query.cpp test.bad_socket.cpp test.split.cpp)

## Link dependencies
target_link_libraries(${TESTED_PROJECT_NAME}_test
                      PRIVATE ${${TESTED_PROJECT_NAME}_NAMESPACE}
                      PRIVATE Catch2::Catch2
                      PRIVATE fmt::fmt
                      )

## Set warnings of MikroTikApi
target_compile_options(${TESTED_PROJECT_NAME}_test
                       PRIVATE ${${TESTED_PROJECT_NAME}_WARNINGS}
                       PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>,/EHsc,-Wall>
                       )

target_include_directories(${TESTED_PROJECT_NAME}_test PRIVATE
                           ${CMAKE_SOURCE_DIR}/src
                           )

## Require C++17
target_compile_features(${TESTED_PROJECT_NAME}_test PUBLIC cxx_std_17)
set_target_properties(${TESTED_PROJECT_NAME}_test PROPERTIES
                      CXX_STANDARD 17)

## Register Catch tests to CTest
catch_discover_tests(${TESTED_PROJECT_NAME}_test)

## Copy shared objects
if (${TESTED_PROJECT_NAME}_BUILD_SHARED)
    add_custom_command(TARGET ${TESTED_PROJECT_NAME}_test POST_BUILD
                       COMMENT "Copying ${TESTED_PROJECT_NAME} shared objects"
                       COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${${TESTED_PROJECT_NAME}_TARGET}>" "${PROJECT_BINARY_DIR}")
endif ()
