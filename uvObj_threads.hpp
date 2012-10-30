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

    struct Once : BasicRef_t< uv_once_t > {
        typedef BasicRef_t< uv_once_t > Base_t;
        Once(once_cb callback) : Base_t() { uv_once(*this, callback); }
    };

    struct Thread : BasicRef_t< uv_thread_t > {
        typedef BasicRef_t< uv_thread_t > Base_t;
        Thread(thread_cb fn, void* arg) : Base_t() { create(fn, arg); }
        Thread(const BoundEvt<thread_cb>& evt) : Base_t() { create(evt); }
        void create(const BoundEvt<thread_cb>& evt) { create(evt.cb, evt.tgt); }
        void create(thread_cb fn, void* arg) { uvResult( uv_thread_create(*this, fn, arg) ); }
        void join() { uvResult( uv_thread_join(*this) ); }
    };
}
