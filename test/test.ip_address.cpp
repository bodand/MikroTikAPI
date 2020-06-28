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
// Created by bodand on 2020-06-24.
//

#include <catch2/catch.hpp>

#include <string_view>
using namespace std::literals;

// test'd
#include <mikrotik/api/exception/bad_ip_format.hpp>
#include <mikrotik/api/ip_address.hpp>
using namespace mikrotik::api;

TEST_CASE("ip_address constructor does not throw with valid IPv4 address",
          "[ip_address][util][api]") {
    CHECK_NOTHROW(ip_address{"1.1.1.1"});
}

TEST_CASE("ip_address constructor throws with empty input",
          "[ip_address][util][api]") {
    CHECK_THROWS_MATCHES(ip_address{""},
                         bad_ip_format,
                         Catch::Predicate<bad_ip_format>([](bad_ip_format ex) {
                             return ex.what() == "error: ill-formed IPv4 address passed: "sv;
                         }));
}

TEST_CASE("ip_address constructor throws with ill-formed ip address - one number",
          "[ip_address][util][api]") {
    CHECK_THROWS_MATCHES(ip_address{"1111"},
                         bad_ip_format,
                         Catch::Predicate<bad_ip_format>([](bad_ip_format ex) {
                             return ex.what() == "error: ill-formed IPv4 address passed: 1111"sv;
                         }));
}

TEST_CASE("ip_address constructor throws with incomplete ip address - only 2 bytes",
          "[ip_address][util][api]") {
    CHECK_THROWS_MATCHES(ip_address{"1.1"},
                         bad_ip_format,
                         Catch::Predicate<bad_ip_format>([](bad_ip_format ex) {
                             return ex.what() == "error: ill-formed IPv4 address passed: 1.1"sv;
                         }));
}

TEST_CASE("ip_address constructor throws with incomplete ip address - only 3 bytes",
          "[ip_address][util][api]") {
    CHECK_THROWS_MATCHES(ip_address{"1.1.1"},
                         bad_ip_format,
                         Catch::Predicate<bad_ip_format>([](bad_ip_format ex) {
                             return ex.what() == "error: ill-formed IPv4 address passed: 1.1.1"sv;
                         }));
}

TEST_CASE("ip_address constructor throws with incomplete ip address - consecutive periods",
          "[ip_address][util][api]") {
    CHECK_THROWS_MATCHES(ip_address{"1..1.1"},
                         bad_ip_format,
                         Catch::Predicate<bad_ip_format>([](bad_ip_format ex) {
                             return ex.what() == "error: ill-formed IPv4 address passed: 1..1.1"sv;
                         }));
}

TEST_CASE("ip_address constructor throws with incomplete ip address - only periods",
          "[ip_address][util][api]") {
    CHECK_THROWS_MATCHES(ip_address{"..."},
                         bad_ip_format,
                         Catch::Predicate<bad_ip_format>([](bad_ip_format ex) {
                             return ex.what() == "error: ill-formed IPv4 address passed: ..."sv;
                         }));
}

TEST_CASE("ip_address constructor throws with incomplete ip address - letters",
          "[ip_address][util][api]") {
    CHECK_THROWS_MATCHES(ip_address{"1.1.1.b"},
                         bad_ip_format,
                         Catch::Predicate<bad_ip_format>([](bad_ip_format ex) {
                             return ex.what() == "error: ill-formed IPv4 address passed: 1.1.1.b"sv;
                         }));
}

TEST_CASE("ip_address constructor throws with incomplete ip address - gibberish",
          "[ip_address][util][api]") {
    CHECK_THROWS_MATCHES(ip_address{"9U5s"},
                         bad_ip_format,
                         Catch::Predicate<bad_ip_format>([](bad_ip_format ex) {
                             return ex.what() == "error: ill-formed IPv4 address passed: 9U5s"sv;
                         }));
}

TEST_CASE("ip_address contains correct bytes",
          "[ip_address][util][api]") {
    ip_address ip{"0.1.2.3"};

    for (std::size_t i = 0; i < 4; ++i) {
        CHECK(ip._bytes[i] == i);
    }
}

TEST_CASE("ip_address is correctly rendered back into a string",
          "[ip_address][util][api]") {
    ip_address ip{"1.2.3.4"};

    CHECK(ip.render() == "1.2.3.4");
}

TEST_CASE("ip_address' render appends non-zero port",
          "[ip_address][util][api]") {
    ip_address ip{"1.2.3.4"};

    CHECK(ip.render(8080) == "1.2.3.4:8080");
}
