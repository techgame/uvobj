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
#ifdef _UVOBJ_EXAMPLES_
    struct FSEventSink {
        typedef uvObj::FS_Event uvobj_t;
        typedef uvObj::events_t<FSEventSink, uvobj_t> evt;

        // void on_fs_event(const char* filename, int events, int status) {}
        // void on_fs_event(uvobj_t& fsEvt, const char* filename, int events, int status) {}

        // void on_close() {}
        // void on_close(uvobj_t& obj) {}
    };

    struct FSSink {
        typedef uvObj::FS uvobj_t;
        typedef uvObj::events_t<FSSink, uvobj_t> evt;

        // void on_fs(uv_fs_t* req) {}
        // void on_fs(uvobj_t& obj, uv_fs_t* req) {}

        // void on_close() {}
        // void on_close(uvobj_t& obj) {}
    };

    struct WorkSink {
        typedef uvObj::Work uvobj_t;
        typedef uvObj::events_t<WorkSink, uvobj_t> evt;

        // void on_work() {}
        // void on_work(uvobj_t& obj) {}

        // void on_after_work() {}
        // void on_after_work(uvobj_t& obj) {}

        // void on_close() {}
        // void on_close(uvobj_t& obj) {}
    };

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

        // void on_close() {}
        // void on_close(uvobj_t& obj) {}
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
}
