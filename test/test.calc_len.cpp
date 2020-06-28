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

#include <mikrotik_api_export.h>
#ifdef MIKROTIK_API_STATIC_DEFINE
#    include <catch2/catch.hpp>

#    include <string>
#    include <string_view>
using namespace std::literals;

#    include <lib/fmt.hpp>

// test'd
#    include <mikrotik/api/exception/bad_word.hpp>
#    include "impl/calc_len.hpp"
using namespace mikrotik::api;
using namespace mikrotik::api::impl;

TEST_CASE("calc_len throws for too large words",
          "[calc_len][util][impl]") {
    try {
        std::string str(500'000'000, 'a');

        CHECK_THROWS_MATCHES(
               calc_len(str),
               bad_word,
               Catch::Predicate<bad_word>([](const bad_word& ex) {
                   return ex.what() == "error: ill-formed word: aaaa..<499999992 other>..aaaa: word too long"sv;
               },
                                          "thrown exception has correct error message"));
    } catch (std::bad_alloc&) {
        fmt::print("Well, your PC couldn't allocate a 500-million-character-long string at this time."
                   " Ignoring this test.");
        CHECK(true);
    }
}

TEST_CASE("calc_len creates correct length string for 1 byte long strings",
          "[calc_len][util][impl]") {
    std::string str("aa");
    std::string exp("\x02");

    CHECK(calc_len(str) == exp);
}

TEST_CASE("calc_len creates correct length string for 2 byte long strings",
          "[calc_len][util][impl]") {
    std::string str(256, 'a');
    std::string exp("\x81\x00");

    CHECK(std::memcmp(calc_len(str).data(), exp.data(), exp.size()) == 0);
}

TEST_CASE("calc_len creates correct length string for 3 byte long strings",
          "[calc_len][util][impl]") {
    std::string str(16385, 'a');
    std::string exp("\xC0\x40\x01");

    CHECK(std::memcmp(calc_len(str).data(), exp.data(), exp.size()) == 0);
}

TEST_CASE("calc_len creates correct length string for 4 byte long strings",
          "[calc_len][util][impl]") {
    std::string str(2097153, 'a');
    std::string exp("\xE0\x20\x00\x01");

    CHECK(std::memcmp(calc_len(str).data(), exp.data(), exp.size()) == 0);
}
#endif
