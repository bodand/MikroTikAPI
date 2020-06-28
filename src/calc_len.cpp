// BSD 3-Clause License
//
// Copyright (c) 2020, bodand
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by bodand on 2020-06-25.
//

#include <mikrotik/api/exception/bad_word.hpp>
#include "impl/calc_len.hpp"

// {fmt}
#include "lib/fmt.hpp"

std::string
mikrotik::api::impl::calc_len(std::string_view str) {
    auto len = str.size();
    char* len_alias = (char*) &len;
    char ret[sizeof(int)] = {'\0'};

    if (len < 0x80) {// 1 byte
        return std::string(1, (char) len);
    }
    if (len < 0x4000) {// 2 bytes
        ret[0] = len_alias[1] | 0x80;
        ret[1] = len_alias[0];
        return std::string(ret, 2);
    }
    if (len < 0x200000) {// 3 bytes
        ret[0] = len_alias[2] | 0xC0;
        ret[1] = len_alias[1];
        ret[2] = len_alias[0];
        return std::string(ret, 3);
    }
    if (len < 0x10000000) {// 4 bytes
        ret[0] = len_alias[3] | 0xE0;
        ret[1] = len_alias[2];
        ret[2] = len_alias[1];
        ret[3] = len_alias[0];
        return std::string(ret, 4);
    }
    std::string short_str = fmt::format("{}..<{} other>..{}",
                                        str.substr(0, 4),
                                        str.size() - 8,
                                        str.substr(str.size() - 4));
    throw bad_word{short_str, "word too long"};
}
