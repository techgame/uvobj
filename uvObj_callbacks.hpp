/* -*- coding: utf-8 -*- vim: set ts=4 sw=4 expandtab */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##
##~ Copyright (C) 2002-2012  TechGame Networks, LLC.              ##
##~                                                               ##
##~ This library is free software; you can redistribute it        ##
##~ and/or modify it under the terms of the MIT style License as  ##
##~ found in the LICENSE file included with this distribution.    ##
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma once

namespace uvObj {
    enum ref_mode_t { from_ref=3 };

    template <typename uvobj_t>
    struct events_t {
        // -- typedef void (*uv_close_cb)(uv_handle_t* handle);
        static void on_close(uv_handle_t* handle) {
            uvobj_t obj(from_ref, handle);
            obj.self()->on_close(obj); }

        // -- typedef uv_buf_t (*uv_alloc_cb)(uv_handle_t* handle, size_t suggested_size);
        static uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size) {
            uvobj_t obj(from_ref, handle);
            return obj.self()->on_alloc(obj, suggested_size); }

        // -- typedef void (*uv_udp_recv_cb)(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags);
        static void on_udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {
            uvobj_t obj(from_ref, handle);
            return obj.self()->on_udp_recv(obj, nread, buf, addr, flags); }

        // -- typedef void (*uv_udp_send_cb)(uv_udp_send_t* req, int status);
        static void on_udp_send(uv_udp_send_t* req, int status) {
            uvobj_t obj(from_ref, req->handle);
            return obj.self()->on_udp_send(obj, req, status); }

        // -- typedef void (*uv_connection_cb)(uv_stream_t* server, int status);
        static void on_connection(uv_stream_t* server, int status) {
            uvobj_t obj(from_ref, server);
            return obj.self()->on_connection(obj, status); }

        // -- typedef void (*uv_connect_cb)(uv_connect_t* req, int status);
        static void on_connect(uv_connect_t* req, int status) {
            uvobj_t obj(from_ref, req->handle);
            return obj.self()->on_connect(obj, req, status); }

        // -- typedef void (*uv_read_cb)(uv_stream_t* stream, ssize_t nread, uv_buf_t buf);
        static void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
            uvobj_t obj(from_ref, stream);
            return obj.self()->on_read(obj, nread, buf); }

        // -- typedef void (*uv_read2_cb)(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
        static void on_read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {
            uvobj_t obj(from_ref, pipe);
            return obj.self()->on_read2(obj, nread, buf, pending); }

        // -- typedef void (*uv_write_cb)(uv_write_t* req, int status);
        static void on_write(uv_write_t* req, int status) {
            uvobj_t obj(from_ref, req->handle);
            return obj.self()->on_write(obj, req, status); }

        // -- typedef void (*uv_shutdown_cb)(uv_shutdown_t* req, int status);
        static void on_shutdown(uv_shutdown_t* req, int status) {
            uvobj_t obj(from_ref, req->handle);
            return obj.self()->on_shutdown(obj, req, status); }

        // -- typedef void (*uv_exit_cb)(uv_process_t*, int exit_status, int term_signal);
        static void on_exit(uv_process_t* proc, int exit_status, int term_signal) {
            uvobj_t obj(from_ref, proc);
            return obj.self()->on_exit(obj, exit_status, term_signal); }

        // -- typedef void (*uv_work_cb)(uv_work_t* req);
        static void on_work(uv_work_t* req) {
            uvobj_t obj(from_ref, req);
            return obj.self()->on_work(obj, req); }

        // -- typedef void (*uv_after_work_cb)(uv_work_t* req);
        static void on_after_work(uv_work_t* req) {
            uvobj_t obj(from_ref, req);
            return obj.self()->on_after_work(obj, req); }

        // -- typedef void (*uv_getaddrinfo_cb)(uv_getaddrinfo_t* handle, int status, struct addrinfo* res);
        static void on_getaddrinfo(uv_getaddrinfo_t* handle, int status, struct addrinfo* res) {
            uvobj_t obj(from_ref, handle);
            return obj.self()->on_getaddrinfo(obj, status, res); }

        // -- typedef void (*uv_poll_cb)(uv_poll_t* handle, int status, int events);
        static void on_poll(uv_poll_t* handle, int status, int events) {
            uvobj_t obj(from_ref, handle);
            return obj.self()->on_poll(obj, status, events); }

        // -- typedef void (*uv_prepare_cb)(uv_prepare_t* handle, int status);
        static void on_prepare(uv_prepare_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            return obj.self()->on_prepare(obj, status); }

        // -- typedef void (*uv_check_cb)(uv_check_t* handle, int status);
        static void on_check(uv_check_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            return obj.self()->on_check(obj, status); }

        // -- typedef void (*uv_idle_cb)(uv_idle_t* handle, int status);
        static void on_idle(uv_idle_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            return obj.self()->on_idle(obj, status); }

        // -- typedef void (*uv_async_cb)(uv_async_t* handle, int status);
        static void on_async(uv_async_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            return obj.self()->on_async(obj, status); }

        // -- typedef void (*uv_timer_cb)(uv_timer_t* handle, int status);
        static void on_timer(uv_timer_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            return obj.self()->on_timer(obj, status); }

    };
}
