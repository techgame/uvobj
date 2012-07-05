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
#include "./uvObj_utils.hpp"
#include "./uvObj_events.hpp"

namespace uvObj {
    struct RequestSink {
        typedef req_events_t< RequestSink > evt;

        void on_connect(uv_connect_t* req, int status) {}
        void on_shutdown(uv_shutdown_t* req, int status) {}
        void on_udp_send(uv_udp_send_t* req, int status) {}
        void on_write(uv_write_t* req, int status) {}
        void on_work(uv_work_t* req) {}
        void on_after_work(uv_work_t* req) {}
        void on_getaddrinfo(uv_getaddrinfo_t* req, int status, struct addrinfo* res) {}
    };
    inline static RequestSink* req_noop() { return NULL; }

    template <typename uv_t, typename uv_handle_t>
    struct ReqRef_t : Ref_t< uv_t > {
        ReqRef_t(uv_handle_t* handle_p, bool delSelf=false)
         : _handle(handle_p), _delSelf(delSelf) {}
        virtual ~ReqRef_t() {}
        virtual void finalizeRequest() {
            _handle = NULL;
            if (_delSelf) delete this; }
        inline bool isValid() { return !!_handle; }
        inline bool assertValid() { assert(isValid()); return true; }
        inline bool uvReqRes(int res) { return uvResult(res, _handle); }

        uv_handle_t* _handle;
        bool _delSelf;
    };

    struct TCPConnectOp : ReqRef_t< uv_connect_t, uv_tcp_t > {
        typedef ReqRef_t< uv_connect_t, uv_tcp_t > Base_t;
        typedef uvObj::req_events_t<TCPConnectOp> evt;

        TCPConnectOp(uv_tcp_t* handle, bool delSelf=false)
         : Base_t(handle, delSelf) { bind(req_noop()); }

        TCPConnectOp* bind(void* self, uv_connect_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        template <typename T>
        TCPConnectOp* bind(T* self) {
            return bind(self, T::evt::on_connect); }

        void connect(const char* ip, int port) {
            connect(IP::addr(ip, port)); }
        void connect(const struct sockaddr_in& addr) {
            Base_t::uvReqRes( uv_tcp_connect(*this, _handle, addr, Base_t::_ref_->cb) );
            finalizeRequest(); }

        void connect6(const char* ip, int port) {
            connect6(IP::addr6(ip, port)); }
        void connect6(const struct sockaddr_in6& addr) {
            Base_t::uvReqRes( uv_tcp_connect6(*this, _handle, addr, Base_t::_ref_->cb) );
            finalizeRequest(); }
    };

    struct PipeConnect : ReqRef_t< uv_connect_t, uv_pipe_t > {
        typedef ReqRef_t< uv_connect_t, uv_pipe_t > Base_t;
        typedef uvObj::req_events_t<PipeConnect> evt;

        PipeConnect(uv_pipe_t* handle, bool delSelf=false)
         : Base_t(handle, delSelf) { bind(req_noop()); }

