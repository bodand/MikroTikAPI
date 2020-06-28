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

#include <catch2/catch.hpp>

// test'd
#include <mikrotik/api/command.hpp>
using namespace mikrotik::api;
using namespace mikrotik::api::literals;

TEST_CASE("command contains the parameter with a leading /",
          "[command][dsl][api]") {
    command cmd("login");

    CHECK(cmd.cmd == "/login");
}

TEST_CASE("cmd udl creates correct command string",
          "[udl_cmd][udl][dsl][api]") {
    auto cmd = "login"_cmd;

    CHECK(cmd.cmd == "/login");
}

TEST_CASE("temporary command object can be chained for complex commands",
          "[command][dsl][api]") {
    auto cmd = "interface"_cmd / "print";

    CHECK(cmd.cmd == "/interface/print");
}

TEST_CASE("named command object can be chained without modifications",
          "[command][dsl][api]") {
    auto cmd1 = "interface"_cmd;
    auto cmd2 = cmd1 / "print";

    CHECK(cmd1.cmd == "/interface");
    CHECK(cmd2.cmd == "/interface/print");
}
