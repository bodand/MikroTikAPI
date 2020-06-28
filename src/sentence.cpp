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

// {fmt}
#include "lib/fmt.hpp"

// project
#include <mikrotik/api/command.hpp>
#include <mikrotik/api/sentence.hpp>

mikrotik::api::sentence::sentence(command cmd)
     : _words{cmd.cmd} {
}

mikrotik::api::sentence::sentence(std::initializer_list<std::string> words)
     : _words(words.begin(), words.end()) {
}

void
mikrotik::api::sentence::add_word(std::string_view word) {
    _words.emplace_back(word.data());
}

const std::vector<std::string>&
mikrotik::api::sentence::words() const {
    return _words;
}

mikrotik::api::sentence&
mikrotik::api::sentence::operator[](mikrotik::api::attribute attr) && {
    add_word(attr.value);
    return *this;
}

mikrotik::api::sentence
mikrotik::api::sentence::operator[](mikrotik::api::attribute attr) const& {
    sentence ret(_words.begin(), _words.end());
    ret.add_word(attr.value);
    return ret;
}

mikrotik::api::sentence&
mikrotik::api::sentence::operator()(mikrotik::api::query q) && {
    add_word(q.value);
    return *this;
}

mikrotik::api::sentence
mikrotik::api::sentence::operator()(mikrotik::api::query q) const& {
    sentence ret(_words.begin(), _words.end());
    ret.add_word(q.value);
    return ret;
}
