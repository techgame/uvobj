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

    template <typename T, typename uvobj_t>
    struct events_t {
        inline static T* self(uvobj_t& obj) { return obj.template data<T>(); }
        template <typename uv_t>
        inline static T* self(uv_t* ref) { return reinterpret_cast<T*>(ref->data); }

        /* Handle-based Callbacks */
        template < void (T::*fn)(uvobj_t& obj) >
        static void on_close(uv_handle_t* handle) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj); }
        template < void (T::*fn)() >
        static void on_close(uv_handle_t* handle) {
            (self(handle)->*fn)(); }
        static void on_close(uv_handle_t* handle) {
            on_close<&T::on_close>(handle); }

        template < uv_buf_t (T::*fn)(uvobj_t& obj, size_t suggested_size) >
        static uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size) {
            uvobj_t obj(from_ref, handle);
            return (self(obj)->*fn)(obj, suggested_size); }
        template < uv_buf_t (T::*fn)(size_t suggested_size) >
        static uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size) {
            return (self(handle)->*fn)(suggested_size); }
        static uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size) {
            return on_alloc<&T::on_alloc>(handle, suggested_size); }

        template < void (T::*fn)(uvobj_t& obj, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) >
        static void on_udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, nread, buf, addr, flags); }
        template < void (T::*fn)(ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) >
        static void on_udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {
            (self(handle)->*fn)(nread, buf, addr, flags); }
        static void on_udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {
            on_udp_recv<&T::on_udp_recv>(handle, nread, buf, addr, flags); }

        template < void (T::*fn)(uvobj_t& obj, int status) >
        static void on_connection(uv_stream_t* server, int status) {
            uvobj_t obj(from_ref, server);
            (self(obj)->*fn)(obj, status); }
        template < void (T::*fn)(int status) >
        static void on_connection(uv_stream_t* server, int status) {
            (self(server)->*fn)(status); }
        static void on_connection(uv_stream_t* server, int status) {
            on_connection<&T::on_connection>(server, status); }

        template < void (T::*fn)(uvobj_t& obj, ssize_t nread, uv_buf_t buf) >
        static void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
            uvobj_t obj(from_ref, stream);
            (self(obj)->*fn)(obj, nread, buf); }
        template < void (T::*fn)(ssize_t nread, uv_buf_t buf) >
        static void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
            (self(stream)->*fn)(nread, buf); }
        static void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
            on_read<&T::on_read>(stream, nread,  buf); }

        template < void (T::*fn)(uvobj_t& obj, ssize_t nread, uv_buf_t buf, uv_handle_type pending) >
        static void on_read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {
            uvobj_t obj(from_ref, pipe);
            (self(obj)->*fn)(obj, nread, buf, pending); }
        template < void (T::*fn)(ssize_t nread, uv_buf_t buf, uv_handle_type pending) >
        static void on_read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {
            (self(pipe)->*fn)(nread, buf, pending); }
        static void on_read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {
            on_read2<&T::on_read2>(pipe, nread, buf, pending); }

        template < void (T::*fn)(uvobj_t& obj, int exit_status, int term_signal) >
        static void on_exit(uv_process_t* proc, int exit_status, int term_signal) {
            uvobj_t obj(from_ref, proc);
            (self(obj)->*fn)(obj, exit_status, term_signal); }
        template < void (T::*fn)(int exit_status, int term_signal) >
        static void on_exit(uv_process_t* proc, int exit_status, int term_signal) {
            (self(proc)->*fn)(exit_status, term_signal); }
        static void on_exit(uv_process_t* proc, int exit_status, int term_signal) {
            on_exit<&T::on_exit>(proc, exit_status, term_signal); }

        template < void (T::*fn)(uvobj_t& obj, int status, struct addrinfo* res) >
        static void on_getaddrinfo(uv_getaddrinfo_t* handle, int status, struct addrinfo* res) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status, res); }
        template < void (T::*fn)(int status, struct addrinfo* res) >
        static void on_getaddrinfo(uv_getaddrinfo_t* handle, int status, struct addrinfo* res) {
            (self(handle)->*fn)(status, res); }
        static void on_getaddrinfo(uv_getaddrinfo_t* handle, int status, struct addrinfo* res) {
            on_getaddrinfo<&T::on_getaddrinfo>(handle, status, res); }

        template < void (T::*fn)(uvobj_t& obj, int status, int events) >
        static void on_poll(uv_poll_t* handle, int status, int events) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status, events); }
        template < void (T::*fn)(int status, int events) >
        static void on_poll(uv_poll_t* handle, int status, int events) {
            (self(handle)->*fn)(status, events); }
        static void on_poll(uv_poll_t* handle, int status, int events) {
            on_poll<&T::on_poll>(handle, status, events); }

        template < void (T::*fn)(uvobj_t& obj, int status) >
        static void on_prepare(uv_prepare_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (T::*fn)(int status) >
        static void on_prepare(uv_prepare_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_prepare(uv_prepare_t* handle, int status) {
            on_prepare<&T::on_prepare>(handle, status); }

        template < void (T::*fn)(uvobj_t& obj, int status) >
        static void on_check(uv_check_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (T::*fn)(int status) >
        static void on_check(uv_check_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_check(uv_check_t* handle, int status) {
            on_check<&T::on_check>(handle, status); }

        template < void (T::*fn)(uvobj_t& obj, int status) >
        static void on_idle(uv_idle_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (T::*fn)(int status) >
        static void on_idle(uv_idle_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_idle(uv_idle_t* handle, int status) {
            on_idle<&T::on_idle>(handle, status); }

        template < void (T::*fn)(uvobj_t& obj, int status) >
        static void on_async(uv_async_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (T::*fn)(int status) >
        static void on_async(uv_async_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_async(uv_async_t* handle, int status) {
            on_async<&T::on_async>(handle, status); }

        template < void (T::*fn)(uvobj_t& obj, int status) >
        static void on_timer(uv_timer_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (T::*fn)(int status) >
        static void on_timer(uv_timer_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_timer(uv_timer_t* handle, int status) {
            on_timer<&T::on_timer>(handle, status); }

        /* Request Callbacks */
        template < void (T::*fn)(uvobj_t& obj, uv_udp_send_t* req, int status) >
        static void on_udp_send(uv_udp_send_t* req, int status) {
            uvobj_t obj(from_ref, req->handle);
            (self(obj)->*fn)(obj, req, status); }
        template < void (T::*fn)(uv_udp_send_t* req, int status) >
        static void on_udp_send(uv_udp_send_t* req, int status) {
            (self(req->handle)->*fn)(req, status); }
        static void on_udp_send(uv_udp_send_t* req, int status) {
            on_udp_send<&T::on_udp_send>(req, status); }

        template < void (T::*fn)(uvobj_t& obj, uv_connect_t* req, int status) >
        static void on_connect(uv_connect_t* req, int status) {
            uvobj_t obj(from_ref, req->handle);
            (self(obj)->*fn)(obj, req, status); }
        template < void (T::*fn)(uv_connect_t* req, int status) >
        static void on_connect(uv_connect_t* req, int status) {
            (self(req->handle)->*fn)(req, status); }
        static void on_connect(uv_connect_t* req, int status) {
            on_connect<&T::on_connect>(req, status); }

        template < void (T::*fn)(uvobj_t& obj, uv_write_t* req, int status) >
        static void on_write(uv_write_t* req, int status) {
            uvobj_t obj(from_ref, req->handle);
            (self(obj)->*fn)(obj, req, status); }
        template < void (T::*fn)(uv_write_t* req, int status) >
        static void on_write(uv_write_t* req, int status) {
            (self(req->handle)->*fn)(req, status); }
        static void on_write(uv_write_t* req, int status) {
            on_write<&T::on_write>(req, status); }

        template < void (T::*fn)(uvobj_t& obj, uv_shutdown_t* req, int status) >
        static void on_shutdown(uv_shutdown_t* req, int status) {
            uvobj_t obj(from_ref, req->handle);
            (self(obj)->*fn)(obj, req, status); }
        template < void (T::*fn)(uv_shutdown_t* req, int status) >
        static void on_shutdown(uv_shutdown_t* req, int status) {
            (self(req->handle)->*fn)(req, status); }
        static void on_shutdown(uv_shutdown_t* req, int status) {
            on_shutdown<&T::on_shutdown>(req, status); }

        template < void (T::*fn)(uvobj_t& obj, uv_work_t* req) >
        static void on_work(uv_work_t* req) {
            uvobj_t obj(from_ref, req);
            (self(obj)->*fn)(obj, req); }
        template < void (T::*fn)(uv_work_t* req) >
        static void on_work(uv_work_t* req) {
            (self(req)->*fn)(req); }
        static void on_work(uv_work_t* req) {
            on_work<&T::on_work>(req); }

        template < void (T::*fn)(uvobj_t& obj, uv_work_t* req) >
        static void on_after_work(uv_work_t* req) {
            uvobj_t obj(from_ref, req);
            (self(obj)->*fn)(obj, req); }
        template < void (T::*fn)(uv_work_t* req) >
        static void on_after_work(uv_work_t* req) {
            (self(req)->*fn)(req); }
        static void on_after_work(uv_work_t* req) {
            on_after_work<&T::on_after_work>(req); }
    };
}
