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
#include <string>
#include <string_view>

// project
#include "sentence.hpp"
#include "attribute.hpp"
#include "query.hpp"
#include <mikrotik_api_export.h>

namespace mikrotik::api {
    /**
     * \brief A command to send and to execute on the MikroTik device
     *
     * A command is a word that can be executed by the MikroTik device it's sent
     * to. This is always the first word to be sent to the device and may be
     * followed by attribute or query words that either provide required
     * information for the command's execution, or filter the output provided
     * by it respectively.
     *
     * A DSL is provided by the library to allow easy creation of commands
     * by utilizing the ``_cmd`` user defined literal on strings,
     * which invokes the first constructor.
     *
     * \since v1.0.0
     */
    struct MIKROTIK_API_EXPORT command {
        /**
         * \brief Creates a command from a string
         *
         * Takes a string and prepends a forward slash to it to
         * create a command. This is then sent to the MikroTik device
         * so the given string with the prepended slash must be a valid
         * MikroTik API command word.
         *
         * \param cmd The string to turn into a command
         *
         * \since v1.0.0
         */
        command(std::string_view cmd);

        std::string cmd; ///< The command as a string ready to be sent

        /**
         * \brief Appends a string to a command
         *
         * Takes a command and adds a new part to it by concatenating
         * a forward slash and the provided string.
         *
         * For example:
         * \code
         * auto cmd = "system"_cmd;           // -> /system
         * cmd = cmd / "resource" / "print"; // -> /system/resource/print
         * \endcode
         *
         * \param nxt The string to append
         * \return The new command
         *
         * \since v1.0.0
         */
        command operator/(std::string_view nxt) &&;
        /**
         * \copydoc command::operator/(std::string_view)&&
         */
        command operator/(std::string_view nxt) const&;

        /**
         * \brief Creates a sentence from the command with an attribute
         *
         * Takes the command and promotes it to a sentence by taking the command
         * and adding an attribute word.
         *
         * \param attr The attribute to add to create the sentence
         * \return The newly created sentence
         *
         * \sa attribute
         * \sa sentence
         *
         * \since v1.0.0
         */
        sentence operator[](const attribute& attr) const;
        /**
         * \brief Creates a sentence from the command with a query
         *
         * Takes the command and promotes it to a sentence by taking the command
         * and adding a query word.
         *
         * \param query The query to add to create the sentence
         * \return The newly created sentence
         *
         * \sa query
         * \sa sentence
         *
         * \since v1.0.0
         */
        sentence operator()(const query& query) const;

        /**
         * \brief Implicit conversion operator to a sentence.
         *
         * Promotes the command to a sentence with one word: this command.
         *
         * \return The new sentence
         *
         * \since v1.0.0
         */
        operator sentence() const;

    private:
        command(std::string_view cmd, int);
    };

    inline namespace literals {
        /**
         * \brief Creates a command from a string literal
         *
         * Takes a string literal and returns a command
         * from it by prepending a forward slash to it.
         *
         * For more information about the construction
         * see command::command(std::string_view)
         *
         * \param str The string to create the command from
         * \param n The length of the string
         * \return The created command
         *
         * \since v1.0.0
         */
        MIKROTIK_API_EXPORT
        command operator""_cmd(const char* str, std::size_t n);
    }
}
