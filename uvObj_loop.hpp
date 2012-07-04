/* -*- coding: utf-8 -*- vim: set ts=4 sw=4 expandtab */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##
##~ Copyright (C) 2002-2012  TechGame Networks, LLC.              ##
##~                                                               ##
##~ This library is free software; you can redistribute it        ##
##~ and/or modify it under the terms of the MIT style License as  ##
##~ found in the LICENSE file included with this distribution.    ##
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma once
#include "./uvobj_base.hpp"

namespace uvObj {
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

        inline operator uv_loop_t* () { return _loop; }

        uv_err_t last_error() { return uv_last_error(_loop); }
        const char* last_strerror() { return uv_strerror(last_error()); }
        const char* strerror(uv_err_t err) { return uv_strerror(err); }
        const char* last_err_name() { return uv_err_name(last_error()); }
        const char* err_name(uv_err_t err) { return uv_err_name(err); }


        void run() { uvResult( uv_run(_loop), _loop); }
        void run_once() { uvResult( uv_run_once(_loop), _loop); }

        void update_time() { uv_update_time(_loop); }
        int64_t now() { return uv_now(_loop); }
        uint64_t hrtime() { return uv_hrtime(); }

        void walk(uv_walk_cb walk_cb, void* arg) {
            uv_walk(_loop, walk_cb, arg); }
    };
}

