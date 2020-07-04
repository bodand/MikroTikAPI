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

// project
#include "impl/calc_len.hpp"
#include <mikrotik/api/impl/sockets.hpp>
#include "impl/socket_funcs.hpp"
#include "lib/fmt.hpp"
#include <mikrotik/api/api_handler.hpp>
#include <mikrotik/api/command.hpp>
#include <mikrotik/api/exception/bad_socket.hpp>
namespace sock = mikrotik::api::impl::socket;
using namespace mikrotik::api::literals;

mikrotik::api::api_handler::api_handler(ip_address address,
                                        std::string_view user,
                                        std::string_view pass)
     : _sock{INVALID_SOCKET} {
    initialize_sockets();
    mk_socket();
    connect_to_device(address);
    login(user, pass);
}

sockaddr
mikrotik::api::api_handler::mk_addr(const mikrotik::api::ip_address& address) const {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, address.render().c_str(), &addr.sin_addr.s_addr) != 1)
        throw bad_socket(fmt::format("unable to create address structure to IP address: {}",
                                     sock::string_error(sock::get_last_error())));
    // ^^ Microsoft going around deprecating POSIX functions damn it:
    // inet_addr is deprecated on MSVC
    addr.sin_port = htons(static_cast<std::uint16_t>(8728));

    sockaddr ret;
    std::memcpy(&ret, &addr, sizeof(sockaddr));

    return ret;
}

int
mikrotik::api::api_handler::disconnect() noexcept {
    int ret = 0;
    if (sock::is_valid(_sock)) {
        ret = sock::close(_sock);
    }
    sock::finish();
    _sock = INVALID_SOCKET;
    return ret;
}

mikrotik::api::api_handler::~api_handler() noexcept {
    disconnect();
}

void
mikrotik::api::api_handler::send_word(std::string_view word) {
    auto sent = impl::calc_len(word) + word.data();
    ::send(_sock, sent.c_str(), sent.size(), 0);
}

std::int32_t
mikrotik::api::api_handler::read_len() {
    // we work as if big-endian because thankfully the Internet is big-endian
    // and in the end we convert because that's more likely than
    // being on a big-endian machine

    std::int32_t len = 0;
    auto len_alias = reinterpret_cast<char*>(&len);

    recv(_sock, &len_alias[0], 1, 0);
    if ((len_alias[0] & 0xE0) == 0xE0) {// 4 bytes
        len_alias[0] &= 0b0001'1111;    // ~0xE0
        recv(_sock, &len_alias[1], 3, 0);

        len = static_cast<std::int32_t>(ntohl(static_cast<unsigned long>(len)));

    } else if ((len_alias[0] & 0xC0) == 0xC0) {// 3 bytes
        len_alias[0] &= 0b0011'1111;           // ~0xC0
        recv(_sock, &len_alias[1], 2, 0);

        auto swapped = static_cast<std::int32_t>(ntohl(static_cast<unsigned long>(len)));
        len >>= 8 * (((char*) &len)[0] != ((char*) &swapped)[0]);

    } else if ((len_alias[0] & 0x80) == 0x80) {// 2 bytes
        len_alias[0] &= 0b0111'1111;           // ~0x80
        recv(_sock, &len_alias[1], 1, 0);

        len = static_cast<std::int32_t>(ntohs(static_cast<unsigned short>(len)));
    }
    return len;
}

void
mikrotik::api::api_handler::initialize_sockets() const {
    // on POSIX this should optimize into nothing
    if (sock::init() != 0)
        throw bad_socket(fmt::format("initialization failed: {}",
                                     sock::string_error(sock::get_last_error())));
}

void
mikrotik::api::api_handler::mk_socket() {
    _sock = sock::create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sock::is_valid(_sock))
        throw bad_socket(fmt::format("creating socket failed: {}",
                                     sock::string_error(sock::get_last_error())));
}

void
mikrotik::api::api_handler::connect_to_device(const ip_address& address) {
    auto addr = mk_addr(address);
    auto conn = connect(_sock, &addr, sizeof(addr));
    if (conn == SOCKET_ERROR)
        throw bad_socket(fmt::format("could not connect to {}: {}",
                                     address.render(8728),
                                     sock::string_error(sock::get_last_error())));
}

std::string
mikrotik::api::api_handler::read_word() {
    std::int32_t len = read_len();
    std::string word;

    char buff[1024];
    while (len > 0) {
        auto to_read = len > 1024 ? 1024 : len;

        auto read = recv(_sock, buff, to_read, 0);

        if (read == SOCKET_ERROR)
            throw bad_socket(fmt::format("failure while reading {} bytes with {} remaining from the word: {}",
                                         to_read,
                                         len,
                                         sock::string_error(sock::get_last_error())));

        word.append(buff, static_cast<unsigned>(read));
        len -= read;
    }

    return word;
}

void
mikrotik::api::api_handler::login(std::string_view usr, std::string_view passwd) {
    auto comm = "login"_cmd
           [{"name", usr}]
           [{"password", passwd}];
    send(comm);
    auto rep = read();
    if (rep.reply_type != rep.done)
        throw bad_socket(fmt::format("error: could not log into device: {}", rep.attributes.back()));
}

void
mikrotik::api::api_handler::send(const mikrotik::api::sentence& snt) {
    for (const auto& word : snt.words()) {
        send_word(word);
    }
    send_word("");
}

mikrotik::api::reply
mikrotik::api::api_handler::read() {
    reply rep;

    std::string word;
    while (!(word = read_word()).empty()) {
        if (word == "!done") {
            rep.reply_type = reply::done;
            continue;
        } else if (word == "!trap") {
            rep.reply_type = reply::trap;
            continue;
        } else if (word == "!fatal") {
            rep.reply_type = reply::fatal;
            continue;
        } else if (word == "!re") {
            rep.reply_type = reply::re;
            continue;
        }

        rep.attributes.push_back(word);
    }

    return rep;
}
