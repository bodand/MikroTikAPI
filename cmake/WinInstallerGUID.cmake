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


function(_WIG_MinGW _WIG_UpgradeGUID _WIG_ProductGUID)
    if (MikroTikApi_TARGET_TYPE STREQUAL "SHARED")
        set("${_WIG_UpgradeGUID}" "c114ab58-75d0-4be7-b39a-2c1e9b2524a4" PARENT_SCOPE)
        set("${_WIG_ProductGUID}" "abf00666-c6c7-41f4-8a87-2d08271d067b" PARENT_SCOPE)
    elseif (MikroTikApi_TARGET_TYPE STREQUAL "STATIC")
        set("${_WIG_UpgradeGUID}" "e69b938b-f16c-4375-9d15-0f6c15c768d5" PARENT_SCOPE)
        set("${_WIG_ProductGUID}" "d9328721-8780-4849-8e4b-51666863579c" PARENT_SCOPE)
    endif ()
endfunction()

function(_WIG_MSVC _WIG_UpgradeGUID _WIG_ProductGUID)
    if (MikroTikApi_TARGET_TYPE STREQUAL "SHARED")
        set("${_WIG_UpgradeGUID}" "5fefc6ff-8c20-4bcb-84c1-948a3136c0b4" PARENT_SCOPE)
        set("${_WIG_ProductGUID}" "ebd8272b-401a-4bde-a5c0-a96f376ae374" PARENT_SCOPE)
    elseif (MikroTikApi_TARGET_TYPE STREQUAL "STATIC")
        set("${_WIG_UpgradeGUID}" "b1a203f8-4b5d-4488-82a2-f55f7c058987" PARENT_SCOPE)
        set("${_WIG_ProductGUID}" "e16d3d30-33de-462c-952c-d4129493afb4" PARENT_SCOPE)
    endif ()
endfunction()

## WinInstallerGUID
# According to the build configuration this generates the GUID used by
# the Windows MSI installers.
#
# Differentiation:
#  - CMAKE_CXX_COMPILER_ID=[GNU,MSVC]
#  - MikroTikApi_TARGET_TYPE=[SHARED,STATIC]
#
# This leaves 4 different GUIDs used by the Windows installers.
# In the future more installers may be supported.
function(WinInstallerGUID _WIG_UpgradeGUID _WIG_ProductGUID)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        _WIG_MinGW(UG PG)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        _WIG_MSVC(UG PG)
    endif ()
    set("${_WIG_UpgradeGUID}" "${UG}" PARENT_SCOPE)
    set("${_WIG_ProductGUID}" "${PG}" PARENT_SCOPE)
endfunction()
