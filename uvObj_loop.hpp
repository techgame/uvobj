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

namespace uvObj {
    static void initLibrary() {
        assert(sizeof(uv_tcp_t) == uv_handle_size(UV_TCP));
        assert(sizeof(uv_udp_t) == uv_handle_size(UV_UDP));
        assert(sizeof(uv_pipe_t) == uv_handle_size(UV_NAMED_PIPE));
        assert(sizeof(uv_tty_t) == uv_handle_size(UV_TTY));
        assert(sizeof(uv_poll_t) == uv_handle_size(UV_POLL));
        assert(sizeof(uv_timer_t) == uv_handle_size(UV_TIMER));
        assert(sizeof(uv_prepare_t) == uv_handle_size(UV_PREPARE));
        assert(sizeof(uv_check_t) == uv_handle_size(UV_CHECK));
        assert(sizeof(uv_idle_t) == uv_handle_size(UV_IDLE));
        assert(sizeof(uv_async_t) == uv_handle_size(UV_ASYNC));
        assert(sizeof(uv_process_t) == uv_handle_size(UV_PROCESS));
        assert(sizeof(uv_fs_event_t) == uv_handle_size(UV_FS_EVENT));
        //assert(sizeof(uv_fs_poll_t) == uv_handle_size(UV_FS_POLL)); // ABI-adaptation code in definition

        assert(sizeof(uv_getaddrinfo_t) == uv_req_size(UV_GETADDRINFO));
        assert(sizeof(uv_shutdown_t) == uv_req_size(UV_SHUTDOWN));
        assert(sizeof(uv_write_t) == uv_req_size(UV_WRITE));
        assert(sizeof(uv_connect_t) == uv_req_size(UV_CONNECT));
        assert(sizeof(uv_udp_send_t) == uv_req_size(UV_UDP_SEND));
        assert(sizeof(uv_fs_t) == uv_req_size(UV_FS)); // sizeof(uv_fs_t) does not match size reported by library -- check _DARWIN_FEATURE_64_BIT_INODE
        assert(sizeof(uv_work_t) == uv_req_size(UV_WORK));

        uv_default_loop();
        uv_disable_stdio_inheritance();
    }

    struct UVLoop {
        uv_loop_t* _loop;
        bool _ownsLoop;
        UVLoop(bool createLoop=false)
         : _loop(createLoop ? uv_loop_new() : uv_default_loop()),
           _ownsLoop(createLoop) {}
        UVLoop(uv_loop_t* loop, bool owns=false)
         : _loop(loop), _ownsLoop(owns) {}

        ~UVLoop() {
            if (_ownsLoop) uv_loop_delete(_loop);
            _loop = NULL; }

        operator uv_loop_t* () { return _loop; }

        uv_err_t last_error() { return uv_last_error(_loop); }
        const char* last_strerror() { return uv_strerror(last_error()); }
        const char* strerror(uv_err_t err) { return uv_strerror(err); }
        const char* last_err_name() { return uv_err_name(last_error()); }
        const char* err_name(uv_err_t err) { return uv_err_name(err); }


        int run() { return uv_run(_loop); }
        int run_once() { return uv_run_once(_loop); }

        void update_time() { uv_update_time(_loop); }
        int64_t now() { return uv_now(_loop); }
        uint64_t hrtime() { return uv_hrtime(); }

        void walk(uv_walk_cb walk_cb, void* arg) {
            uv_walk(_loop, walk_cb, arg); }
        void walk(const BoundEvt<uv_walk_cb>& evt) {
            walk(evt.cb, evt.tgt); }
    };

    template <typename V>
    struct WalkVisitor {
        V* visitor;
        WalkVisitor(V* visitor_, uv_loop_t* loop=NULL)
         : visitor(visitor_)
        { UVLoop(loop).walk(this); }

        bool on_walk(uv_handle_t* handle) {
            switch (handle->type) {
            case UV_UNKNOWN_HANDLE:
                visitor->visit_unknown(handle); return true;

            #define XX(uc, lc) \
            case UV_##uc: \
                visitor->visit_##lc(reinterpret_cast<uv_##lc##_t*>(handle)); return true;

            UV_HANDLE_TYPE_MAP(XX);
            XX(ARES_TASK, ares_task);
            //XX(FILE, file); -- not yet supported
            case UV_FILE: visitor->visit_file(handle); return true;
            #undef XX
            default: return true; }}
    };
}
