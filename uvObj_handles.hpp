/* -*- coding: utf-8 -*- vim: set ts=4 sw=4 expandtab */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##
##~ Copyright (C) 2002-2012  TechGame Networks, LLC.              ##
##~                                                               ##
##~ This library is free software; you can redistribute it        ##
##~ and/or modify it under the terms of the MIT style License as  ##
##~ found in the LICENSE file included with this distribution.    ##
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma once
#include "./uvobj_base.hpp"

namespace uvObj {
    template <typename self_t=self_void_t>
    struct Work : Ref_t< uv_work_t, self_t > {
        typedef Ref_t< uv_work_t, self_t > Base_t;
        int queue(uv_work_cb work_cb, uv_after_work_cb after_cb) {
            return queue(NULL, work_cb, after_cb); }
        int queue(uv_loop_t* loop, uv_work_cb work_cb, uv_after_work_cb after_cb) {
            return uv_queue_work(loop, *this, work_cb, after_cb); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct GetAddrInfo : Ref_t< uv_getaddrinfo_t, self_t > {
        typedef Ref_t< uv_getaddrinfo_t, self_t > Base_t;
        int getaddrinfo(uv_getaddrinfo_cb getaddrinfo_cb,
                const char* node, const char* service, const struct addrinfo* hints) {
            return getaddrinfo(NULL, getaddrinfo_cb, node, service, hints); }
        int getaddrinfo(uv_loop_t* loop, uv_getaddrinfo_cb getaddrinfo_cb,
                const char* node, const char* service, const struct addrinfo* hints) {
            return uv_getaddrinfo(loop, *this, getaddrinfo_cb, node, service, hints); }

        static void freeaddrinfo(struct addrinfo* ai) { uv_freeaddrinfo(ai); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct Poll : Handle_t< uv_poll_t, self_t > {
        typedef Handle_t< uv_poll_t, self_t > Base_t;
        Poll(uv_loop_t* loop, int fd) : Base_t() { init(loop, fd); }
        Poll(int fd) : Base_t() { init(fd); }

        int init(int fd) { return init(NULL, fd); }
        int init(uv_loop_t* loop, int fd) {
            return uv_poll_init(_as_loop(loop), *this, fd); }
        int init_socket(uv_os_sock_t socket) {
            return init_socket(NULL, socket); }
        int init_socket(uv_loop_t* loop, uv_os_sock_t socket) {
            return uv_poll_init_socket(_as_loop(loop), *this, socket); }
        int start(int events, uv_poll_cb cb) {
            return uv_poll_start(*this, events, cb); }
        int stop() { return uv_poll_stop(*this); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct Prepare : Handle_t< uv_prepare_t, self_t > {
        typedef Handle_t< uv_prepare_t, self_t > Base_t;
        Prepare(uv_loop_t* loop) : Base_t() { init(loop); }
        Prepare() : Base_t() { init(); }

        int init() { return init(NULL); }
        int init(uv_loop_t* loop) {
            return uv_prepare_init(_as_loop(loop), *this); }
        int start(uv_prepare_cb cb) {
            return uv_prepare_start(*this, cb); }
        int stop() { return uv_prepare_stop(*this); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct Check : Handle_t< uv_check_t, self_t > {
        typedef Handle_t< uv_check_t, self_t > Base_t;
        Check(uv_loop_t* loop) : Base_t() { init(loop); }
        Check() : Base_t() { init(); }

        int init() { return init(NULL); }
        int init(uv_loop_t* loop) {
            return uv_check_init(_as_loop(loop), *this); }
        int start(uv_check_cb cb) {
            return uv_check_start(*this, cb); }
        int stop() { return uv_check_stop(*this); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct Idle : Handle_t< uv_idle_t, self_t > {
        typedef Handle_t< uv_idle_t, self_t > Base_t;
        Idle(uv_loop_t* loop) : Base_t() { init(loop); }
        Idle() : Base_t() { init(); }

        int init() { return init(NULL); }
        int init(uv_loop_t* loop) {
            return uv_idle_init(_as_loop(loop), *this); }
        int start(uv_idle_cb cb) {
            return uv_idle_start(*this, cb); }
        int stop() { return uv_idle_stop(*this); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct Async : Handle_t< uv_async_t, self_t > {
        typedef Handle_t< uv_async_t, self_t > Base_t;
        Async(uv_loop_t* loop, uv_async_cb cb) : Base_t() { init(loop, cb); }
        Async(uv_async_cb cb) : Base_t() { init(cb); }

        int init(uv_async_cb cb) { return init(NULL, cb); }
        int init(uv_loop_t* loop, uv_async_cb cb) {
            return uv_async_init(_as_loop(loop), *this, cb); }
        int send() { return uv_async_send(*this); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct Timer : Handle_t< uv_timer_t, self_t > {
        typedef Handle_t< uv_timer_t, self_t > Base_t;
        Timer(uv_loop_t* loop) : Base_t() { init(loop); }
        Timer() : Base_t() { init(); }

        int init() { return init(NULL); }
        int init(uv_loop_t* loop) {
            return uv_timer_init(_as_loop(loop), *this); }
        int start(uv_timer_cb cb, int64_t timeout, int64_t repeat) {
            return uv_timer_start(*this, cb, timeout, repeat); }
        int stop() { return uv_timer_stop(*this); }
        int again() { return uv_timer_again(*this); }
        int64_t get_repeat() {
            return uv_timer_get_repeat(*this); }
        void set_repeat(int64_t repeat) {
            uv_timer_set_repeat(*this, repeat); }
    };
}
