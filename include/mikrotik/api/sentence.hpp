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

    /**
     * \brief Contains a command with optional additional words sent to the MikroTik device
     *
     * A sentence is a command followed by zero or more attribute or query words
     * and is terminated by the empty word. A sentence is sent to the MikroTik device
     * which processes it when the empty word is received.
     *
     * A valid sentence matches the following EBNF grammar:
     * \rst
     * .. code-block:: abnf
     *
     *  sentence ::= command, [ additional words ];
     *
     *  additional words ::= { attribute }
     *                     | { query }
     *                     ;
     * \endrst
     * For the \ref command, \ref attribute, and \ref query grammars see their documentation.
     *
     * \since v1.0.0
     */
    struct MIKROTIK_API_EXPORT sentence {
        /**
         * \brief Constructs the empty sentence
         *
         * Creates the empty sentence which is always
         * ignored when sent.
         *
         * \since v1.1.0
         */
        sentence() = default;
        /**
         * \brief Constructs a sentence from a command
         *
         * Constructs a sentence from a command with zero attributes.
         * Useful for use with the DSL.
         *
         * \param cmd The command to create the sentence from
         *
         * \since v1.0.0
         */
        explicit sentence(command cmd);
        /**
         * \brief Creates a sentence from a sequence of words
         *
         * Takes an initializer_list of strings and interprets them as
         * a sequence of words to create a sentence from. The first
         * word is the command, which must be a valid MikroTik API command word,
         * as the following words must be valid MikroTik API attribute or query words.
         * All words are sent verbatim.
         *
         * \param words The sequence of words to create a sentence from
         *
         * \since v1.0.0
         */
        explicit sentence(std::initializer_list<std::string> words);

        /**
         * \brief Creates a sentence from an iterator provided range of words
         *
         * Essentially does the same thing as sentence(std::initializer_list<std::string>)
         * but with an iterator pair.
         *
         * Starts reading from `beg` and copies all words until it reaches `end`.
         * The value pointed to by `beg` is the command and it must be a valid
         * MikroTik API command word, as the following words must be valid
         * MikroTik API attribute or query words. All words are sent verbatim.
         *
         * \tparam It The type of the given iterator
         * \param beg The iterator pointing at the beginning of the range
         * \param end The iterator pointing past the end of the range
         *
         * \since v1.0.0
         */
        template<class It>
        sentence(It beg, It end);

        /**
         * \brief Adds an attribute to the sentence
         *
         * As an attribute may also be constructed implicitly from a string
         * the following two invocations are usable:
         * \code
         * sentence snt{ // ...
         * };
         *
         * snt = snt["name-without-value"];
         * snt = snt[{"name-with-value", "value"}];
         * \endcode
         * They of course may be chained.
         *
         * \param attr The attribute to add.
         * \return The modified sentence
         *
         * \since v1.0.0
         */
        sentence& operator[](attribute attr) &&;
        /**
         * \copydoc sentence::operator[](attribute)&&
         */
        sentence operator[](attribute attr) const&;
        /**
         * \brief Adds an query to the sentence
         *
         * As a query may also be constructed implicitly from a string
         * the following two invocations are usable:
         * \code
         * sentence snt{ // ...
         * };
         *
         * snt = snt("name-without-value"); // this creates ?name-without-value
         * snt = snt({"name-with-value", "value"}); // ?name-with-value=value
         * \endcode
         * They of course may be chained.
         *
         * \param q The query to add.
         * \return The modified sentence
         *
         * \since v1.0.0
         */
        sentence& operator()(query q) &&;
        /**
         * \copydoc sentence::operator()(query)&&
         */
        sentence operator()(query q) const&;

        /**
         * \brief Adds a word to the sentence as is
         * \param word The word to add to the sentence
         *
         * \since v1.0.0
         */
        void add_word(std::string_view word);
        /**
         * \brief Returns the current words of the sentence
         *
         * \return The words stored by the sentence
         */
        const std::vector<std::string>& words() const;
    private:
        std::vector<std::string> _words;
    };

    template<class It>
    sentence::sentence(It beg, It end)
         : _words(beg, end) {
    }
}
