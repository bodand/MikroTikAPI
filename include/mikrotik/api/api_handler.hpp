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

#pragma once

// stdlib
#include <string>
#include <string_view>

// project
#include "impl/sockets.hpp"
#include "ip_address.hpp"
#include "reply.hpp"
#include "sentence.hpp"
#include <mikrotik_api_export.h>

namespace mikrotik::api {
    struct MIKROTIK_API_EXPORT api_handler {
        explicit api_handler(ip_address address = "192.168.88.1",
                             std::string_view user = "admin",
                             std::string_view pass = "");

        void disconnect();

        virtual ~api_handler();

        void send(sentence snt);
        mikrotik::api::reply read();
    private:
        std::string read_word();
        void send_word(std::string_view word);

        std::int32_t read_len();

        impl::socket::handle _sock;

        // socket handling
        void initialize_sockets() const;
        void mk_socket();
        void connect_to_device(const ip_address& address);
        sockaddr mk_addr(const ip_address& address) const;
        void login(std::string_view view, std::string_view view_1);
    };
}
