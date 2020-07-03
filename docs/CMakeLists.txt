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

find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

## Doxygen #####################################################################
set(DOXYGEN_INCLUDE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file(
        Doxyfile.in Doxyfile
        @ONLY
)

add_custom_command(
        COMMENT "Generating documentation"
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/xml/index.xml"
        BYPRODUCT "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
        COMMAND "${DOXYGEN_EXECUTABLE}" Doxyfile
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(docs-doxy ALL
                  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/xml/index.xml")

## Sphinx ######################################################################
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/docs/)

add_custom_target(docs ALL
                  COMMENT "Generating documentation with Sphinx"
                  COMMAND "${SPHINX_EXECUTABLE}" -b html
                  "${SPHINX_SOURCE}" "${SPHINX_BUILD}"
                  "-Dbreathe_projects.MikroTikApi=${DOXYGEN_OUTPUT_DIRECTORY}/xml"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  )
add_dependencies(docs docs-doxy)
