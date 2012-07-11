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

        void init(int fd) { init(NULL, fd); }
        void init(uv_loop_t* loop, int fd) {
            Base_t::_uvRes( uv_poll_init(_as_loop(loop), *this, fd) ); }
        void init_socket(uv_os_sock_t socket) {
            init_socket(NULL, socket); }
        void init_socket(uv_loop_t* loop, uv_os_sock_t socket) {
            Base_t::_uvRes( uv_poll_init_socket(_as_loop(loop), *this, socket) ); }
        void start(int events, uv_poll_cb cb) {
            Base_t::_uvRes( uv_poll_start(*this, events, cb) ); }
        template <typename T>
        void start(T* self, int events) {
            Base_t::setData(self); start(events, T::evt::on_poll); }
        void stop() {
            Base_t::_uvRes( uv_poll_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Process : Handle_t< uv_process_t > {
        typedef Handle_t< uv_process_t > Base_t;
        void spawn(const uv_process_options_t& options) {
            spawn(NULL, options); }
        void spawn(uv_loop_t* loop, const uv_process_options_t& options) {
            Base_t::_uvRes( uv_spawn(_as_loop(loop), *this, options) ); }
        void kill(int signum) {
            Base_t::_uvRes( uv_process_kill(*this, signum) ); }
        static void kill(int pid, int signum) {
            uvResult(uv_kill(pid, signum)); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Prepare : Handle_t< uv_prepare_t > {
        typedef Handle_t< uv_prepare_t > Base_t;
        Prepare(uv_loop_t* loop) : Base_t() { init(loop); }
        Prepare() : Base_t() { init(); }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_prepare_init(_as_loop(loop), *this) ); }
        void start(uv_prepare_cb cb) {
            Base_t::_uvRes( uv_prepare_start(*this, cb) ); }
        template <typename T>
        void start(T* self) {
            Base_t::setData(self); start(T::evt::on_prepare); }
        void stop() {
            Base_t::_uvRes( uv_prepare_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Check : Handle_t< uv_check_t > {
        typedef Handle_t< uv_check_t > Base_t;
        Check(uv_loop_t* loop) : Base_t() { init(loop); }
        Check() : Base_t() { init(); }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_check_init(_as_loop(loop), *this) ); }
        void start(uv_check_cb cb) {
            Base_t::_uvRes( uv_check_start(*this, cb) ); }
        template <typename T>
        void start(T* self) {
            Base_t::setData(self); start(T::evt::on_check); }
        void stop() {
            Base_t::_uvRes( uv_check_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Idle : Handle_t< uv_idle_t > {
        typedef Handle_t< uv_idle_t > Base_t;
        Idle(uv_loop_t* loop) : Base_t() { init(loop); }
        Idle() : Base_t() { init(); }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_idle_init(_as_loop(loop), *this) ); }
        void start(uv_idle_cb cb) {
            Base_t::_uvRes( uv_idle_start(*this, cb) ); }
        template <typename T>
        void start(T* self) {
            Base_t::setData(self); start(T::evt::on_idle); }
        void stop() {
            Base_t::_uvRes( uv_idle_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Async : Handle_t< uv_async_t > {
        typedef Handle_t< uv_async_t > Base_t;
        Async(uv_loop_t* loop, uv_async_cb cb) : Base_t() { init(loop, cb); }
        Async(uv_async_cb cb) : Base_t() { init(cb); }
        template <typename T>
        Async(T* self, uv_loop_t* loop=NULL) : Base_t() {
            Base_t::setData(self); init(loop, T::evt::on_async); }

        void init(uv_async_cb cb) { init(NULL, cb); }
        void init(uv_loop_t* loop, uv_async_cb cb) {
            Base_t::_uvRes( uv_async_init(_as_loop(loop), *this, cb) ); }
        template <typename T>
        void init(T* self, uv_loop_t* loop=NULL) {
            Base_t::setData(self); init(loop, T::evt::on_async); }
        void send() {
            Base_t::_uvRes( uv_async_send(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Timer : Handle_t< uv_timer_t > {
        typedef Handle_t< uv_timer_t > Base_t;
        Timer(uv_loop_t* loop) : Base_t() { init(loop); }
        Timer() : Base_t() { init(); }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_timer_init(_as_loop(loop), *this) ); }
        void start(uv_timer_cb cb, int64_t timeout, int64_t repeat=0) {
            Base_t::_uvRes( uv_timer_start(*this, cb, timeout, repeat) ); }
        template <typename T>
        void start(T* self, int64_t timeout, int64_t repeat=0) {
            Base_t::setData(self); start(T::evt::on_timer, timeout, repeat); }
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
