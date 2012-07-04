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

struct FS_EventSink {
    typedef uvObj::FS_Event uvobj_t;
    typedef uvObj::events_t<FS_EventSink, uvobj_t> evt;

    // void on_fs_event(const char* filename, int events, int status) {}
    // void on_fs_event(uvobj_t& fsEvt, const char* filename, int events, int status) {}

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

struct FS_PollSink {
    typedef uvObj::FS_Poll uvobj_t;
    typedef uvObj::events_t<FS_PollSink, uvobj_t> evt;

    // void on_fs_event(const char* filename, int events, int status) {}
    // void on_fs_event(uvobj_t& fsEvt, const char* filename, int events, int status) {}

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

struct FS_Sink {
    typedef uvObj::FS uvobj_t;
    typedef uvObj::req_events_t<FS_Sink> evt;

    // void on_fs_open(uv_fs_t* req) {}
    // void on_fs_close(uv_fs_t* req) {}
    // void on_fs_read(uv_fs_t* req) {}
    // void on_fs_write(uv_fs_t* req) {}
    // void on_fs_sendfile(uv_fs_t* req) {}
    // void on_fs_ftruncate(uv_fs_t* req) {}
    // void on_fs_fsync(uv_fs_t* req) {}
    // void on_fs_fdatasync(uv_fs_t* req) {}
    // void on_fs_stat(uv_fs_t* req) {}
    // void on_fs_fstat(uv_fs_t* req) {}
    // void on_fs_lstat(uv_fs_t* req) {}
    // void on_fs_utime(uv_fs_t* req) {}
    // void on_fs_futime(uv_fs_t* req) {}
    // void on_fs_chmod(uv_fs_t* req) {}
    // void on_fs_fchmod(uv_fs_t* req) {}
    // void on_fs_chown(uv_fs_t* req) {}
    // void on_fs_fchown(uv_fs_t* req) {}
    // void on_fs_rename(uv_fs_t* req) {}
    // void on_fs_readdir(uv_fs_t* req) {}
    // void on_fs_mkdir(uv_fs_t* req) {}
    // void on_fs_rmdir(uv_fs_t* req) {}
    // void on_fs_unlink(uv_fs_t* req) {}
    // void on_fs_link(uv_fs_t* req) {}
    // void on_fs_symlink(uv_fs_t* req) {}
    // void on_fs_readlink(uv_fs_t* req) {}
};

#endif
