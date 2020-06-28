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

#include "impl/socket_funcs.hpp"
#include <mikrotik/api/impl/sockets.hpp>

bool
mikrotik::api::impl::socket::is_valid(handle sock) noexcept {
    return sock != INVALID_SOCKET;
}

int
mikrotik::api::impl::socket::init() noexcept {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data);
}

int
mikrotik::api::impl::socket::finish() noexcept {
    return WSACleanup();
}

int
mikrotik::api::impl::socket::close(handle sock) noexcept {
    int ret = shutdown(sock, SD_BOTH);
    if (ret == 0) {
        return closesocket(sock);
    }
    return ret;
}

int
mikrotik::api::impl::socket::get_last_error() noexcept {
    return WSAGetLastError();
}

std::string_view
mikrotik::api::impl::socket::string_error(int err) noexcept {
    switch (err) {
        // set of error-codes from https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2
    case WSA_INVALID_HANDLE:
        return "WSA_INVALID_HANDLE";
    case WSA_NOT_ENOUGH_MEMORY:
        return "WSA_NOT_ENOUGH_MEMORY";
    case WSA_INVALID_PARAMETER:
        return "WSA_INVALID_PARAMETER";
    case WSA_OPERATION_ABORTED:
        return "WSA_OPERATION_ABORTED";
    case WSA_IO_INCOMPLETE:
        return "WSA_IO_INCOMPLETE";
    case WSA_IO_PENDING:
        return "WSA_IO_PENDING";
    case WSAEINTR:
        return "WSAEINTR";
    case WSAEBADF:
        return "WSAEBADF";
    case WSAEACCES:
        return "WSAEACCES";
    case WSAEFAULT:
        return "WSAEFAULT";
    case WSAEINVAL:
        return "WSAEINVAL";
    case WSAEMFILE:
        return "WSAEMFILE";
    case WSAEWOULDBLOCK:
        return "WSAEWOULDBLOCK";
    case WSAEINPROGRESS:
        return "WSAEINPROGRESS";
    case WSAEALREADY:
        return "WSAEALREADY";
    case WSAENOTSOCK:
        return "WSAENOTSOCK";
    case WSAEDESTADDRREQ:
        return "WSAEDESTADDRREQ";
    case WSAEMSGSIZE:
        return "WSAEMSGSIZE";
    case WSAEPROTOTYPE:
        return "WSAEPROTOTYPE";
    case WSAENOPROTOOPT:
        return "WSAENOPROTOOPT";
    case WSAEPROTONOSUPPORT:
        return "WSAEPROTONOSUPPORT";
    case WSAESOCKTNOSUPPORT:
        return "WSAESOCKTNOSUPPORT";
    case WSAEOPNOTSUPP:
        return "WSAEOPNOTSUPP";
    case WSAEPFNOSUPPORT:
        return "WSAEPFNOSUPPORT";
    case WSAEAFNOSUPPORT:
        return "WSAEAFNOSUPPORT";
    case WSAEADDRINUSE:
        return "WSAEADDRINUSE";
    case WSAEADDRNOTAVAIL:
        return "WSAEADDRNOTAVAIL";
    case WSAENETDOWN:
        return "WSAENETDOWN";
    case WSAENETUNREACH:
        return "WSAENETUNREACH";
    case WSAENETRESET:
        return "WSAENETRESET";
    case WSAECONNABORTED:
        return "WSAECONNABORTED";
    case WSAECONNRESET:
        return "WSAECONNRESET";
    case WSAENOBUFS:
        return "WSAENOBUFS";
    case WSAEISCONN:
        return "WSAEISCONN";
    case WSAENOTCONN:
        return "WSAENOTCONN";
    case WSAESHUTDOWN:
        return "WSAESHUTDOWN";
    case WSAETOOMANYREFS:
        return "WSAETOOMANYREFS";
    case WSAETIMEDOUT:
        return "WSAETIMEDOUT";
    case WSAECONNREFUSED:
        return "WSAECONNREFUSED";
    case WSAELOOP:
        return "WSAELOOP";
    case WSAENAMETOOLONG:
        return "WSAENAMETOOLONG";
    case WSAEHOSTDOWN:
        return "WSAEHOSTDOWN";
    case WSAEHOSTUNREACH:
        return "WSAEHOSTUNREACH";
    case WSAENOTEMPTY:
        return "WSAENOTEMPTY";
    case WSAEPROCLIM:
        return "WSAEPROCLIM";
    case WSAEUSERS:
        return "WSAEUSERS";
    case WSAEDQUOT:
        return "WSAEDQUOT";
    case WSAESTALE:
        return "WSAESTALE";
    case WSAEREMOTE:
        return "WSAEREMOTE";
    case WSASYSNOTREADY:
        return "WSASYSNOTREADY";
    case WSAVERNOTSUPPORTED:
        return "WSAVERNOTSUPPORTED";
    case WSANOTINITIALISED:
        return "WSANOTINITIALISED";
    case WSAEDISCON:
        return "WSAEDISCON";
    case WSAENOMORE:
        return "WSAENOMORE";
    case WSAECANCELLED:
        return "WSAECANCELLED";
    case WSAEINVALIDPROCTABLE:
        return "WSAEINVALIDPROCTABLE";
    case WSAEINVALIDPROVIDER:
        return "WSAEINVALIDPROVIDER";
    case WSAEPROVIDERFAILEDINIT:
        return "WSAEPROVIDERFAILEDINIT";
    case WSASYSCALLFAILURE:
        return "WSASYSCALLFAILURE";
    case WSASERVICE_NOT_FOUND:
        return "WSASERVICE_NOT_FOUND";
    case WSATYPE_NOT_FOUND:
        return "WSATYPE_NOT_FOUND";
    case WSA_E_NO_MORE:
        return "WSA_E_NO_MORE";
    case WSA_E_CANCELLED:
        return "WSA_E_CANCELLED";
    case WSAEREFUSED:
        return "WSAEREFUSED";
    case WSAHOST_NOT_FOUND:
        return "WSAHOST_NOT_FOUND";
    case WSATRY_AGAIN:
        return "WSATRY_AGAIN";
    case WSANO_RECOVERY:
        return "WSANO_RECOVERY";
    case WSANO_DATA:
        return "WSANO_DATA";
    case WSA_QOS_RECEIVERS:
        return "WSA_QOS_RECEIVERS";
    case WSA_QOS_SENDERS:
        return "WSA_QOS_SENDERS";
    case WSA_QOS_NO_SENDERS:
        return "WSA_QOS_NO_SENDERS";
    case WSA_QOS_NO_RECEIVERS:
        return "WSA_QOS_NO_RECEIVERS";
    case WSA_QOS_REQUEST_CONFIRMED:
        return "WSA_QOS_REQUEST_CONFIRMED";
    case WSA_QOS_ADMISSION_FAILURE:
        return "WSA_QOS_ADMISSION_FAILURE";
    case WSA_QOS_POLICY_FAILURE:
        return "WSA_QOS_POLICY_FAILURE";
    case WSA_QOS_BAD_STYLE:
        return "WSA_QOS_BAD_STYLE";
    case WSA_QOS_BAD_OBJECT:
        return "WSA_QOS_BAD_OBJECT";
    case WSA_QOS_TRAFFIC_CTRL_ERROR:
        return "WSA_QOS_TRAFFIC_CTRL_ERROR";
    case WSA_QOS_GENERIC_ERROR:
        return "WSA_QOS_GENERIC_ERROR";
    case WSA_QOS_ESERVICETYPE:
        return "WSA_QOS_ESERVICETYPE";
    case WSA_QOS_EFLOWSPEC:
        return "WSA_QOS_EFLOWSPEC";
    case WSA_QOS_EPROVSPECBUF:
        return "WSA_QOS_EPROVSPECBUF";
    case WSA_QOS_EFILTERSTYLE:
        return "WSA_QOS_EFILTERSTYLE";
    case WSA_QOS_EFILTERTYPE:
        return "WSA_QOS_EFILTERTYPE";
    case WSA_QOS_EFILTERCOUNT:
        return "WSA_QOS_EFILTERCOUNT";
    case WSA_QOS_EOBJLENGTH:
        return "WSA_QOS_EOBJLENGTH";
    case WSA_QOS_EFLOWCOUNT:
        return "WSA_QOS_EFLOWCOUNT";
    case WSA_QOS_EUNKOWNPSOBJ:
        return "WSA_QOS_EUNKOWNPSOBJ";
    case WSA_QOS_EPOLICYOBJ:
        return "WSA_QOS_EPOLICYOBJ";
    case WSA_QOS_EFLOWDESC:
        return "WSA_QOS_EFLOWDESC";
    case WSA_QOS_EPSFLOWSPEC:
        return "WSA_QOS_EPSFLOWSPEC";
    case WSA_QOS_EPSFILTERSPEC:
        return "WSA_QOS_EPSFILTERSPEC";
    case WSA_QOS_ESDMODEOBJ:
        return "WSA_QOS_ESDMODEOBJ";
    case WSA_QOS_ESHAPERATEOBJ:
        return "WSA_QOS_ESHAPERATEOBJ";
    case WSA_QOS_RESERVED_PETYPE:
        return "WSA_QOS_RESERVED_PETYPE";
    default:
        return "unrecognized error";
    }
}
