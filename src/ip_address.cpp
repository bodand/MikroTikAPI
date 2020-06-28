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
//
// Created by bodand on 2020-06-24.
//


// stdlib
#include <algorithm>
#include <charconv>
#include <string>
#include <vector>

// {fmt}
#include "lib/fmt.hpp"

// project
#include <mikrotik/api/impl/sockets.hpp>

#include "impl/split.hpp"
#include <mikrotik/api/api_handler.hpp>
#include <mikrotik/api/attribute.hpp>
#include <mikrotik/api/exception/bad_ip_format.hpp>
#include <mikrotik/api/ip_address.hpp>
#include <mikrotik_api_export.h>

mikrotik::api::ip_address::ip_address(std::string_view address)
     : _bytes() {
    std::vector<std::string> bytes = impl::split(address, '.');

    if (bytes.size() != 4)
        throw bad_ip_format(address);

    std::transform(bytes.begin(), bytes.end(), _bytes.begin(), [address](std::string_view str) {
        std::uint8_t ret;
        if (auto [__, err] = std::from_chars(str.data(), str.data() + str.size(), ret);
            err == std::errc{})
            return ret;
        throw bad_ip_format(address);
    });
}

mikrotik::api::ip_address::ip_address(const char *address)
     : ip_address{std::string_view{address}} { }

std::string
mikrotik::api::ip_address::render(int port) const {
    if (port) {
        return fmt::format("{}.{}.{}.{}:{}", _bytes[0], _bytes[1], _bytes[2], _bytes[3], port);
    }
    return fmt::format("{}.{}.{}.{}", _bytes[0], _bytes[1], _bytes[2], _bytes[3]);
}
