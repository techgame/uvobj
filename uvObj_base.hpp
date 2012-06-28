/* -*- coding: utf-8 -*- vim: set ts=4 sw=4 expandtab */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##
##~ Copyright (C) 2002-2012  TechGame Networks, LLC.              ##
##~                                                               ##
##~ This library is free software; you can redistribute it        ##
##~ and/or modify it under the terms of the MIT style License as  ##
##~ found in the LICENSE file included with this distribution.    ##
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma once
#include <string>
#include <vector>

#include <uv.h>

#include "./uvObj_callbacks.hpp"

namespace uvObj {
    inline uv_loop_t* _as_loop(uv_loop_t* loop=NULL) {
        return loop ? loop : uv_default_loop(); }

    struct self_void_t {
        typedef void api_t;
    };

    template <typename uv_type, typename self_type=self_void_t >
    struct Ref_t {
        typedef uv_type uv_t;
        typedef self_type self_t;
        typedef events_t<typename self_t::api_t> Evt;

        Ref_t() : _ref_(new uv_t) { ::memset(_ref_, 0, sizeof(uv_t)); }
        explicit Ref_t(ref_mode_t m, uv_t* ref) : _ref_(ref) {}

        inline operator uv_t* () { return _ref_; }

        inline operator self_t* () { return self(); }
        inline self_t* self() { return reinterpret_cast<self_t*>(_ref_->data); }
        inline void setSelf(self_t* self) { _ref_->data = self; }
        inline void delSelf() { delete self(); _ref_->data = NULL; }

        inline uv_loop_t* loop() { return _ref_->loop; }
        inline operator uv_loop_t* () { return _ref_->loop; }

        void destroy() { delete _ref_; _ref_ = NULL; }

        uv_err_t last_error() { return uv_last_error(*this); }
        const char* last_strerror() { return uv_strerror(last_error()); }
        const char* strerror(uv_err_t err) { return uv_strerror(err); }
        const char* last_err_name() { return uv_err_name(last_error()); }
        const char* err_name(uv_err_t err) { return uv_err_name(err); }

        uv_t* _ref_;
    };

    template <typename uv_t, typename self_t >
    struct Handle_t : Ref_t< uv_t, self_t > {
        typedef Ref_t< uv_t, self_t > Base_t;
        Handle_t() : Base_t() {}
        explicit Handle_t(ref_mode_t m, uv_t* ref) : Base_t(m, ref) {}

        inline uv_handle_t* asHandle() {
            return reinterpret_cast<uv_handle_t*>(Base_t::_ref_); }
        inline operator uv_handle_t* () { return asHandle(); }

        void ref() { uv_ref(asHandle()); }
        void unref() { uv_unref(asHandle()); }

        int is_active() { return uv_is_active(asHandle()); }
        int is_closing() { return uv_is_closing(asHandle()); }
        int close(uv_close_cb cb) {
            return uv_close(asHandle(), cb); }
        int close() { return close(Base_t::Evt::on_close); }

        static uv_buf_t buf_create(unsigned int len) {
            return buf_init((char*)::malloc(len), len); }
        inline static uv_buf_t buf_init(char* base, unsigned int len) {
            return uv_buf_init(base, len); }
        static void buf_free(uv_buf_t& buf) {
            if (!buf.base || !buf.len) return;
            ::free(buf.base);
            buf.base = NULL;
            buf.len = 0; }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct Mutex : Ref_t< uv_mutex_t, self_t > {
        int init() { return uv_mutex_init(*this); }
        void destroy() { return uv_mutex_destroy(*this); }
        void lock() { return uv_mutex_lock(*this); }
        int trylock() { return uv_mutex_trylock(*this); }
        void unlock() { return uv_mutex_unlock(*this); }
    };

    template <typename self_t=self_void_t>
    struct RWLock : Ref_t< uv_rwlock_t, self_t > {
        int init() { return uv_rwlock_init(*this); }
        void destroy() { return uv_rwlock_destroy(*this); }

        void rdlock() { return uv_rwlock_rdlock(*this); }
        int tryrdlock() { return uv_rwlock_tryrdlock(*this); }
        void rdunlock() { return uv_rwlock_rdunlock(*this); }

        void wrlock() { return uv_rwlock_wrlock(*this); }
        int trywrlock() { return uv_rwlock_trywrlock(*this); }
        void wrunlock() { return uv_rwlock_wrunlock(*this); }
    };
}
