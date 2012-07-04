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

    /*~ Request Callbacks ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t>
    struct req_events_t {
        template <typename uv_t>
        inline static self_t* self(uv_t* ref) {
            return reinterpret_cast<self_t*>(ref->data); }

        // uv_fs_cb
        template < void (self_t::*fn)(uv_fs_t* req) >
        static void on_fs(uv_fs_t* req) {
            (self(req)->*fn)(req);
            uv_fs_req_cleanup(req);
            delete req; }

        // uv_connect_cb
        template < void (self_t::*fn)(uv_connect_t* req, int status) >
        static void on_connect(uv_connect_t* req, int status) {
            (self(req)->*fn)(req, status); delete req; }
        static void on_connect(uv_connect_t* req, int status) {
            on_connect<&self_t::on_connect>(req, status); }

        // uv_shutdown_cb
        template < void (self_t::*fn)(uv_shutdown_t* req, int status) >
        static void on_shutdown(uv_shutdown_t* req, int status) {
            (self(req)->*fn)(req, status); delete req; }
        static void on_shutdown(uv_shutdown_t* req, int status) {
            on_shutdown<&self_t::on_shutdown>(req, status); }

        // uv_udp_send_cb
        template < void (self_t::*fn)(uv_udp_send_t* req, int status) >
        static void on_udp_send(uv_udp_send_t* req, int status) {
            (self(req)->*fn)(req, status); delete req; }
        static void on_udp_send(uv_udp_send_t* req, int status) {
            on_udp_send<&self_t::on_udp_send>(req, status); }

        // uv_write_cb
        template < void (self_t::*fn)(uv_write_t* req, int status) >
        static void on_write(uv_write_t* req, int status) {
            (self(req)->*fn)(req, status); delete req; }
        static void on_write(uv_write_t* req, int status) {
            on_write<&self_t::on_write>(req, status); }

        // uv_work_cb
        template < void (self_t::*fn)(uv_work_t* req) >
        static void on_work(uv_work_t* req) {
            (self(req)->*fn)(req); }
        template < void (self_t::*fn)() >
        static void on_work(uv_work_t* req) {
            (self(req)->*fn)(); }
        static void on_work(uv_work_t* req) {
            on_work<&self_t::on_work>(req); }

        // uv_after_work_cb
        template < void (self_t::*fn)(uv_work_t* req) >
        static void on_after_work(uv_work_t* req) {
            (self(req)->*fn)(req); delete req; }
        template < void (self_t::*fn)() >
        static void on_after_work(uv_work_t* req) {
            (self(req)->*fn)(); delete req; }
        static void on_after_work(uv_work_t* req) {
            on_after_work<&self_t::on_after_work>(req); }

        // uv_getaddrinfo_cb
        template < void (self_t::*fn)(int status, struct addrinfo* res) >
        static void on_getaddrinfo(uv_getaddrinfo_t* req, int status, struct addrinfo* res) {
            (self(req)->*fn)(req, status, res); delete req; }
        static void on_getaddrinfo(uv_getaddrinfo_t* req, int status, struct addrinfo* res) {
            on_getaddrinfo<&self_t::on_getaddrinfo>(req, status, res); }

    };

    /*~ Object Callbacks ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t, typename uvobj_t_>
    struct events_t : req_events_t<self_t> {
        typedef uvobj_t_ uvobj_t;

        inline static self_t* self(uvobj_t& obj) { return obj.template data<self_t*>(); }
        template <typename uv_t>
        inline static self_t* self(uv_t* ref) {
            return reinterpret_cast<self_t*>(ref->data); }

        /* Handle-based Callbacks */

        // uv_close_cb
        template < void (self_t::*fn)(uvobj_t& obj) >
        static void on_close(uv_handle_t* handle) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj); }
        template < void (self_t::*fn)() >
        static void on_close(uv_handle_t* handle) {
            (self(handle)->*fn)(); }
        static void on_close(uv_handle_t* handle) {
            on_close<&self_t::on_close>(handle); }

        // uv_alloc_cb
        template < uv_buf_t (self_t::*fn)(uvobj_t& obj, size_t suggested_size) >
        static uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size) {
            uvobj_t obj(from_ref, handle);
            return (self(obj)->*fn)(obj, suggested_size); }
        template < uv_buf_t (self_t::*fn)(size_t suggested_size) >
        static uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size) {
            return (self(handle)->*fn)(suggested_size); }
        static uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size) {
            return on_alloc<&self_t::on_alloc>(handle, suggested_size); }

        // uv_udp_recv_cb
        typedef void (self_t::*udp_recv_obj_mfn)(uvobj_t& obj, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags);
        template < udp_recv_obj_mfn fn >
        static void on_udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, nread, buf, addr, flags); }

        typedef void (self_t::*udp_recv_mfn)(ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags);
        template < udp_recv_mfn fn >
        static void on_udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {
            (self(handle)->*fn)(nread, buf, addr, flags); }
        static void on_udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {
            on_udp_recv<&self_t::on_udp_recv>(handle, nread, buf, addr, flags); }

        // uv_connection_cb
        template < void (self_t::*fn)(uvobj_t& obj, int status) >
        static void on_connection(uv_stream_t* server, int status) {
            uvobj_t obj(from_ref, server);
            (self(obj)->*fn)(obj, status); }
        template < void (self_t::*fn)(int status) >
        static void on_connection(uv_stream_t* server, int status) {
            (self(server)->*fn)(status); }
        static void on_connection(uv_stream_t* server, int status) {
            on_connection<&self_t::on_connection>(server, status); }

        // uv_read_cb
        typedef void (self_t::*on_read_obj_mfn)(uvobj_t& obj, ssize_t nread, uv_buf_t buf);
        template < on_read_obj_mfn fn >
        static void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
            uvobj_t obj(from_ref, stream);
            (self(obj)->*fn)(obj, nread, buf); }
        typedef void (self_t::*on_read_mfn)(ssize_t nread, uv_buf_t buf);
        template < on_read_mfn fn >
        static void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
            (self(stream)->*fn)(nread, buf); }
        static void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
            on_read<&self_t::on_read>(stream, nread,  buf); }

        // uv_read2_cb
        typedef void (self_t::*on_read2_obj_mfn)(uvobj_t& obj, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
        template < on_read2_obj_mfn fn >
        static void on_read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {
            uvobj_t obj(from_ref, pipe);
            (self(obj)->*fn)(obj, nread, buf, pending); }
        typedef void (self_t::*on_read2_mfn)(ssize_t nread, uv_buf_t buf, uv_handle_type pending);
        template < on_read2_mfn fn >
        static void on_read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {
            (self(pipe)->*fn)(nread, buf, pending); }
        static void on_read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {
            on_read2<&self_t::on_read2>(pipe, nread, buf, pending); }

        // uv_exit_cb
        template < void (self_t::*fn)(uvobj_t& obj, int exit_status, int term_signal) >
        static void on_exit(uv_process_t* proc, int exit_status, int term_signal) {
            uvobj_t obj(from_ref, proc);
            (self(obj)->*fn)(obj, exit_status, term_signal); }
        template < void (self_t::*fn)(int exit_status, int term_signal) >
        static void on_exit(uv_process_t* proc, int exit_status, int term_signal) {
            (self(proc)->*fn)(exit_status, term_signal); }
        static void on_exit(uv_process_t* proc, int exit_status, int term_signal) {
            on_exit<&self_t::on_exit>(proc, exit_status, term_signal); }

        // uv_poll_cb
        template < void (self_t::*fn)(uvobj_t& obj, int status, int events) >
        static void on_poll(uv_poll_t* handle, int status, int events) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status, events); }
        template < void (self_t::*fn)(int status, int events) >
        static void on_poll(uv_poll_t* handle, int status, int events) {
            (self(handle)->*fn)(status, events); }
        static void on_poll(uv_poll_t* handle, int status, int events) {
            on_poll<&self_t::on_poll>(handle, status, events); }

        // uv_prepare_cb
        template < void (self_t::*fn)(uvobj_t& obj, int status) >
        static void on_prepare(uv_prepare_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (self_t::*fn)(int status) >
        static void on_prepare(uv_prepare_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_prepare(uv_prepare_t* handle, int status) {
            on_prepare<&self_t::on_prepare>(handle, status); }

        // uv_check_cb
        template < void (self_t::*fn)(uvobj_t& obj, int status) >
        static void on_check(uv_check_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (self_t::*fn)(int status) >
        static void on_check(uv_check_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_check(uv_check_t* handle, int status) {
            on_check<&self_t::on_check>(handle, status); }

        // uv_idle_cb
        template < void (self_t::*fn)(uvobj_t& obj, int status) >
        static void on_idle(uv_idle_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (self_t::*fn)(int status) >
        static void on_idle(uv_idle_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_idle(uv_idle_t* handle, int status) {
            on_idle<&self_t::on_idle>(handle, status); }

        // uv_async_cb
        template < void (self_t::*fn)(uvobj_t& obj, int status) >
        static void on_async(uv_async_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (self_t::*fn)(int status) >
        static void on_async(uv_async_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_async(uv_async_t* handle, int status) {
            on_async<&self_t::on_async>(handle, status); }

        // uv_timer_cb
        template < void (self_t::*fn)(uvobj_t& obj, int status) >
        static void on_timer(uv_timer_t* handle, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status); }
        template < void (self_t::*fn)(int status) >
        static void on_timer(uv_timer_t* handle, int status) {
            (self(handle)->*fn)(status); }
        static void on_timer(uv_timer_t* handle, int status) {
            on_timer<&self_t::on_timer>(handle, status); }

        // uv_fs_event_cb
        template < void (self_t::*fn)(uvobj_t& obj, const char* filename, int events, int status) >
        static void on_fs_event(uv_fs_event_t* handle, const char* filename, int events, int status) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, filename, events, status); }
        template < void (self_t::*fn)(const char* filename, int events, int status) >
        static void on_fs_event(uv_fs_event_t* handle, const char* filename, int events, int status) {
            (self(handle)->*fn)(filename, events, status); }
        static void on_fs_event(uv_fs_event_t* handle, const char* filename, int events, int status) {
            on_fs_event<&self_t::on_fs_event>(handle, filename, events, status); }

        // uv_fs_poll_cb
        template < void (self_t::*fn)(uvobj_t& obj, int status, const uv_statbuf_t* prev, const uv_statbuf_t* curr) >
        static void on_fs_poll(uv_fs_poll_t* handle, int status, const uv_statbuf_t* prev, const uv_statbuf_t* curr) {
            uvobj_t obj(from_ref, handle);
            (self(obj)->*fn)(obj, status, prev, curr); }
        template < void (self_t::*fn)(int status, const uv_statbuf_t* prev, const uv_statbuf_t* curr) >
        static void on_fs_poll(uv_fs_poll_t* handle, int status, const uv_statbuf_t* prev, const uv_statbuf_t* curr) {
            (self(handle)->*fn)(status, prev, curr); }
        static void on_fs_poll(uv_fs_poll_t* handle, int status, const uv_statbuf_t* prev, const uv_statbuf_t* curr) {
            on_fs_poll<&self_t::on_fs_poll>(handle, status, prev, curr); }
    };


}
