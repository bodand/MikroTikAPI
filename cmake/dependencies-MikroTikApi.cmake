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


## GetDependency
if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/GetDependency.cmake")
    message(STATUS "Installing dependency manager")
    file(DOWNLOAD
         https://raw.githubusercontent.com/isbodand/GetDependency/master/cmake/GetDependency.cmake
         "${CMAKE_CURRENT_LIST_DIR}/GetDependency.cmake"
         )
endif ()
include(GetDependency)

message(STATUS "[MikroTikApi] Getting dependencies")
## Build dependencies

set(FMT_INSTALL On CACHE BOOL "whatever" FORCE)
## {fmt}
if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    GetDependency(
            fmt
            REMOTE_ONLY
            REPOSITORY_URL https://github.com/fmtlib/fmt.git
            VERSION 6.2.1
    )
else ()
    GetDependency(
            fmt
            REPOSITORY_URL https://github.com/fmtlib/fmt.git
            VERSION 6.2.1
    )
endif ()

## Test Dependencies
if (${PROJECT_NAME}_BUILD_TESTS)
    # Catch2
    GetDependency(
            Catch2
            REPOSITORY_URL https://github.com/catchorg/Catch2.git
            VERSION v2.12.1
    )

endif ()

## Example Dependencies
if (${PROJECT_NAME}_BUILD_EXAMPLES)
    # cxxopts
    GetDependency(
            cxxopts
            REPOSITORY_URL https://github.com/jarro2783/cxxopts.git
            VERSION v2.2.0
    )

    # magic_enum
    GetDependency(
            magic_enum
            REPOSITORY_URL https://github.com/Neargye/magic_enum.git
            VERSION v0.6.6
    )

endif ()
