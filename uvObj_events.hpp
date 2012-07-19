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
    template <typename uv_cb>
    struct BoundEvt {
        void* tgt; uv_cb cb;
        BoundEvt(void* tgt_, uv_cb cb_) : tgt(tgt_), cb(cb_) {}
        template <typename T> BoundEvt(T* self);
    };
    template <typename uv_cb>
    struct BoundAllocEvt {
        void* tgt; uv_alloc_cb alloc; uv_cb cb;
        BoundAllocEvt(void* tgt_, uv_alloc_cb alloc_, uv_cb cb_)
         : tgt(tgt_), alloc(alloc_), cb(cb_) {}
        template <typename T> BoundAllocEvt(T* self);
    };
    struct BoundWorkEvt {
        void* tgt; uv_work_cb work; uv_after_work_cb after_work;
        BoundWorkEvt(void* tgt_, uv_work_cb work_, uv_after_work_cb after_work_)
         : tgt(tgt_), work(work_), after_work(after_work_) {}
        template <typename T> BoundWorkEvt(T* self);
    };

    template <typename T>
    struct evt_t {
        template <typename uv_t>
        inline static T* evtTarget(uv_t* ref) {
            return reinterpret_cast<T*>(ref->data); }

        /*~ uv_alloc_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef uv_buf_t (T::*alloc_mfn)(uv_handle_t* handle, size_t suggested_size);
        template <alloc_mfn mfn>
        static uv_buf_t __alloc(uv_handle_t* handle, size_t suggested_size) {
            return (evtTarget(handle)->*mfn)(handle, suggested_size); }

        /*~ uv_udp_send_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*udp_send_mfn)(uv_udp_send_t* req, int status);
        template <udp_send_mfn mfn>
        static void __udp_send(uv_udp_send_t* req, int status) {
            if ((evtTarget(req)->*mfn)(req, status)) {
                ::free(req); }}

        static BoundEvt<uv_udp_send_cb> on_udp_send(T* tgt) {
            return on_udp_send<&T::on_udp_send>(tgt); }
        template <udp_send_mfn mfn>
        static BoundEvt<uv_udp_send_cb> on_udp_send(T* tgt) {
            return BoundEvt<uv_udp_send_cb>(tgt, &__udp_send<mfn>); }
        static BoundEvt<uv_udp_send_cb> on_send(T* tgt) {
            return on_send<&T::on_send>(tgt); }
        template <udp_send_mfn mfn>
        static BoundEvt<uv_udp_send_cb> on_send(T* tgt) {
            return on_send<mfn>(tgt); }

        /*~ uv_udp_recv_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*udp_recv_mfn)(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags);
        template <udp_recv_mfn mfn>
        static void __udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) {
            (evtTarget(handle)->*mfn)(handle, nread, buf, addr, flags); }

        static BoundAllocEvt<uv_udp_recv_cb> on_udp_recv(T* tgt) {
            return on_udp_recv<&T::on_alloc, &T::on_udp_recv>(tgt); }
        template <udp_recv_mfn mfnRecv>
        static BoundAllocEvt<uv_udp_recv_cb> on_udp_recv(T* tgt) {
            return on_udp_recv<&T::on_alloc, mfnRecv>(tgt); }
        template <alloc_mfn mfnAlloc, udp_recv_mfn mfnRecv>
        static BoundAllocEvt<uv_udp_recv_cb> on_udp_recv(T* tgt) {
            return BoundAllocEvt<uv_udp_recv_cb>(tgt, &__alloc<mfnAlloc>, &__udp_recv<mfnRecv>); }
        static BoundAllocEvt<uv_udp_recv_cb> on_recv(T* tgt) {
            return on_recv<&T::on_alloc, &T::on_recv>(tgt); }
        template <alloc_mfn mfnAlloc, udp_recv_mfn mfnRecv>
        static BoundAllocEvt<uv_udp_recv_cb> on_recv(T* tgt) {
            return on_recv<mfnAlloc, mfnRecv>(tgt); }

        /*~ uv_read_cb~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*read_mfn)(uv_stream_t* stream, ssize_t nread, uv_buf_t buf);
        template <read_mfn mfn>
        static void __read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
            (evtTarget(stream)->*mfn)(stream, nread, buf); }

        static BoundAllocEvt<uv_read_cb> on_read(T* tgt) {
            return on_read<&T::on_alloc, &T::on_read>(tgt); }
        template <read_mfn mfnRead>
        static BoundAllocEvt<uv_read_cb> on_read(T* tgt) {
            return on_read<&T::on_alloc, mfnRead>(tgt); }
        template <alloc_mfn mfnAlloc, read_mfn mfnRead>
        static BoundAllocEvt<uv_read_cb> on_read(T* tgt) {
            return BoundAllocEvt<uv_read_cb>(tgt, &__alloc<mfnAlloc>, &__read<mfnRead>); }

        /*~ uv_read2_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*read2_mfn)(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
        template <read2_mfn mfn>
        static void __read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) {
            (evtTarget(pipe)->*mfn)(pipe, nread, buf, pending); }

        static BoundAllocEvt<uv_read2_cb> on_read2(T* tgt) {
            return on_read2<&T::on_read2>(tgt); }
        template <read2_mfn mfnRead>
        static BoundAllocEvt<uv_read2_cb> on_read2(T* tgt) {
            return on_read2<&T::on_alloc, mfnRead>(tgt); }
        template <alloc_mfn mfnAlloc, read2_mfn mfnRead>
        static BoundAllocEvt<uv_read2_cb> on_read2(T* tgt) {
            return BoundAllocEvt<uv_read2_cb>(tgt, &__alloc<mfnAlloc>, &__read2<mfnRead>); }

        /*~ uv_write_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*write_mfn)(uv_write_t* req, int status);
        template <write_mfn mfn>
        static void __write(uv_write_t* req, int status) {
            if ((evtTarget(req)->*mfn)(req, status)) {
                ::free(req); }}

        static BoundEvt<uv_write_cb> on_write(T* tgt) {
            return on_write<&T::on_write>(tgt); }
        template <write_mfn mfn>
        static BoundEvt<uv_write_cb> on_write(T* tgt) {
            return BoundEvt<uv_write_cb>(tgt, &__write<mfn>); }

        /*~ uv_connect_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*connect_mfn)(uv_connect_t* req, int status);
        template <connect_mfn mfn>
        static void __connect(uv_connect_t* req, int status) {
            if ((evtTarget(req)->*mfn)(req, status)) {
                ::free(req); }}

        static BoundEvt<uv_connect_cb> on_connect(T* tgt) {
            return on_connect<&T::on_connect>(tgt); }
        template <connect_mfn mfn>
        static BoundEvt<uv_connect_cb> on_connect(T* tgt) {
            return BoundEvt<uv_connect_cb>(tgt, &__connect<mfn>); }

        /*~ uv_shutdown_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*shutdown_mfn)(uv_shutdown_t* req, int status);
        template <shutdown_mfn mfn>
        static void __shutdown(uv_shutdown_t* req, int status) {
            if ((evtTarget(req)->*mfn)(req, status)) {
                ::free(req); }}

        static BoundEvt<uv_shutdown_cb> on_shutdown(T* tgt) {
            return on_shutdown<&T::on_shutdown>(tgt); }
        template <shutdown_mfn mfn>
        static BoundEvt<uv_shutdown_cb> on_shutdown(T* tgt) {
            return BoundEvt<uv_shutdown_cb>(tgt, &__shutdown<mfn>); }

        /*~ uv_connection_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*connection_mfn)(uv_stream_t* stream, int status);
        template <connection_mfn mfn>
        static void __connection(uv_stream_t* stream, int status) {
            (evtTarget(stream)->*mfn)(stream, status); }

        static BoundEvt<uv_connection_cb> on_connection(T* tgt) {
            return on_connection<&T::on_connection>(tgt); }
        template <connection_mfn mfn>
        static BoundEvt<uv_connection_cb> on_connection(T* tgt) {
            return BoundEvt<uv_connection_cb>(tgt, &__connection<mfn>); }

        /*~ uv_close_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*close_mfn)(uv_handle_t* handle);
        template <close_mfn mfn>
        static void __close(uv_handle_t* handle) {
            if ((evtTarget(handle)->*mfn)(handle)) {
                ::free(handle); }}

        static BoundEvt<uv_close_cb> on_close(T* tgt) {
            return on_close<&T::on_close>(tgt); }
        template <close_mfn mfn>
        static BoundEvt<uv_close_cb> on_close(T* tgt) {
            return BoundEvt<uv_close_cb>(tgt, &__close<mfn>); }

        /*~ uv_poll_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*poll_mfn)(uv_poll_t* handle, int status, int events);
        template <poll_mfn mfn>
        static void __poll(uv_poll_t* handle, int status, int events) {
            (evtTarget(handle)->*mfn)(handle, status, events); }

        static BoundEvt<uv_poll_cb> on_poll(T* tgt) {
            return on_poll<&T::on_poll>(tgt); }
        template <poll_mfn mfn>
        static BoundEvt<uv_poll_cb> on_poll(T* tgt) {
            return BoundEvt<uv_poll_cb>(tgt, &__poll<mfn>); }

        /*~ uv_timer_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*timer_mfn)(uv_timer_t* handle, int status);
        template <timer_mfn mfn>
        static void __timer(uv_timer_t* handle, int status) {
            (evtTarget(handle)->*mfn)(handle, status); }

        static BoundEvt<uv_timer_cb> on_timer(T* tgt) {
            return on_timer<&T::on_timer>(tgt); }
        template <timer_mfn mfn>
        static BoundEvt<uv_timer_cb> on_timer(T* tgt) {
            return BoundEvt<uv_timer_cb>(tgt, &__timer<mfn>); }

        /*~ uv_async_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*async_mfn)(uv_async_t* handle, int status);
        template <async_mfn mfn>
        static void __async(uv_async_t* handle, int status) {
            (evtTarget(handle)->*mfn)(handle, status); }

        static BoundEvt<uv_async_cb> on_async(T* tgt) {
            return on_async<&T::on_async>(tgt); }
        template <async_mfn mfn>
        static BoundEvt<uv_async_cb> on_async(T* tgt) {
            return BoundEvt<uv_async_cb>(tgt, &__async<mfn>); }

        /*~ uv_prepare_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*prepare_mfn)(uv_prepare_t* handle, int status);
        template <prepare_mfn mfn>
        static void __prepare(uv_prepare_t* handle, int status) {
            (evtTarget(handle)->*mfn)(handle, status); }

        static BoundEvt<uv_prepare_cb> on_prepare(T* tgt) {
            return on_prepare<&T::on_prepare>(tgt); }
        template <prepare_mfn mfn>
        static BoundEvt<uv_prepare_cb> on_prepare(T* tgt) {
            return BoundEvt<uv_prepare_cb>(tgt, &__prepare<mfn>); }

        /*~ uv_check_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*check_mfn)(uv_check_t* handle, int status);
        template <check_mfn mfn>
        static void __check(uv_check_t* handle, int status) {
            (evtTarget(handle)->*mfn)(handle, status); }

        static BoundEvt<uv_check_cb> on_check(T* tgt) {
            return on_check<&T::on_check>(tgt); }
        template <check_mfn mfn>
        static BoundEvt<uv_check_cb> on_check(T* tgt) {
            return BoundEvt<uv_check_cb>(tgt, &__check<mfn>); }

        /*~ uv_idle_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*idle_mfn)(uv_idle_t* handle, int status);
        template <idle_mfn mfn>
        static void __idle(uv_idle_t* handle, int status) {
            (evtTarget(handle)->*mfn)(handle, status); }

        static BoundEvt<uv_idle_cb> on_idle(T* tgt) {
            return on_idle<&T::on_idle>(tgt); }
        template <idle_mfn mfn>
        static BoundEvt<uv_idle_cb> on_idle(T* tgt) {
            return BoundEvt<uv_idle_cb>(tgt, &__idle<mfn>); }

        /*~ uv_getaddrinfo_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*getaddrinfo_mfn)(uv_getaddrinfo_t* handle, int status, struct addrinfo* res);
        template <getaddrinfo_mfn mfn>
        static void __getaddrinfo(uv_getaddrinfo_t* handle, int status, struct addrinfo* res) {
            if ((evtTarget(handle)->*mfn)(handle, status, res)) {
                uv_freeaddrinfo(res); ::free(handle); }}

        static BoundEvt<uv_getaddrinfo_cb> on_getaddrinfo(T* tgt) {
            return on_getaddrinfo<&T::on_getaddrinfo>(tgt); }
        template <getaddrinfo_mfn mfn>
        static BoundEvt<uv_getaddrinfo_cb> on_getaddrinfo(T* tgt) {
            return BoundEvt<uv_getaddrinfo_cb>(tgt, &__getaddrinfo<mfn>); }

        /*~ uv_exit_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*exit_mfn)(uv_process_t* handle, int exit_status, int term_signal);
        template <exit_mfn mfn>
        static void __exit(uv_process_t* handle, int exit_status, int term_signal) {
            if ((evtTarget(handle)->*mfn)(handle, exit_status, term_signal)) {
                ::free(handle); }}

        static BoundEvt<uv_exit_cb> on_exit(T* tgt) {
            return on_exit<&T::on_exit>(tgt); }
        template <exit_mfn mfn>
        static BoundEvt<uv_exit_cb> on_exit(T* tgt) {
            return BoundEvt<uv_exit_cb>(tgt, &__exit<mfn>); }

        /*~ uv_work_cb / uv_after_work_cb ~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*work_mfn)(uv_work_t* req);
        template <work_mfn mfn>
        static void __work(uv_work_t* req) {
            (evtTarget(req)->*mfn)(req); }

        typedef bool (T::*after_work_mfn)(uv_work_t* req);
        template <after_work_mfn mfn>
        static void __after_work(uv_work_t* req) {
            if ((evtTarget(req)->*mfn)(req)) {
                ::free(req); }}

        static BoundWorkEvt on_work(T* tgt) {
            return on_work<&T::on_work, &T::on_after_work>(tgt); }
        template <work_mfn mfnWork, after_work_mfn mfnAfter>
        static BoundWorkEvt on_work(T* tgt) {
            return BoundWorkEvt(tgt, &__work<mfnWork>, &__after_work<mfnAfter>); }

        /*~ uv_fs_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*fs_mfn)(uv_fs_t* req);
        template <fs_mfn mfn>
        static void __fs(uv_fs_t* req) {
            if ((evtTarget(req)->*mfn)(req)) {
                uv_fs_req_cleanup(req); ::free(req); }}

        static BoundEvt<uv_fs_cb> on_fs(T* tgt) {
            return on_fs<&T::on_fs>(tgt); }
        template <fs_mfn mfn>
        static BoundEvt<uv_fs_cb> on_fs(T* tgt) {
            return BoundEvt<uv_fs_cb>(tgt, &__fs<mfn>); }

        /*~ uv_fs_event_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*fs_event_mfn)(uv_fs_event_t* handle, const char* filename, int events, int status);
        template <fs_event_mfn mfn>
        static void __fs_event(uv_fs_event_t* handle, const char* filename, int events, int status) {
            (evtTarget(handle)->*mfn)(handle, filename, events, status); }

        static BoundEvt<uv_fs_event_cb> on_fs_event(T* tgt) {
            return on_fs_event<&T::on_fs_event>(tgt); }
        template <fs_event_mfn mfn>
        static BoundEvt<uv_fs_event_cb> on_fs_event(T* tgt) {
            return BoundEvt<uv_fs_event_cb>(tgt, &__fs_event<mfn>); }

        /*~ uv_fs_poll_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*fs_poll_mfn)(uv_fs_poll_t* handle, int status, const uv_statbuf_t* prev, const uv_statbuf_t* curr);
        template <fs_poll_mfn mfn>
        static void __fs_poll(uv_fs_poll_t* handle, int status, const uv_statbuf_t* prev, const uv_statbuf_t* curr) {
            (evtTarget(handle)->*mfn)(handle, status, prev, curr); }

        static BoundEvt<uv_fs_poll_cb> on_fs_poll(T* tgt) {
            return on_fs_poll<&T::on_fs_poll>(tgt); }
        template <fs_poll_mfn mfn>
        static BoundEvt<uv_fs_poll_cb> on_fs_poll(T* tgt) {
            return BoundEvt<uv_fs_poll_cb>(tgt, &__fs_poll<mfn>); }

        /*~ uv_walk_cb ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        typedef bool (T::*walk_mfn)(uv_handle_t* handle);
        template <walk_mfn mfn>
        static void __walk(uv_handle_t* handle, void* arg) {
            (evtTarget(arg)->*mfn)(handle); }

        static BoundEvt<uv_walk_cb> on_walk(T* tgt) {
            return on_fs_poll<&T::on_walk>(tgt); }
        template <walk_mfn mfn>
        static BoundEvt<uv_walk_cb> on_walk(T* tgt) {
            return BoundEvt<uv_walk_cb>(tgt, &__fs_poll<mfn>); }


        /*~ Automatic destination type conversion ~~~~~~~~~~~~~ */
        static BoundEvt<uv_udp_send_cb> byType(T* tgt, uv_udp_send_cb cb) { return on_udp_send(tgt); }
        static BoundAllocEvt<uv_udp_recv_cb> byType(T* tgt, uv_udp_recv_cb cb) { return on_udp_recv(tgt); }
        static BoundAllocEvt<uv_read_cb> byType(T* tgt, uv_read_cb cb) { return on_read(tgt); }
        static BoundAllocEvt<uv_read2_cb> byType(T* tgt, uv_read2_cb cb) { return on_read2(tgt); }
        static BoundEvt<uv_write_cb> byType(T* tgt, uv_write_cb cb) { return on_write(tgt); }
        static BoundEvt<uv_connect_cb> byType(T* tgt, uv_connect_cb cb) { return on_connect(tgt); }
        static BoundEvt<uv_shutdown_cb> byType(T* tgt, uv_shutdown_cb cb) { return on_shutdown(tgt); }
        static BoundEvt<uv_connection_cb> byType(T* tgt, uv_connection_cb cb) { return on_connection(tgt); }
        static BoundEvt<uv_close_cb> byType(T* tgt, uv_close_cb cb) { return on_close(tgt); }
        static BoundEvt<uv_poll_cb> byType(T* tgt, uv_poll_cb cb) { return on_poll(tgt); }
        static BoundEvt<uv_timer_cb> byType(T* tgt, uv_timer_cb cb) { return on_timer(tgt); }
        static BoundEvt<uv_async_cb> byType(T* tgt, uv_async_cb cb) { return on_async(tgt); }
        static BoundEvt<uv_prepare_cb> byType(T* tgt, uv_prepare_cb cb) { return on_prepare(tgt); }
        static BoundEvt<uv_check_cb> byType(T* tgt, uv_check_cb cb) { return on_check(tgt); }
        static BoundEvt<uv_idle_cb> byType(T* tgt, uv_idle_cb cb) { return on_idle(tgt); }
        static BoundEvt<uv_getaddrinfo_cb> byType(T* tgt, uv_getaddrinfo_cb cb) { return on_getaddrinfo(tgt); }
        static BoundEvt<uv_exit_cb> byType(T* tgt, uv_exit_cb cb) { return on_exit(tgt); }
        static BoundWorkEvt byType(T* tgt, uv_work_cb cb) { return on_work(tgt); }
        static BoundEvt<uv_fs_cb> byType(T* tgt, uv_fs_cb cb) { return on_fs(tgt); }
        static BoundEvt<uv_fs_event_cb> byType(T* tgt, uv_fs_event_cb cb) { return on_fs_event(tgt); }
        static BoundEvt<uv_fs_poll_cb> byType(T* tgt, uv_fs_poll_cb cb) { return on_fs_poll(tgt); }
        static BoundEvt<uv_walk_cb> byType(T* tgt, uv_walk_cb cb) { return on_walk(tgt); }
    };
    template <typename uv_cb> template<typename T> BoundEvt<uv_cb>::BoundEvt(T* self) { *this = evt_t<T>::byType(self, cb); }
    template <typename uv_cb> template <typename T> BoundAllocEvt<uv_cb>::BoundAllocEvt(T* self) { *this = evt_t<T>::byType(self, cb); }
    template <typename T> BoundWorkEvt::BoundWorkEvt(T* self) { *this = evt_t<T>::on_work(self); }

    struct BlackHoleSink {
        typedef uvObj::evt_t< BlackHoleSink > evt;
        // Note: Some methods are unsuited for NOOP implementation

        bool on_close(uv_handle_t* handle) { return true; }

        // uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size) { return true; }
        // bool on_udp_recv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) { return true; }
        bool on_udp_send(uv_udp_send_t* req, int status) { return true; }

        // uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size) { return true; }
        // bool on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) { return true; }
        // bool on_read2(uv_pipe_t* pipe, ssize_t nread, uv_buf_t buf, uv_handle_type pending) { return true; }
        bool on_write(uv_write_t* req, int status) { return true; }

        bool on_connection(uv_stream_t* stream, int status) { return true; }
        bool on_shutdown(uv_shutdown_t* req, int status) { return true; }
        bool on_connect(uv_connect_t* req, int status) { return true; }

        bool on_poll(uv_poll_t* handle, int status, int events) { return true; }
        bool on_timer(uv_timer_t* handle, int status) { return true; }
        bool on_async(uv_async_t* handle, int status) { return true; }
        bool on_prepare(uv_prepare_t* handle, int status) { return true; }
        bool on_check(uv_check_t* handle, int status) { return true; }
        bool on_idle(uv_idle_t* handle, int status) { return true; }
        bool on_getaddrinfo(uv_getaddrinfo_t* handle, int status, struct addrinfo* res) { return true; }
        bool on_exit(uv_process_t* handle, int exit_status, int term_signal) { return true; }
        /* Unsuited for NOOP implementation
        bool on_work(uv_work_t* req) { return true; }
        bool on_after_work(uv_work_t* req) { return true; }
        */
        bool on_fs(uv_fs_t* req) { return true; }
        bool on_fs_event(uv_fs_event_t* handle, const char* filename, int events, int status) { return true; }
        bool on_fs_poll(uv_fs_poll_t* handle, int status, const uv_statbuf_t* prev, const uv_statbuf_t* curr) { return true; }

        //bool (T::*walk_mfn)(uv_handle_t* handle) { return true; }
    };
    inline static BlackHoleSink* blackhole() { return NULL; }
}
