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


/*~ Handle Sinks ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct PollSink {
    typedef uvObj::evt_t<PollSink> evt;
    typedef uvObj::Poll uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_poll(uv_poll_t* handle, int status, int events) { return true; }
};

struct ProcessSink {
    typedef uvObj::evt_t<ProcessSink> evt;
    typedef uvObj::Process uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_exit(uv_process_t* handle, int exit_status, int term_signal) { return true; }
};

struct PrepareSink {
    typedef uvObj::evt_t<PrepareSink> evt;
    typedef uvObj::Prepare uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_prepare(uv_prepare_t* handle, int status) { return true; }
};

struct CheckSink {
    typedef uvObj::evt_t<CheckSink> evt;
    typedef uvObj::Check uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_check(uv_check_t* handle, int status) { return true; }
};

struct IdleSink {
    typedef uvObj::evt_t<IdleSink> evt;
    typedef uvObj::Idle uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_idle(uv_idle_t* handle, int status) { return true; }
};

struct AsyncSink {
    typedef uvObj::evt_t<AsyncSink> evt;
    typedef uvObj::Async uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_async(uv_async_t* handle, int status) { return true; }
};

struct TimerSink {
    typedef uvObj::evt_t<TimerSink> evt;
    typedef uvObj::Timer uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_timer(uv_timer_t* handle, int status) { return true; }
};


/*~ Channel Sinks ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct UDPSink {
    typedef uvObj::evt_t<UDPSink> evt;
    typedef uvObj::UDP uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size);
    bool on_udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags);
};

struct TCPSink {
    typedef uvObj::evt_t<TCPSink> evt;
    typedef uvObj::TCP uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_connection(uv_stream_t* stream, int status) { return true; }

    uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size);
    bool on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) { return true; }
};

struct PipeSink {
    typedef uvObj::evt_t<PipeSink> evt;
    typedef uvObj::Pipe uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_connection(uv_stream_t* stream, int status) { return true; }

    uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size);
    bool on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) { return true; }
    bool on_read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) { return true; }
};

struct TTYSink {
    typedef uvObj::evt_t<TTYSink> evt;
    typedef uvObj::TTY uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_connection(uv_stream_t* stream, int status) { return true; }

    uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size);
    bool on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) { return true; }
};


/*~ Request Sinks ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct ConnectSink {
    typedef uvObj::evt_t<ConnectSink> evt;
    bool on_connect(uv_connect_t* req, int status) { return true; }
};

struct ShutdownSink {
    typedef uvObj::evt_t<ShutdownSink> evt;
    bool on_shutdown(uv_shutdown_t* req, int status) { return true; }
};

struct UDPSendSink {
    typedef uvObj::evt_t<UDPSendSink> evt;
    bool on_udp_send(uv_udp_send_t* req, int status) { return true; }
};

struct WriteSink {
    typedef uvObj::evt_t<WriteSink> evt;
    bool on_write(uv_write_t* req, int status) { return true; }
};

struct WorkSink {
    typedef uvObj::evt_t<WorkSink> evt;
    bool on_work(uv_work_t* req) { return true; }
    bool on_after_work(uv_work_t* req) { return true; }
};

struct GetAddrInfoSink {
    typedef uvObj::evt_t<GetAddrInfoSink> evt;
    bool on_getaddrinfo(uv_getaddrinfo_t* handle, int status, struct addrinfo* res) { return true; }
};



/*~ Filesystem Sinks ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct FSEventSink {
    typedef uvObj::evt_t<FSEventSink> evt;
    typedef uvObj::FSEvent uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_fs_event(uv_fs_event_t* handle, const char* filename, int events, int status) { return true; }
};

struct FSPollSink {
    typedef uvObj::evt_t<FSPollSink> evt;
    typedef uvObj::FSPoll uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_fs_poll(uv_fs_poll_t* handle, int status, const uv_statbuf_t* prev, const uv_statbuf_t* curr) { return true; }
};

struct FSSink {
    typedef uvObj::evt_t<FSSink> evt;
    typedef uvObj::FS uvref;

    bool on_close(uv_handle_t* handle) { return true; }
    bool on_fs(uv_fs_t* req) { return true; }
};

#endif
