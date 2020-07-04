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
#include <cstdint>
#include <string_view>
#include <array>

// project
#include <mikrotik_api_export.h>

namespace mikrotik::api {
    /**
     * \brief IP address verifier
     *
     * A quick utility to catch invalid ip address formats, before
     * dealing with the sockets.
     *
     * Checks if the given string is in the expected format for
     * an IPv4 address: `<a>.<b>.<c>.<d>` where all variables
     * are expected to be a byte ie. in the interval [0; 255].
     *
     * \since v1.0.0
     *
     * \rst
     * .. caution::
     *  This thing's existence is questionable.
     *  It is likely going to get deprecated while in v1.x.x,
     *  or get some new and fancy functionality
     * \endrst
     */
    struct MIKROTIK_API_EXPORT ip_address {
        /**
         * \brief Creates an ip_address object from a string
         *
         * Creates an IPv4 address object by parsing the string.
         * IPv4 is the only supported IP standard thus the expected string
         * is of the form `<a>.<b>.<c>.<d>` where all values are in the
         * interval [0; 255].
         *
         * \throws bad_ip_format: If the provided string does not contain a valid
         * IPv4 address.
         *
         * \param address The string that stores the IP address to validate & convert
         *
         * \since v1.0.0
         */
        ip_address(const char* address);
        /**
         * \copydoc ip_address(const char*)
         */
        ip_address(std::string_view address);

        /**
         * \brief Creates a string from the stored ip address
         *
         * Creates a valid IPv4 address from the stored bytes.
         *
         * Optionally a port may be supplied, which, if not zero,
         * will be added at the end of the string ie. the end
         * result is of the form `<a>.<b>.<c>.<d>[:<port>]`
         * where the part in brackets is only created if port is nonzero.
         *
         * \param port The port to add to the address. Zero is not rendered.
         * \return A string containing the IPv4 address and optionally the port.
         *
         * \since v1.0.0
         */
        std::string render(int port = 0) const;

        std::array<std::uint8_t, 4> _bytes; ///< The four bytes of the IPv4 address
    };
}
