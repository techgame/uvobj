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

struct ConnectSink {
    typedef uvObj::Connect uvobj_t;
    typedef uvObj::req_events_t<ConnectSink> evt;

    // void on_connect(uv_connect_t* req, int status) { delete this; }
};

struct ShutdownSink {
    typedef uvObj::Shutdown uvobj_t;
    typedef uvObj::req_events_t<ShutdownSink> evt;

    // void on_shutdown(uv_shutdown_t* req, int status) { delete this; }
};

struct UDPSendSink {
    typedef uvObj::UDPSend uvobj_t;
    typedef uvObj::req_events_t<UDPSendSink> evt;

    // void on_udp_send(uv_udp_send_t* req, int status) { delete this; }
};

struct WriteSink {
    typedef uvObj::Write uvobj_t;
    typedef uvObj::req_events_t<WriteSink> evt;

    // void on_write(uv_write_t* req, int status) { delete this; }
};

struct WorkSink {
    typedef uvObj::Work uvobj_t;
    typedef uvObj::req_events_t<WorkSink> evt;

    // void on_work() {}
    // void on_after_work() { delete this; }
};

struct GetAddrInfoSink {
    typedef uvObj::GetAddrInfo uvobj_t;
    typedef uvObj::req_events_t<GetAddrInfoSink> evt;

    // void on_getaddrinfo(uv_getaddrinfo_t* req, int status, struct addrinfo* res) { delete this; }
};

#endif
