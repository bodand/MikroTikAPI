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
// Created by bodand on 2020-06-28.
//

// stdlib
#include <exception>
#include <iostream>
#include <string>

// {fmt}
#include "lib/fmt.hpp"

// magic_enum
#include "lib/magic_enum.hpp"

// cxxopts
#include "lib/cxxopts.hpp"

// MikroTikAPI
#include <mikrotik/api/api_handler.hpp>
#include <mikrotik/api/command.hpp>
using namespace mikrotik::api::literals;

int
main(int argc, char** argv) {
    cxxopts::Options opts("mikrottyk", "A MikroTik device tty presenting the usage of MikroTikAPI.");

    // clang-format off
    opts.add_options()
           ("u,user", "The user to log in as into the MikroTik device. [admin]",
                cxxopts::value<std::string>()->default_value("admin"))
           ("p,password", "The password of the user to log in as into the MikroTik device. []",
                cxxopts::value<std::string>()->default_value(""))
           ("4,ip4", "The IPv4 address of the MikroTik device. [192.168.88.1]",
                cxxopts::value<std::string>()->default_value("192.168.88.1"))
           ("h,help", "Print this help message")
    ;
    // clang-format on

    auto ops = opts.parse(argc, argv);

    // help
    if (ops.count("help")) {
        fmt::print(opts.help());
        return 0;
    }

    std::string usr = ops["user"].as<std::string>();
    std::string passwd = ops["password"].as<std::string>();
    std::string ip4 = ops["ip4"].as<std::string>();

    try {
        mikrotik::api::api_handler api(ip4.c_str(), usr, passwd);

        std::vector<std::string> words;
        for (;;) {
            do {
                std::string inp;
                fmt::print(">>> ");
                std::getline(std::cin, inp);

                words.push_back(inp);
            } while (!(words.back().empty()
                       || words.back() == ":q"));
            if (words.back() == ":q")
                break;

            mikrotik::api::sentence snt(words.begin(), words.end());

            mikrotik::api::reply rep;
            api.send(snt);
            do {
                rep = api.read();
                fmt::print("<<< !{}\n", magic_enum::enum_name(rep.reply_type));
                for (const auto& resp : rep.attributes) {
                    fmt::print("<<< {}\n", resp);
                }
            } while (rep.reply_type == rep.re);
            words.clear();
        }
    } catch (const std::exception& ex) {
        fmt::print(ex.what());
        return -1;
    }
}
