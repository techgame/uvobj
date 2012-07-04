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

struct PollSink {
    typedef uvObj::Poll uvobj_t;
    typedef uvObj::events_t<PollSink, uvobj_t> evt;

    // void on_poll(int status, int events);
    // void on_poll(uvobj_t& obj, int status, int events);

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

struct ProcessSink {
    typedef uvObj::Process uvobj_t;
    typedef uvObj::events_t<ProcessSink, uvobj_t> evt;

    // void on_exit(int exit_status, int term_signal);
    // void on_exit(uvobj_t& obj, int exit_status, int term_signal);

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

struct PrepareSink {
    typedef uvObj::Prepare uvobj_t;
    typedef uvObj::events_t<PrepareSink, uvobj_t> evt;

    // void on_prepare(int status);
    // void on_prepare(uvobj_t& obj, int status);

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

struct CheckSink {
    typedef uvObj::Check uvobj_t;
    typedef uvObj::events_t<CheckSink, uvobj_t> evt;

    // void on_check(int status);
    // void on_check(uvobj_t& obj, int status);

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

struct IdleSink {
    typedef uvObj::Idle uvobj_t;
    typedef uvObj::events_t<IdleSink, uvobj_t> evt;

    // void on_idle(int status);
    // void on_idle(uvobj_t& obj, int status);

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

struct AsyncSink {
    typedef uvObj::Async uvobj_t;
    typedef uvObj::events_t<AsyncSink, uvobj_t> evt;

    // void on_async(int status);
    // void on_async(uvobj_t& obj, int status);

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

struct TimerSink {
    typedef uvObj::Timer uvobj_t;
    typedef uvObj::events_t<TimerSink, uvobj_t> evt;

    // void on_timer(int status);
    // void on_timer(uvobj_t& obj, int status);

    // void on_close() { delete this; }
    // void on_close(uvobj_t& obj) { delete this; }
};

#endif
