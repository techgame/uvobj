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
#include "./uvObj_events.hpp"

namespace uvObj {
    struct Connect : Ref_t< uv_connect_t > {
        typedef Ref_t< uv_connect_t > Base_t;
        typedef uvObj::req_events_t<Connect> evt;

        Connect() { init(this); }
        Connect* init(void* self, uv_connect_cb cb) {
            Base_t::setCallback(self, cb);
            return this; }
        template <typename T>
        Connect* init(T* self) { return init(self, T::evt::on_connect); }

        template <typename uvobj_t>
        void connect(uvobj_t& obj, const char* ip, int port) {
            obj.connect(*this, ip, port, Base_t::_ref_->cb); }
        template <typename uvobj_t>
        void connect6(uvobj_t& obj, const char* ip, int port) {
            obj.connect6(*this, ip, port, Base_t::_ref_->cb); }

        template <typename uvobj_t>
        void connect(uvobj_t& obj, const struct sockaddr_in& addr) {
            obj.connect(*this, addr, Base_t::_ref_->cb); }
        template <typename uvobj_t>
        void connect6(uvobj_t& obj, const struct sockaddr_in6& addr) {
            obj.connect6(*this, addr, Base_t::_ref_->cb); }

        void on_connect(uv_connect_t* req, int status) { }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Shutdown : Ref_t< uv_shutdown_t > {
        typedef Ref_t< uv_shutdown_t > Base_t;
        typedef uvObj::req_events_t<Shutdown> evt;

        Shutdown() { init(this); }
        template <typename T>
        Shutdown(T* self) { init(self); }
        template <typename T>
        Shutdown(T* self, uv_shutdown_cb cb) { init(self, cb); }

        template <typename T>
        void init(T* self, uv_shutdown_cb cb) {
            Base_t::setCallback(self, cb); }
        template <typename T>
        void init(T* self) { init(self, T::evt::on_shutdown); }

        template <typename uvobj_t>
        void perform(uvobj_t& obj) {
            obj.shutdown(*this, Base_t::_ref_->cb); }
        template <typename uvobj_t>
        void operator()(uvobj_t& obj) { perform(obj); }

        void on_shutdown(uv_shutdown_t* req, int status) { }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct UDPSend : Ref_t< uv_udp_send_t > {
        typedef Ref_t< uv_udp_send_t > Base_t;
        typedef uvObj::req_events_t<UDPSend> evt;

        UDPSend() { init(this); }
        UDPSend(uv_buf_t buf) { init(this); push(buf); }
        UDPSend(const char* buf) { init(this); push(buf); }
        UDPSend(const char* buf, unsigned int len) { init(this); push(buf, len); }

        template <typename T>
        UDPSend(T* self) { init(self, T::on_udp_send); }
        template <typename T>
        UDPSend(T* self, uv_udp_send_cb cb) { init(self, cb); }

        template <typename T>
        void init(T* self, uv_udp_send_cb cb) {
            Base_t::setCallback(self, cb); }
        template <typename T>
        void init(T* self) { init(self, T::evt::on_udp_send); }

        std::vector<uv_buf_t> bufs;
        UDPSend& push(uv_buf_t buf) { bufs.push_back(buf); return *this; }
        UDPSend& push(const char* buf) { return push(buf, ::strlen(buf)); }
        UDPSend& push(const char* buf, unsigned int len) {
            bufs.push_back(uv_buf_init(const_cast<char*>(buf), len));
            return *this; }

        template <typename uvobj_t>
        void send(uvobj_t& obj, const char* ip, int port) {
            obj.send(*this, &bufs[0], bufs.size(), ip, port, Base_t::_ref_->cb); }
        template <typename uvobj_t>
        void send6(uvobj_t& obj, const char* ip, int port) {
            obj.send(*this, &bufs[0], bufs.size(), ip, port, Base_t::_ref_->cb); }

        template <typename uvobj_t>
        void send(uvobj_t& obj, const struct sockaddr_in& addr) {
            obj.send(*this, &bufs[0], bufs.size(), addr, Base_t::_ref_->cb); }
        template <typename uvobj_t>
        void send6(uvobj_t& obj, const struct sockaddr_in6& addr) {
            obj.send6(*this, &bufs[0], bufs.size(), addr, Base_t::_ref_->cb); }

        void on_udp_send(uv_udp_send_t* req, int status) { }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Write : Ref_t< uv_write_t > {
        typedef Ref_t< uv_write_t > Base_t;
        typedef uvObj::req_events_t<Write> evt;

        Write() { init(this); }
        Write(uv_buf_t buf) { init(this); push(buf); }
        Write(const char* buf) { init(this); push(buf); }
        Write(const char* buf, unsigned int len) { init(this); push(buf, len); }

        template <typename T>
        Write(T* self) { init(self); }
        template <typename T>
        Write(T* self, uv_write_cb cb) { init(self, cb); }

        template <typename T>
        void init(T* self, uv_write_cb cb) {
            Base_t::setCallback(self, cb); }
        template <typename T>
        void init(T* self) { init(self, T::evt::on_write); }

        std::vector<uv_buf_t> bufs;
        Write& push(uv_buf_t buf) { bufs.push_back(buf); return *this; }
        Write& push(const char* buf) { return push(buf, ::strlen(buf)); }
        Write& push(const char* buf, unsigned int len) {
            bufs.push_back(uv_buf_init(const_cast<char*>(buf), len));
            return *this; }

        template <typename uvobj_t>
        void write(uvobj_t& obj) {
            obj.write(*this, &bufs[0], bufs.size(), Base_t::_ref_->cb); }
        template <typename uvobj_t>
        void operator()(uvobj_t& obj) {
            write(obj); }

        template <typename uvobj_t>
        void write2(uvobj_t& obj, uv_stream_t* send_handle) {
            obj.write2(*this, &bufs[0], bufs.size(), send_handle, Base_t::_ref_->cb); }
        template <typename uvobj_t>
        void operator()(uvobj_t& obj, uv_stream_t* send_handle) {
            write(obj, send_handle); }

        void on_write(uv_write_t* req, int status) { }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Work : Ref_t< uv_work_t > {
        typedef Ref_t< uv_work_t > Base_t;
        void queue(uv_work_cb work_cb, uv_after_work_cb after_cb) {
            queue(NULL, work_cb, after_cb); }
        void queue(uv_loop_t* loop, uv_work_cb work_cb, uv_after_work_cb after_cb) {
            Base_t::uvRes( uv_queue_work(_as_loop(loop), *this, work_cb, after_cb) ); }
        template <typename T>
        void queue(T* self, uv_loop_t* loop=NULL) {
            Base_t::setData(self); queue(loop, T::evt::on_work, T::evt::on_after_work); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct GetAddrInfo : Ref_t< uv_getaddrinfo_t > {
        typedef Ref_t< uv_getaddrinfo_t > Base_t;

        void perform(uv_getaddrinfo_cb getaddrinfo_cb,
                const char* node, const char* service, const struct addrinfo* hints) {
            perform(NULL, getaddrinfo_cb, node, service, hints); }
        template <typename T>
        void perform(T* self, const char* node, const char* service, const struct addrinfo* hints) {
            Base_t::setData(self); perform(NULL, T::evt::on_getaddrinfo, node, service, hints); }

        void perform(uv_loop_t* loop, uv_getaddrinfo_cb getaddrinfo_cb,
                const char* node, const char* service, const struct addrinfo* hints) {
            Base_t::uvRes( uv_getaddrinfo(loop, *this, getaddrinfo_cb, node, service, hints) ); }
        template <typename T>
        void perform(T* self, uv_loop_t* loop, const char* node, const char* service, const struct addrinfo* hints) {
            Base_t::setData(self); perform(loop, T::evt::on_getaddrinfo, node, service, hints); }

        static void freeaddrinfo(struct addrinfo* ai) { uv_freeaddrinfo(ai); }
    };
}
