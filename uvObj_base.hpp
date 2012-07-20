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
    
    template <typename uv_t>
    inline static uv_t* __uv_create() {
        uv_t* uv = (uv_t*) ::malloc(sizeof(uv_t));
        ::memset(uv, 0, sizeof(uv_t));
        return uv; }
    
    inline static void __uv_destroy(void* uv) {
        ::free(uv); }
    inline static void __uv_destroy_handle(uv_handle_t* uv) {
        __uv_destroy(uv); }
    template <typename uv_t, typename data_t>
    static void __uv_delData(uv_t* uv) {
        if (!uv) return;
        delete reinterpret_cast<data_t>(uv->data);
        uv->data = NULL; }    
    template <typename data_t, typename uv_t>
    inline static void __uv_destroyEx(uv_t* uv) {
        if (!uv) return;
        __uv_delData<data_t>(uv);
        __uv_destroy(uv); }

    inline uv_buf_t buf_create(unsigned int len) {
        char* buf = (char*)::malloc(len);
        buf[len-1] = 0;
        return uv_buf_init(buf, len); }
    inline uv_buf_t buf_init(char* base, unsigned int len) {
        return uv_buf_init(base, len); }
    inline void buf_free(uv_buf_t& buf) {
        if (!buf.base || !buf.len) return;
        ::free(buf.base);
        buf.base = NULL;
        buf.len = 0; }

    struct error : std::exception {
        error(uv_loop_t* loop, int res_p) : res(res_p) {
            if (res == -1) {
                err.code = UV_UNKNOWN; err.sys_errno_ = 0;
            } else {
                if (loop == NULL) loop = uv_default_loop();
                err = uv_last_error(loop);
            }}
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
    template <typename T>
    inline bool uvResult(int res, T* handle) {
        return uvResult(res, handle->loop); }
    inline bool uvResult(uv_err_t err, uv_loop_t* loop=NULL) {
        if (err.code == UV_OK) return true;
        else throw error(loop, err); }

    template <typename uv_type >
    struct Ref_t {
        typedef uv_type uv_t;

        Ref_t() : uv(__uv_create<uv_t>()) {}
        explicit Ref_t(uv_t* ref) : uv(ref) {}
        operator uv_t* () { return uv; }
        void destroy() { __uv_destroy(uv); uv = NULL; }

        uv_loop_t* loop() const { return uv->loop; }
        uv_err_t last_error() { return uv_last_error(loop()); }
        bool _uvRes(int res) const { return uvResult(res, loop()); }
        bool _uvRes(int res, int ignore) const {
            if (res == ignore) return false;
            return uvResult(res, loop()); }

        inline void* data() { return uv ? uv->data : NULL; }
        template <typename data_t>
        inline data_t data() { return reinterpret_cast<data_t>(data()); }
        void setData(void* data) {
            if (uv) uv->data = data; }
        template <typename Fn>
        void setCallback(void* data, Fn cb) {
            if (!uv) return;
            uv->data = data;
            uv->cb = cb; }
        template <typename BoundEvt>
        void setCallback(const BoundEvt& evt) {
            if (!uv) return;
            uv->data = evt.tgt;
            uv->cb = evt.cb; }
        template <typename data_t>
        void delData() { __uv_delData<data_t>(uv); }

        uv_t* uv;
    };

    template <typename uv_t >
    struct Handle_t : Ref_t< uv_t > {
        typedef Ref_t< uv_t > Base_t;
        Handle_t() : Base_t() {}
        explicit Handle_t(uv_t* ref) : Base_t(ref) {}

        inline uv_handle_t* asHandle() {
            return reinterpret_cast<uv_handle_t*>(Base_t::uv); }
        inline operator uv_handle_t* () { return asHandle(); }

        void destroy() {
            if (Base_t::uv) return;
            assert(is_closed());
            Base_t::destroy(); }
        void ref() { uv_ref(asHandle()); }
        void unref() { uv_unref(asHandle()); }

        bool is_active() { return Base_t::uv ? uv_is_active(asHandle()) : false; }
        bool is_closing() { return Base_t::uv ? uv_is_closing(asHandle()) : true; }
        bool is_closed() { return is_closing() & 0x2; }
        void close(uv_close_cb cb=NULL) {
            uv_close(asHandle(), cb ? cb : &__uv_destroy_handle); }
        template <typename data_t>
        void closeEx(data_t* tgt=NULL) {
            uv_close(asHandle(), &Base_t::template __uv_destroyEx<data_t>); }
        void close(const BoundEvt<uv_close_cb>& evt) {
            Base_t::setData(evt.tgt); close(evt.cb); }

        static void __destroy_on_close(uv_handle_t* uv) {
            Base_t::__destroy((uv_t*)uv); }
    };
    typedef Handle_t<uv_handle_t> Handle;

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename uv_type >
    struct BasicRef_t {
        typedef uv_type uv_t;
        BasicRef_t() : uv(__uv_create<uv_t>()) {}
        explicit BasicRef_t(uv_t* ref) : uv(ref) {}
        operator uv_t* () { return uv; }
        void destroy() { __uv_destroy(uv); uv = NULL; }
        uv_t* uv;
    };
        
    struct Mutex : BasicRef_t< uv_mutex_t > {
        typedef BasicRef_t< uv_mutex_t > Base_t;
        Mutex() : Base_t() { init(); }
        void init() { uvResult( uv_mutex_init(*this) ); }
        void destroy() { uv_mutex_destroy(*this); Base_t::destroy(); }
        void lock() { uv_mutex_lock(*this); }
        void trylock() { uvResult( uv_mutex_trylock(*this) ); }
        void unlock() { uv_mutex_unlock(*this); }
    };

    struct RWLock : BasicRef_t< uv_rwlock_t > {
        typedef BasicRef_t< uv_rwlock_t > Base_t;
        RWLock() : Base_t() { init(); }
        void init() { uvResult( uv_rwlock_init(*this) ); }
        void destroy() { uv_rwlock_destroy(*this); Base_t::destroy(); }

        void rdlock() { uv_rwlock_rdlock(*this); }
        void tryrdlock() { uvResult( uv_rwlock_tryrdlock(*this) ); }
        void rdunlock() { uv_rwlock_rdunlock(*this); }

        void wrlock() { uv_rwlock_wrlock(*this); }
        void trywrlock() { uvResult( uv_rwlock_trywrlock(*this) ); }
        void wrunlock() { uv_rwlock_wrunlock(*this); }
    };

    struct Semaphore : BasicRef_t< uv_sem_t > {
        typedef BasicRef_t< uv_sem_t > Base_t;
        Semaphore(unsigned int value) : Base_t() { init(value); }
        void init(unsigned int value) { uvResult( uv_sem_init(*this, value) ); }
        void destroy() { uv_sem_destroy(*this); Base_t::destroy(); }
        void post() { uv_sem_post(*this); }
        void wait() { uv_sem_wait(*this); }
        void trywait() { uvResult( uv_sem_trywait(*this) ); }
    };
}
