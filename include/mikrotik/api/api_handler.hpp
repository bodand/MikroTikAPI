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
    /**
     * \brief Handles all interaction with the MikroTik API.
     *
     * The class which is used to connect to, log into, then
     * send to and receive sentences from the MikroTik device of your
     * dreams, or at least the one on your shelf.
     *
     * Its lifetime is equivalent to the lifetime of the connection,
     * it this dies, the connection is killed with it.
     *
     * The constructor takes the IP address of the device, only IPv4 at the moment,
     * the user to log in as, and the password to do so. If any of them
     * is incorrect an exception will be thrown:
     * If the IP is ill-formed, then during creation, otherwise
     * when trying to use the incorrect data to connect: in this case
     * a timeout is likely to occur, which will result in an exception,
     * but will also halt the program for some time,
     * before the resident socket library realizes data's incorrect.
     *
     * The public API allows sending and receiving sentences.
     * A sentence according to the MikroTik API docs, is a list of words,
     * where a word is just a string.
     * A sentence contains a command and some additional words. For
     * a more comprehensive documentation see the \ref sentence docs.
     *
     * If you want to explicitly disconnect, use the disconnect() member function.
     * This does all the things the destructor would do without requiring fancy
     * tricks with scopes to get the lifetime just right.
     *
     * Example usage:
     * \code
     * #include <mikrotik/api/api_handler.hpp>
     * #include <mikrotik/api/command.hpp>
     *
     * namespace mt = mikrotik::api;
     * using namespace mt::literals;
     *
     * int main() {
     *     mt::api_handler api("192.168.1.1"); // user = "admin"
     *                                         // passwd = ""
     *     api.send("system"_cmd / "restart");
     *     api.read();
     * }
     * \endcode
     * This program simply restarts the MikroTik device, provided
     * the credentials are correct. Then it proceeds to ignore the response.
     * Don't do that, be nice and tell your users that things didn't go the
     * way you wanted it to.
     *
     * \rst
     * .. note::
     *  If the code is compiled for Windows, it'll use WinSock2 as the resident
     *  socket library. This, contrary to POSIX socket implementations, requires
     *  an initializer and finisher function call (``WSAStartup`` and ``WSACleanup``).
     *  If the executable relying on this library also wishes to deal with WinSock2,
     *  they should call ``WSAStartup`` before instantiating an object from this class
     *  and call ``WSACleanup`` after the object has been destructed. While
     *  this may not be necessary, behavior may be surprising when the call to ``WSACleanup``
     *  does nothing and the executable's sockets only get cleaned when this object
     *  gets destructed.
     * \endrst
     *
     * \rst
     * .. warning::
     *    Currently, SSL is not supported, it's in the works,
     *    but don't use this to connect through public networks yet.
     * \endrst
     *
     * \since v1.0.0
     */
    struct MIKROTIK_API_EXPORT api_handler {
        /**
         * \brief Sends a \ref sentence through the open connection
         *
         * Takes a \ref sentence and sends it through the open socket to the
         * MikroTik device.
         * The \ref sentence does not need to be terminated with the empty word,
         * the function will always send it after sending the contents of the
         * sentence.
         *
         * \rst
         * .. warning::
         *  This function does not deal with the reply just sends the sentence
         *  and it's done with it.
         *  To ensure no reply data gets lost, or dislocated from its sent sentence,
         *  always make sure to call read after using this function to get the device's reply.
         * \endrst
         *
         * \param snt The sentence to send
         *
         * \since v1.0.0
         */
        void send(const sentence& snt);

        /**
         * \brief Reads the reply sentence from the connection
         *
         * Reads a reply sentence from the open connection. This contains
         * the response type, and an optional list of response contents stored
         * as strings.
         *
         * For example let's see the following exchange:
         * \code{.unparsed}
         * >>> /user/print
         * >>>
         * <<< !re
         * <<< =.id=*1
         * <<< =name=admin
         * <<< =group=full
         * <<< =address=
         * <<< =last-logged-in=jun/29/2020 23:22:47
         * <<< =disabled=false
         * <<< =comment=system default user
         * <<< !done
         * \endcode
         * The lines beginning with `>>>` are the words of the sent sentence,
         * while lines with a leading `<<<` are the words of the reply sentence.
         *
         * There are two reply sentences here. A reply sentence will contain the
         * response type and a vector of strings. The vector will contain
         * the meaningful content of the `!re` reply sentence. (`{"=.id=*1", "=name=admin", etc...}`)
         *
         * More about what is a  \ref sentence, a \ref reply sentence, and their differences in their
         * appropriate documentation.
         *
         * \return The reply from the MikroTik device
         *
         * \since v1.0.0
         */
        mikrotik::api::reply read();

        /**
         * \brief Disconnects from the MikroTik device
         *
         * Implements the disconnection procedure to end the communication
         * with the MikroTik device. The destructor relies on this function
         * to call the appropriate syscalls to kill the connection.
         *
         *
         * On POSIX socket implementations, this closes the socket.
         *
         * On WinSock2, this closes the socket and calls the cleanup
         * function ``WSACleanup``.
         *
         * \rst
         * .. note:
         *  The function does not throw to make call suitable in a destructor
         *  where we don't like throwing.
         *  To check for errors see the return value.
         * \endrst
         *
         * \return The return value from the function that closes the socket.
         *  (``close`` or ``closesocket``).
         *
         * \since v1.0.0
         */
        int disconnect() noexcept;

        /**
         * \brief Connects to and logs into the MikroTik device at the specified
         * address.
         *
         * Establishes a socket connection with the device, then sends the
         * provided user credentials. If the IP address is invalid, the program
         * will halt until the resident socket implementation decides to give
         * up with a timeout error, then throws. If the credentials are incorrect
         * the function throws.
         *
         * \throw bad_ip_format: If the provided IP address does not follow
         * the x.x.x.x format. IPv6 is currently not supported.
         * \throw bad_socket: If an error arises from the resident socket
         * implementation: cannot create socket, cannot connect, or cannot send/read
         * data.
         *
         * \param address The IPv4 address of the MikroTik device to connect to.
         * \param user The username to log in as
         * \param pass The password of the provided user
         *
         * \since v1.0.0
         */
        explicit api_handler(ip_address address = "192.168.88.1",
                             std::string_view user = "admin",
                             std::string_view pass = "");

        /**
         * \brief Destructor that terminates connection
         *
         * When the object is destroyed, the connection to the
         * MikroTik device is terminated, which in turn causes
         * the user to log out as well.
         *
         * \rst
         * .. warning::
         *
         *  To make the destructor not throw, since that can case
         *  interesting problems, the disconnect() member function which implements
         *  the termination of the connection does not throw.
         *  If you think there is a chance of the connection not getting terminated
         *  properly, and there is something you can do about that, call the
         *  disconnect() member function directly, which returns the return value
         *  of the call that closed the socket.
         *  Otherwise, any errors are silently discarded by the destructor.
         * \endrst
         *
         * \since v1.0.0
         */
        virtual ~api_handler() noexcept;

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
        void login(std::string_view usr, std::string_view passwd);
    };
}
