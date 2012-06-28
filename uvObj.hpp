/* -*- coding: utf-8 -*- vim: set ts=4 sw=4 expandtab */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##
##~ Copyright (C) 2002-2012  TechGame Networks, LLC.              ##
##~                                                               ##
##~ This library is free software; you can redistribute it        ##
##~ and/or modify it under the terms of the MIT style License as  ##
##~ found in the LICENSE file included with this distribution.    ##
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma once
#include "./uvObj_fs.hpp"
#include "./uvObj_loop.hpp"
#include "./uvObj_handles.hpp"
#include "./uvObj_chan.hpp"

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

    template <typename endpoint_t>
    struct Root {
        typedef typename endpoint_t::api_t api_t;
        api_t obj;

        explicit Root(endpoint_t* ep=NULL) : obj() { _initSelf(ep); }

        template <typename a1_t>
        Root(a1_t a1) : obj(a1) { _initSelf(); }
        template <typename a1_t, typename a2_t>
        Root(a1_t a1, a2_t a2) : obj(a1, a2) { _initSelf(); }
        template <typename a1_t, typename a2_t, typename a3_t>
        Root(a1_t a1, a2_t a2, a3_t a3) : obj(a1, a2, a3) { _initSelf(); }
        template <typename a1_t, typename a2_t, typename a3_t, typename a4_t>
        Root(a1_t a1, a2_t a2, a3_t a3, a4_t a4) : obj(a1, a2, a3, a4) { _initSelf(); }

        void _initSelf(endpoint_t* ep=NULL) {
            if (!ep) ep = new endpoint_t;
            obj.setSelf(ep); }

        inline operator api_t* () { return &obj; }
        inline api_t* operator->() { return &obj; }
        inline api_t& operator()() { return obj; }

        inline endpoint_t* ep() { return obj.self(); }
        inline operator endpoint_t*() { return obj.self(); }
    };
}

