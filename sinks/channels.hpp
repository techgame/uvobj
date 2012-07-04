/* -*- coding: utf-8 -*- vim: set ts=4 sw=4 expandtab */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##
##~ Copyright (C) 2002-2012  TechGame Networks, LLC.              ##
##~                                                               ##
##~ This library is free software; you can redistribute it        ##
##~ and/or modify it under the terms of the MIT style License as  ##
##~ found in the LICENSE file included with this distribution.    ##
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma once
#ifdef _UVOBJ_EXAMPLES_

#include <uvObj/uvObj.hpp>

struct UDPSink {
    typedef uvObj::UDP uvobj_t;
    typedef uvObj::events_t<UDPSink, uvobj_t> evt;

    // void on_alloc(size_t suggested_size) {}
    // void on_alloc(uvobj_t& obj, size_t suggested_size) {}

    // void on_recv(ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {}
    // void on_recv(uvobj_t& obj, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {}
};

struct StreamSink {
    // void on_connection(int status) {}
    // void on_connection(uvobj_t& obj, int status) {}

    // void on_alloc(size_t suggested_size) {}
    // void on_alloc(uvobj_t& obj, size_t suggested_size) {}

    // void on_read(ssize_t nread, uv_buf_t buf) {}
    // void on_read(uvobj_t& obj, ssize_t nread, uv_buf_t buf) {}

    // void on_read2(ssize_t nread, uv_buf_t buf, uv_handle_type pending) {}
    // void on_read2(uvobj_t& obj, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {}

    // void on_write(uv_write_t* req, int status) {}
    // void on_write(uvobj_t& obj, uv_write_t* req, int status) {}

    // void on_shutdown(uv_shutdown_t* req, int status) {}
    // void on_shutdown(uvobj_t& obj, uv_shutdown_t* req, int status) {}

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

struct TCPSink {
    typedef uvObj::TCP uvobj_t;
    typedef uvObj::events_t<TCPSink, uvobj_t> evt;

    // void on_connect(uv_connect_t* req, int status) {}
    // void on_connect(uvobj_t& obj, uv_connect_t* req, int status) {}

    // + StreamSink +
};

struct TTYSink {
    typedef uvObj::TTY uvobj_t;
    typedef uvObj::events_t<TTYSink, uvobj_t> evt;

    // + StreamSink +
};

struct PipeSink {
    typedef uvObj::Pipe uvobj_t;
    typedef uvObj::events_t<PipeSink, uvobj_t> evt;

    // void on_connect(uv_connect_t* req, int status) {}
    // void on_connect(uvobj_t& obj, uv_connect_t* req, int status) {}

    // + StreamSink +
};

#endif
