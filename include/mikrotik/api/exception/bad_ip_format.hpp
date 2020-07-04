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
#include <exception>
#include <string>
#include <string_view>

// project
#include <mikrotik_api_export.h>

namespace mikrotik::api {
    /**
     * \brief The exception thrown if the provided string does not contain a valid IPv4 address
     *
     * This exception is thrown by ip_address if the string provided does not
     * fit the requirements of an IPv4 address.
     * Most plausible causes are:
     *  - Does not contain all four bytes
     *  - Contains non digit values
     *  - Contains an inappropriate amount of separator periods
     *
     * \since v1.0.0
     */
    struct MIKROTIK_API_EXPORT bad_ip_format : std::exception {
        /**
         * Creates a bad_ip_format exception with the provided
         * ill-formed IPv4 address string
         *
         * \param faulty_address The string containing the ill-formed IPv4 address
         *
         * \since v1.0.0
         */
        explicit bad_ip_format(std::string_view faulty_address) noexcept;

        /**
         * Returns a formatted error message about the error
         * \return A c-string with the error message
         * \since v1.0.0
         */
        const char* what() const noexcept override;
    private:
        std::string _bad_ip;
    };
}
