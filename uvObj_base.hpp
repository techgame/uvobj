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

#if defined(WIN32)
#define snprintf _snprintf
#endif

#include <uv.h>

#include "./uvObj_events.hpp"

namespace uvObj {
    inline uv_loop_t* _as_loop(uv_loop_t* loop=NULL) {
        return loop ? loop : uv_default_loop(); }

    struct error : std::exception {
        error(uv_loop_t* loop, int res_p) : res(res_p) {
            if (loop == NULL) loop = uv_default_loop();
            err = uv_last_error(loop); }
        error(uv_loop_t* loop, uv_err_t err_p, int res_p=0)
         : res(res_p), err(err_p) {}
        const char* strerror() const { return uv_strerror(err); }
        const char* err_name() const { return uv_err_name(err); }
        virtual const char* what() const throw() { return err_name(); }

        int res;
        uv_err_t err;
    };

    inline bool uvResult(int res, uv_loop_t* loop=NULL) {
        if (res == UV_OK) return true;
        else throw error(loop, res); }
    inline bool uvResult(uv_err_t err, uv_loop_t* loop=NULL) {
        if (err.code == UV_OK) return true;
        else throw error(loop, err); }

    template <typename uv_type >
    struct Ref_t {
        typedef uv_type uv_t;

        Ref_t() : _ref_(new uv_t) { ::memset(_ref_, 0, sizeof(uv_t)); }
        explicit Ref_t(ref_mode_t m, uv_t* ref) : _ref_(ref) {}

        inline operator uv_t* () { return _ref_; }

        void destroy() { delete _ref_; _ref_ = NULL; }

        inline uv_loop_t* loop() const { return _ref_->loop; }
        inline bool uvRes(int res) const { return uvResult(res, loop()); }
        inline bool uvRes(int res, int ignore) const {
            if (res == ignore) return false;
            return uvResult(res, loop()); }        

        template <typename data_t>
        inline data_t data() { return reinterpret_cast<data_t>(_ref_ ? _ref_->data : NULL); }
        template <typename data_t>
        inline void setData(data_t data) { if (_ref_) _ref_->data = data; }
        template <typename data_t>
        inline void delData() {
            if (!_ref_) {
                delete data<data_t>();
                _ref_->data = NULL;
            }
        }

        uv_t* _ref_;
    };

    template <typename uv_t >
    struct Handle_t : Ref_t< uv_t > {
        typedef Ref_t< uv_t > Base_t;
        Handle_t() : Base_t() {}
        explicit Handle_t(ref_mode_t m, uv_t* ref) : Base_t(m, ref) {}

        inline uv_handle_t* asHandle() {
            return reinterpret_cast<uv_handle_t*>(Base_t::_ref_); }
        inline operator uv_handle_t* () { return asHandle(); }

        void ref() { uv_ref(asHandle()); }
        void unref() { uv_unref(asHandle()); }

        bool is_active() { return uv_is_active(asHandle()); }
        bool is_closing() { return uv_is_closing(asHandle()); }
        void close(uv_close_cb cb) { uv_close(asHandle(), cb); }
        template <typename T>
        void close(T* self) { Base_t::setData(self); close(T::evt::on_close); }

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

    struct Mutex : Ref_t< uv_mutex_t > {
        void init() { uvResult( uv_mutex_init(*this) ); }
        void destroy() { uv_mutex_destroy(*this); }
        void lock() { uv_mutex_lock(*this); }
        void trylock() { uvResult( uv_mutex_trylock(*this) ); }
        void unlock() { uv_mutex_unlock(*this); }
    };

    struct RWLock : Ref_t< uv_rwlock_t > {
        void init() { uvResult( uv_rwlock_init(*this) ); }
        void destroy() { uv_rwlock_destroy(*this); }

        void rdlock() { uv_rwlock_rdlock(*this); }
        void tryrdlock() { uvResult( uv_rwlock_tryrdlock(*this) ); }
        void rdunlock() { uv_rwlock_rdunlock(*this); }

        void wrlock() { uv_rwlock_wrlock(*this); }
        void trywrlock() { uvResult( uv_rwlock_trywrlock(*this) ); }
        void wrunlock() { uv_rwlock_wrunlock(*this); }
    };

    struct Semaphore : Ref_t< uv_sem_t > {
        void init(unsigned int value) { uvResult( uv_sem_init(*this, value) ); }
        void destroy() { uv_sem_destroy(*this); }
        void post() { uv_sem_post(*this); }
        void wait() { uv_sem_wait(*this); }
        void trywait() { uvResult( uv_sem_trywait(*this) ); }
    };
}
