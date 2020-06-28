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
// Created by bodand on 2020-06-27.
//

#include <catch2/catch.hpp>

// test'd
#include <mikrotik/api/command.hpp>
#include <mikrotik/api/sentence.hpp>
using namespace mikrotik::api;
using namespace mikrotik::api::literals;

TEST_CASE("sentence can be created from a command",
          "[sentence][dsl][api]") {
    auto cmd = "login"_cmd;
    sentence snt(cmd);

    CHECK_THAT(snt.words(),
               Catch::Equals(std::vector<std::string>{"/login"}));
}

TEST_CASE("sentence can be created from initializer list of strings",
          "[sentence][dsl][api]") {
    sentence snt{"/login"};

    CHECK_THAT(snt.words(),
               Catch::Equals(std::vector<std::string>{"/login"}));
}

TEST_CASE("sentence can be created from command with dsl - attribute",
          "[sentence][dsl][api]") {
    auto snt =// clang-format off
           ("system"_cmd / "resource" / "print")
           [ {".proplist", "uptime,cpu-load"} ];
    // clang-format on

    CHECK_THAT(snt.words(),
               Catch::Equals(std::vector<std::string>{"/system/resource/print",
                                                      "=.proplist=uptime,cpu-load"}));
}

TEST_CASE("sentence can be created from command with dsl - query",
          "[sentence][dsl][api]") {
    auto snt =// clang-format off
           ("system"_cmd / "resource" / "print")
           ("-type");
    // clang-format on

    CHECK_THAT(snt.words(),
               Catch::Equals(std::vector<std::string>{"/system/resource/print",
                                                      "?-type"}));
}

TEST_CASE("named sentence can be expanded with attribute word",
          "[sentence][dsl][api]") {
    auto snt =// clang-format off
           ("i"_cmd / "cannot" / "think" / "of" / "an" / "appropriate" / "command")
           [ {"something", "some other thing"} ];
    // clang-format on
    snt = snt["val-less"][{"added", "sugar"}];

    CHECK_THAT(snt.words(),
               Catch::Equals(std::vector<std::string>{"/i/cannot/think/of/an/appropriate/command",
                                                      "=something=some other thing",
                                                      "=val-less=",
                                                      "=added=sugar"}));
}

TEST_CASE("named sentence can be expanded with query word",
          "[sentence][dsl][api]") {
    auto snt =// clang-format off
           ("interface"_cmd / "print")
                  ( {"type", "vlan"} )
                  ( {"type", "ether"} );
    // clang-format on
    snt = snt("#|");

    CHECK_THAT(snt.words(),
               Catch::Equals(std::vector<std::string>{"/interface/print",
                                                      "?type=vlan",
                                                      "?type=ether",
                                                      "?#|"}));
}
