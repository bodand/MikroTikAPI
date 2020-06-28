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


include(CheckCXXCompilerFlag)

## Compiler warnings
function(CheckWarningFlag OptionName CacheName)
    check_cxx_compiler_flag("-W${OptionName}" "HasWarning_${CacheName}")
    set("HAS_WARNING_${CacheName}" ${HasWarning_${CacheName}} PARENT_SCOPE)
endfunction()

# Possible warnings to check for
set("${PROJECT_NAME}_POSSIBLE_WARNINGS"
    extra pedantic sign-compare uninitialized unused cast-qual cast-align
    abstract-vbase-init array-bounds-pointer-arithmetic assign-enum conditional-uninitialized
    consumed deprecated-implementations documentation-deprecated-sync non-virtual-dtor
    header-hygiene infinite-recursion keyword-macro loop-analysis
    move newline-eof over-aligned redundant-parens reorder reserved-id-macro sign-conversion
    signed-enum-bitfield sometimes-uninitialized tautological-overlap-compare thread-safety
    undefined-internal-type undefined-reinterpret-cast unneeded-internal-declaration
    unreachable-code-aggressive unreachable-code-loop-increment unused-const-variable
    unused-exception-parameter unused-parameter unused-template unused-variable nullability-completeness
    no-unknown-pragmas no-unused-macros no-nullability-extension
    no-c++20-extensions
    suggest-attribute=pure suggest-attribute=const suggest-attribute=cold suggest-final-types
    suggest-final-methods duplicated-branches trampolines placement-new=2 redundant-decls logical-op
    # User requested
    ${INFO_ADDITIONAL_WARNINGS} # todo document this
    ${${PROJECT_NAME}_ADDITIONAL_WARNINGS} # todo document this
    )

# check warning flags
foreach (WARN_I IN LISTS "${PROJECT_NAME}_POSSIBLE_WARNINGS")
    string(MAKE_C_IDENTIFIER "${WARN_I}" "CacheName")
    CheckWarningFlag("${WARN_I}" ${CacheName})
    if (HAS_WARNING_${CacheName})
        list(APPEND "${PROJECT_NAME}_WARNINGS" -W${WARN_I})
    endif ()
endforeach ()
