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

#pragma once

// stdlib
#include <initializer_list>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// InfoUtils
//#include <info/_macros.hpp>

// project
#include <mikrotik_api_export.h>
#include "attribute.hpp"
#include "query.hpp"

namespace mikrotik::api {
    struct MIKROTIK_API_EXPORT command;

    struct MIKROTIK_API_EXPORT sentence {
        explicit sentence(command cmd);
        explicit sentence(std::initializer_list<std::string> words);

        template<class It>
        sentence(It beg, It end);

        sentence& operator[](attribute attr) &&;
        sentence operator[](attribute attr) const&;
        sentence& operator()(query q) &&;
        sentence operator()(query q) const&;

        void add_word(std::string_view word);
        const std::vector<std::string>& words() const;
    private:
        std::vector<std::string> _words;
    };

    template<class It>
    sentence::sentence(It beg, It end)
         : _words(beg, end) {
    }
}
