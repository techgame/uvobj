/* -*- coding: utf-8 -*- vim: set ts=4 sw=4 expandtab */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##
##~ Copyright (C) 2002-2012  TechGame Networks, LLC.              ##
##~                                                               ##
##~ This library is free software; you can redistribute it        ##
##~ and/or modify it under the terms of the MIT style License as  ##
##~ found in the LICENSE file included with this distribution.    ##
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma once
#include "./uvObj_base.hpp"
#include "./uvObj_loop.hpp"
#include "./uvObj_handles.hpp"
#include "./uvObj_channels.hpp"
#include "./uvObj_extensions.hpp"
#include "./uvObj_filesystem.hpp"

namespace uvObj {
    /*
    struct Endpoint {
        typedef uvObj::XYZ<Endpoint> api_t;

        void on_close(api_t& obj) {}
        uv_buf_t on_alloc(api_t& obj, size_t suggested_size) {}
        void on_udp_recv(api_t& obj, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {}
        void on_udp_send(api_t& obj, int status) {}
        void on_connection(api_t& obj, int status) {}
        void on_connect(api_t& obj, int status) {}
        void on_read(api_t& obj, ssize_t nread, uv_buf_t buf) {}
        void on_read2(api_t& obj, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {}
        void on_write(api_t& obj, int status) {}
        void on_shutdown(api_t& obj, int status) {}
        void on_exit(api_t& obj, int exit_status, int term_signal) {}
        void on_work(api_t& obj) {}
        void on_after_work(api_t& obj) {}
        void on_getaddrinfo(api_t& obj, int status, struct addrinfo* res) {}
        void on_poll(api_t& obj, int status, int events) {}
        void on_prepare(api_t& obj, int status) {}
        void on_check(api_t& obj, int status) {}
        void on_idle(api_t& obj, int status) {}
        void on_async(api_t& obj, int status) {}
        void on_timer(api_t& obj, int status) {}
    };
    */
}

