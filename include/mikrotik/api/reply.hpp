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
// Created by bodand on 2020-06-27.
//

#pragma once

// stdlib
#include <string>
#include <vector>

namespace mikrotik::api {
    /**
     * \brief The sentence sent as a reply from the MikroTik device
     *
     * Contains the reply word and the attributes that were sent
     * as a response by the MikroTik device.
     *
     * It's difference to a sentence is mostly that it's received
     * and not sent.
     * While a sentence contains a command which is sent then executed on the device,
     * a reply sentence contains the results of that execution. They may contain
     * that it's been successfully completed, some content that was generated, or some error.
     *
     * A valid reply sentence matches the following EBNF grammar:
     * \rst
     * .. code-block:: abnf
     *
     *  reply sentence ::= "!", reply content;
     *
     *  reply content ::= reply done
     *                  | reply re
     *                  | reply trap
     *                  | reply fatal
     *                  ;
     *
     *  reply done ::= "done";
     *
     *  reply re ::= "re", { attribute };
     *
     *  reply trap ::= "trap", attribute;
     *
     *  reply fatal ::= "fatal", attribute;
     * \endrst
     * For the attribute grammar see its documentation.
     *
     * \since v1.0.0
     */
    struct reply {
        /**
         * \brief The reply sentence's type
         *
         * The type of the reply sent from the MikroTik device
         * as response to the sent sentence.
         * They are represented by a leading exclamation mark:
         * eg. done is sent as `!done`.
         *
         * \since v1.0.0
         */
        enum type {
            /**
             * Denotes the last reply word to a sent sentence.
             * Always comes last and the sentence never contains
             * any other words.
             *
             * \since v1.0.0
             */
            done,
            /**
             * Denotes an error during the processing of the sent
             * sentence. Sentences with this word contain a message
             * word which contains the reason for the error.
             *
             * \since v1.0.0
             */
            trap,
            /**
             * Denotes the connection getting terminated.
             *
             * \since v1.0.0
             */
            fatal,
            /**
             * Denotes one of the reply sentences that are sent with
             * meaningful content. There may be more, or zero re sentences
             * forming a reply to a sent sentence. The end of re sentences
             * is represented by a done sentence.
             *
             * \since v1.0.0
             */
            re
        };

        type reply_type; ///< The type of the reply sentence received
        std::vector<std::string> attributes; ///< Content attributes of the received sentence
    };
}
