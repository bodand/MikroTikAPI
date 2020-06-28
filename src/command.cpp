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
// Created by bodand on 2020-06-26.
//

#include <mikrotik/api/command.hpp>

// stdlib
#include <cstring>

// {fmt}
#include "lib/fmt.hpp"

mikrotik::api::command::command(std::string_view cmd)
     : cmd(cmd.size() + 1, '/') {
    std::memcpy(this->cmd.data() + 1, cmd.data(), cmd.size());
}

mikrotik::api::command
mikrotik::api::command::operator/(std::string_view nxt) && {
    return {std::move(this->cmd) + '/' + nxt.data(), 1};
}

mikrotik::api::command
mikrotik::api::command::operator/(std::string_view nxt) const& {
    return {cmd + '/' + nxt.data(), 1};
}

mikrotik::api::command::command(std::string_view cmd, int)
     : cmd(cmd) {
}

mikrotik::api::sentence
mikrotik::api::command::operator[](const attribute& attr) const {
    return mikrotik::api::sentence{cmd, attr.value};
}

mikrotik::api::sentence
mikrotik::api::command::operator()(const mikrotik::api::query& query) const {
    return mikrotik::api::sentence{cmd, query.value};
}

mikrotik::api::command::operator mikrotik::api::sentence() const {
    return sentence(*this);
}

// clang-format off
mikrotik::api::command
mikrotik::api::literals::operator""_cmd(const char* str, std::size_t n) {
    return {{str, n}};
}
// clang-format on
