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
    struct Poll : Handle_t< uv_poll_t > {
        typedef Handle_t< uv_poll_t > Base_t;
        Poll(uv_loop_t* loop, int fd) : Base_t() { init(loop, fd); }
        Poll(int fd) : Base_t() { init(fd); }
        explicit Poll(uv_poll_t* ref) : Base_t(ref) { }

        void init(int fd) { init(NULL, fd); }
        void init(uv_loop_t* loop, int fd) {
            Base_t::_uvRes( uv_poll_init(_as_loop(loop), *this, fd) ); }
        void init_socket(uv_os_sock_t socket) {
            init_socket(NULL, socket); }
        void init_socket(uv_loop_t* loop, uv_os_sock_t socket) {
            Base_t::_uvRes( uv_poll_init_socket(_as_loop(loop), *this, socket) ); }
        void start(int events, uv_poll_cb cb) {
            Base_t::_uvRes( uv_poll_start(*this, events, cb) ); }
        void start(const BoundEvt<uv_poll_cb>& evt, int events) {
            Base_t::setData(evt.tgt); start(events, evt.cb); }
        void stop() {
            Base_t::_uvRes( uv_poll_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Process : Handle_t< uv_process_t > {
        typedef Handle_t< uv_process_t > Base_t;
        Process() : Base_t() { }
        explicit Process(uv_process_t* ref) : Base_t(ref) { }

        void spawn(const uv_process_options_t& options) {
            spawn(NULL, options); }
        void spawn(uv_loop_t* loop, const uv_process_options_t& options) {
            Base_t::_uvRes( uv_spawn(_as_loop(loop), *this, options) ); }
        void kill(int signum=SIGTERM) {
            Base_t::_uvRes( uv_process_kill(*this, signum) ); }
        static void kill(int pid, int signum) {
            uvResult(uv_kill(pid, signum)); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Prepare : Handle_t< uv_prepare_t > {
        typedef Handle_t< uv_prepare_t > Base_t;
        Prepare(uv_loop_t* loop) : Base_t() { init(loop); }
        Prepare() : Base_t() { init(); }
        explicit Prepare(uv_prepare_t* ref) : Base_t(ref) { }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_prepare_init(_as_loop(loop), *this) ); }
        void start(uv_prepare_cb cb) {
            Base_t::_uvRes( uv_prepare_start(*this, cb) ); }
        void start(const BoundEvt<uv_prepare_cb>& evt) {
            Base_t::setData(evt.tgt); start(evt.cb); }
        void stop() {
            Base_t::_uvRes( uv_prepare_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Check : Handle_t< uv_check_t > {
        typedef Handle_t< uv_check_t > Base_t;
        Check(uv_loop_t* loop) : Base_t() { init(loop); }
        Check() : Base_t() { init(); }
        explicit Check(uv_check_t* ref) : Base_t(ref) { }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_check_init(_as_loop(loop), *this) ); }
        void start(uv_check_cb cb) {
            Base_t::_uvRes( uv_check_start(*this, cb) ); }
        void start(const BoundEvt<uv_check_cb>& evt) {
            Base_t::setData(evt.tgt); start(evt.cb); }
        void stop() {
            Base_t::_uvRes( uv_check_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Idle : Handle_t< uv_idle_t > {
        typedef Handle_t< uv_idle_t > Base_t;
        Idle(uv_loop_t* loop) : Base_t() { init(loop); }
        Idle() : Base_t() { init(); }
        explicit Idle(uv_idle_t* ref) : Base_t(ref) { }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_idle_init(_as_loop(loop), *this) ); }
        void start(uv_idle_cb cb) {
            Base_t::_uvRes( uv_idle_start(*this, cb) ); }
        void start(const BoundEvt<uv_idle_cb>& evt) {
            Base_t::setData(evt.tgt); start(evt.cb); }
        void stop() {
            Base_t::_uvRes( uv_idle_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Async : Handle_t< uv_async_t > {
        typedef Handle_t< uv_async_t > Base_t;
        Async(uv_loop_t* loop, uv_async_cb cb) : Base_t() { init(loop, cb); }
        Async(uv_async_cb cb) : Base_t() { init(cb); }
        Async(const BoundEvt<uv_async_cb>& evt, uv_loop_t* loop=NULL) : Base_t() {
            Base_t::setData(evt.tgt); init(loop, evt.cb); }
        explicit Async(uv_async_t* ref) : Base_t(ref) { }

        void init(uv_async_cb cb) { init(NULL, cb); }
        void init(uv_loop_t* loop, uv_async_cb cb) {
            Base_t::_uvRes( uv_async_init(_as_loop(loop), *this, cb) ); }
        void init(const BoundEvt<uv_async_cb>& evt, uv_loop_t* loop=NULL) {
            Base_t::setData(evt.tgt); init(loop, evt.cb); }
        void send() {
            Base_t::_uvRes( uv_async_send(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Timer : Handle_t< uv_timer_t > {
        typedef Handle_t< uv_timer_t > Base_t;
        Timer(uv_loop_t* loop) : Base_t() { init(loop); }
        Timer() : Base_t() { init(); }
        explicit Timer(uv_timer_t* ref) : Base_t(ref) { }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_timer_init(_as_loop(loop), *this) ); }
        void start(uv_timer_cb cb, int64_t timeout, int64_t repeat=0) {
            Base_t::_uvRes( uv_timer_start(*this, cb, timeout, repeat) ); }
        void start(const BoundEvt<uv_timer_cb>& evt, int64_t timeout, int64_t repeat=0) {
            Base_t::setData(evt.tgt); start(evt.cb, timeout, repeat); }
        void stop() {
            Base_t::_uvRes( uv_timer_stop(*this) ); }
        void again() {
            Base_t::_uvRes( uv_timer_again(*this) ); }
        int64_t get_repeat() {
            return uv_timer_get_repeat(*this); }
        void set_repeat(int64_t repeat) {
            uv_timer_set_repeat(*this, repeat); }
    };
}