        PipeConnect* bind(void* self, uv_connect_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        template <typename T>
        PipeConnect* bind(T* self) {
            return bind(self, T::evt::on_connect); }

        void connect(const char* name) {
            uv_pipe_connect(*this, _handle, name, Base_t::_ref_->cb);
            finalizeRequest(); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct ShutdownOp : ReqRef_t< uv_shutdown_t, uv_stream_t > {
        typedef ReqRef_t< uv_shutdown_t, uv_stream_t > Base_t;
        typedef uvObj::req_events_t<ShutdownOp> evt;

        ShutdownOp(uv_stream_t* handle, bool delSelf=false)
         : Base_t(handle, delSelf) { bind(req_noop()); }

        ShutdownOp* bind(void* self, uv_shutdown_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        template <typename T>
        ShutdownOp* bind(T* self) {
            return bind(self, T::evt::on_shutdown); }

        void shutdown() {
            Base_t::uvReqRes( uv_shutdown(*this, _handle, Base_t::_ref_->cb) );
            finalizeRequest(); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct UDPSend : ReqRef_t< uv_udp_send_t, uv_udp_t > {
        typedef ReqRef_t< uv_udp_send_t, uv_udp_t > Base_t;
        typedef uvObj::req_events_t<UDPSend> evt;

        UDPSend(uv_udp_t* handle, bool delSelf=false)
         : Base_t(handle, delSelf) { bind(req_noop()); }

        UDPSend* bind(void* self, uv_udp_send_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        template <typename T>
        UDPSend* bind(T* self) {
            return bind(self, T::evt::on_udp_send); }

        std::vector<uv_buf_t> bufs;
        UDPSend* push(uv_buf_t buf) { bufs.push_back(buf); return this; }
        UDPSend* push(const char* buf) { return push(buf, ::strlen(buf)); }
        UDPSend* push(const char* buf, unsigned int len) {
            bufs.push_back(uv_buf_init(const_cast<char*>(buf), len)); return this; }

        void send(const char* ip, int port) { send(IP::addr(ip, port)); }
        void send(const struct sockaddr_in& addr) {
            Base_t::uvReqRes( uv_udp_send(*this, _handle, &bufs[0], bufs.size(), addr, Base_t::_ref_->cb) );
            finalizeRequest(); }

        void send6(const char* ip, int port) { send6(IP::addr6(ip, port)); }
        void send6(const struct sockaddr_in6& addr) {
            Base_t::uvReqRes( uv_udp_send6(*this, _handle, &bufs[0], bufs.size(), addr, Base_t::_ref_->cb) );
            finalizeRequest(); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct StreamWrite : ReqRef_t< uv_write_t, uv_stream_t > {
        typedef ReqRef_t< uv_write_t, uv_stream_t > Base_t;
        typedef uvObj::req_events_t<StreamWrite> evt;

        StreamWrite(uv_stream_t* handle, bool delSelf=false)
         : Base_t(handle, delSelf) { bind(req_noop()); }

        StreamWrite* bind(void* self, uv_write_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        template <typename T>
        StreamWrite* bind(T* self) {
            return bind(self, T::evt::on_write); }

        std::vector<uv_buf_t> bufs;
        StreamWrite* push(uv_buf_t buf) { bufs.push_back(buf); return this; }
        StreamWrite* push(const char* buf) { return push(buf, ::strlen(buf)); }
        StreamWrite* push(const char* buf, unsigned int len) {
            bufs.push_back(uv_buf_init(const_cast<char*>(buf), len)); return this; }

        void write() {
            Base_t::uvReqRes( uv_write(*this, _handle, &bufs[0], bufs.size(), Base_t::_ref_->cb) );
            finalizeRequest(); }
        void write(uv_stream_t* send_handle) { write2(send_handle); }
        void write2(uv_stream_t* send_handle) {
            Base_t::uvReqRes( uv_write2(*this, _handle, &bufs[0], bufs.size(), send_handle, Base_t::_ref_->cb) );
            finalizeRequest(); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Work : Ref_t< uv_work_t > {
        typedef Ref_t< uv_work_t > Base_t;
        Work() {}
        template <typename T>
        Work(T* self, uv_loop_t* loop=NULL) {
            queue(self, loop); }
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

        GetAddrInfo() { bind(req_noop()); }
        GetAddrInfo* bind(void* self, uv_getaddrinfo_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        template <typename T>
        GetAddrInfo* bind(T* self) {
            return bind(self, T::evt::on_getaddrinfo); }

        void lookup(const char* node, const char* service, const struct addrinfo* hints=NULL) {
            lookup(NULL, node, service, hints); }
        void lookup(uv_loop_t* loop, const char* node, const char* service, const struct addrinfo* hints=NULL) {
            Base_t::uvRes( uv_getaddrinfo(_as_loop(loop), *this, Base_t::_ref_->cb, node, service, hints) ); }

        static void freeaddrinfo(struct addrinfo* ai) { uv_freeaddrinfo(ai); }
    };
}
