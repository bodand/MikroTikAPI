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

#pragma once

// stdlib
#include <string_view>
#include <string>
#include <exception>

// project
#include <mikrotik_api_export.h>

namespace mikrotik::api {
    /**
     * \brief Exception describing failures occurring during handling
     * words
     *
     * An exception that describes words that caused problems. For example
     * such a situation is a word that's too long
     * (ie. its length takes more than four bytes to represent).
     *
     * \rst
     * .. note::
     *  The exception may not contain the whole word, just a representation
     *  of it. For example a word containing 50 million characters is a word
     *  that's too long to handle so trying to process it will cause this exception.
     *  The word in the exception will only contain the first and last 4 characters
     *  and an indication about the amount of characters omitted.
     * \endrst
     *
     * \since v1.0.0
     */
    struct MIKROTIK_API_EXPORT bad_word : std::exception {
        /**
         * Creates a bad_word exception with the provided
         * word and the reason for the error.
         *
         * \param word The word that caused the error
         * \param reason A string describing the error
         *
         * \since v1.0.0
         */
        explicit bad_word(std::string_view word, std::string_view reason = "unknown error");

        /**
         * \copydoc bad_ip_format::what()
         */
        const char* what() const noexcept override;
    private:
        std::string _reason;
        std::string _word;
    };
}
