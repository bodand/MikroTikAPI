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

#include <catch2/catch.hpp>

#include <type_traits>
#include <exception>
#include <string_view>
using namespace std::literals;

// test'd
#include <mikrotik/api/exception/bad_word.hpp>
using namespace mikrotik::api;

TEST_CASE("bad_word constructor/1 does not throw",
          "[bad_word][exception][api]") {
    CHECK_NOTHROW(bad_word{"some word"});
}

TEST_CASE("bad_word constructor/2 does not throw",
          "[bad_word][exception][api]") {
    CHECK_NOTHROW(bad_word{"some word", "some reason"});
}

TEST_CASE("bad_word without reason creates correct message",
          "[bad_word][exception][api]") {
    bad_word ex{"some word"};

    CHECK(ex.what() == "error: ill-formed word: some word: unknown error"sv);
}

TEST_CASE("bad_word with reason creates correct message",
          "[bad_word][exception][api]") {
    bad_word ex{"some word", "some reason"};

    CHECK(ex.what() == "error: ill-formed word: some word: some reason"sv);
}
