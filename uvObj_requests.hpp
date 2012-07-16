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
    template <typename uv_t, typename uv_handle_t>
    struct ReqRef_t : Ref_t< uv_t > {
        ReqRef_t(uv_handle_t* handle_p, bool finalizeSelf=false)
         : _handle(handle_p), _finalizeSelf(finalizeSelf) {}
        virtual ~ReqRef_t() {}
        virtual void finalizeRequest() {
            _handle = NULL;
            if (_finalizeSelf) delete this; }
        bool isValid() { return !!_handle; }
        bool _uvReqRes(int res) { return uvResult(res, _handle); }

        uv_handle_t* _handle;
        bool _finalizeSelf;
    };

    struct TCPConnectOp : ReqRef_t< uv_connect_t, uv_tcp_t > {
        typedef ReqRef_t< uv_connect_t, uv_tcp_t > Base_t;

        TCPConnectOp(uv_tcp_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf) { bind(blackhole()); }

        TCPConnectOp* bind(void* self, uv_connect_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        template <typename T>
        TCPConnectOp* bind(T* self) {
            return bind(self, T::evt::on_connect); }

        void connect(const char* ip, int port) {
            connect(IP::addr(ip, port)); }
        void connect(const struct sockaddr_in& addr) {
            Base_t::_uvReqRes( uv_tcp_connect(*this, _handle, addr, Base_t::uv->cb) );
            finalizeRequest(); }

        void connect6(const char* ip, int port) {
            connect6(IP::addr6(ip, port)); }
        void connect6(const struct sockaddr_in6& addr) {
            Base_t::_uvReqRes( uv_tcp_connect6(*this, _handle, addr, Base_t::uv->cb) );
            finalizeRequest(); }
    };

    struct PipeConnect : ReqRef_t< uv_connect_t, uv_pipe_t > {
        typedef ReqRef_t< uv_connect_t, uv_pipe_t > Base_t;

        PipeConnect(uv_pipe_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf) { bind(blackhole()); }

        PipeConnect* bind(void* self, uv_connect_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        template <typename T>
        PipeConnect* bind(T* self) {
            return bind(self, T::evt::on_connect); }

        void connect(const char* name) {
            uv_pipe_connect(*this, _handle, name, Base_t::uv->cb);
            finalizeRequest(); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct ShutdownOp : ReqRef_t< uv_shutdown_t, uv_stream_t > {
        typedef ReqRef_t< uv_shutdown_t, uv_stream_t > Base_t;

        ShutdownOp(uv_stream_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf) { bind(blackhole()); }

        ShutdownOp* bind(void* self, uv_shutdown_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        template <typename T>
        ShutdownOp* bind(T* self) {
            return bind(self, T::evt::on_shutdown); }

        void shutdown() {
            Base_t::_uvReqRes( uv_shutdown(*this, _handle, Base_t::uv->cb) );
            finalizeRequest(); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct BufferManager {
        typedef req_events_proxy_t< BufferManager > evt;
        typedef std::vector<uv_buf_t> vec;

        BufferManager() : data(0), cb_release(NULL) {cb.cb=NULL;}

        uv_buf_release_cb cb_release;
        vec bufs;

        uv_buf_t* ptr() { return &bufs[0]; }
        size_t size() { return bufs.size(); }

        void push(uv_buf_t buf) { bufs.push_back(buf); }
        void push(const char* buf) { push(buf, ::strlen(buf)); }
        void push_z(const char* buf) { push(buf, ::strlen(buf)+1); }
        void push(const char* buf, size_t len) {
            bufs.push_back(uv_buf_init(const_cast<char*>(buf), (unsigned int)len)); }
        void bindCleanup(uv_buf_release_cb cb) {
            cb_release = cb; }
        void bindCleanup(bool useFree) {
            bindCleanup(useFree ? uvObj::buf_free : NULL); }

        void finalize() {
            if (cb_release) {
                vec::iterator iter=bufs.begin(), end=bufs.end();
                for(;iter!=end;++iter) cb_release(*iter);
            }
            bufs.clear();
            delete this; }

        /* Event proxying */
        void bind(void* self, uv_udp_send_cb send) {
            data = self; cb.send = send; }
        void bind(void* self, uv_write_cb write) {
            data = self; cb.write = write; }
        void on_udp_send(uv_udp_send_t* req, int status) {
            if (cb.send) {
                req->data = data; req->cb = cb.send;
                cb.send(req, status);
            } finalize(); }
        void on_write(uv_write_t* req, int status) {
            if (cb.write) {
                req->data = data; req->cb = cb.write;
                cb.write(req, status);
            } finalize(); }

    protected:
        void* data;
        union { void* cb; uv_udp_send_cb send; uv_write_cb write; } cb;
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename BufferMgr_t>
    struct UDPSend_t : ReqRef_t< uv_udp_send_t, uv_udp_t > {
        typedef ReqRef_t< uv_udp_send_t, uv_udp_t > Base_t;

        UDPSend_t(uv_udp_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf), mgr(new BufferMgr_t())
        { Base_t::setCallback<uv_udp_send_cb>(mgr, &BufferMgr_t::evt::on_udp_send); }
        ~UDPSend_t() { delete mgr; }

        UDPSend_t* bind(void* self, uv_udp_send_cb cb) {
            if (mgr) mgr->bind(self, cb); return this; }
        template <typename T>
        UDPSend_t* bind(T* self) {
            return bind(self, T::evt::on_udp_send); }

        BufferMgr_t* mgr;
        UDPSend_t* push(uv_buf_t buf) {
            if (mgr) mgr->push(buf); return this; }
        UDPSend_t* push(const char* buf) {
            if (mgr) mgr->push(buf); return this; }
        UDPSend_t* push_z(const char* buf) {
            if (mgr) mgr->push_z(buf); return this; }
        UDPSend_t* push(const char* buf, unsigned int len) {
            if (mgr) mgr->push(buf, len); return this; }
        template <typename Fn>
        UDPSend_t* bindCleanup(Fn fn) {
            if (mgr) mgr->bindCleanup(fn); return this; }

        void send(const char* ip, int port) { send(IP::addr(ip, port)); }
        void send(const struct sockaddr_in& addr) {
            if (!mgr) return;
            Base_t::_uvReqRes( uv_udp_send(*this, _handle, mgr->ptr(), mgr->size(), addr, Base_t::uv->cb) );
            mgr = NULL; // allow mgr to look after itself now
            finalizeRequest(); }

        void send6(const char* ip, int port) { send6(IP::addr6(ip, port)); }
        void send6(const struct sockaddr_in6& addr) {
            if (!mgr) return;
            Base_t::_uvReqRes( uv_udp_send6(*this, _handle, mgr->ptr(), mgr->size(), addr, Base_t::uv->cb) );
            mgr = NULL; // allow mgr to look after itself now
            finalizeRequest(); }
    };
    typedef UDPSend_t< BufferManager > UDPSend;

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename BufferMgr_t>
    struct StreamWrite_t : ReqRef_t< uv_write_t, uv_stream_t > {
        typedef ReqRef_t< uv_write_t, uv_stream_t > Base_t;

        StreamWrite_t(uv_stream_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf), mgr(new BufferMgr_t())
        { Base_t::setCallback<uv_write_cb>(mgr, &BufferMgr_t::evt::on_write); }
        ~StreamWrite_t() { delete mgr; }

        StreamWrite_t* bind(void* self, uv_write_cb cb) {
            if (mgr) mgr->bind(self, cb); return this; }
        template <typename T>
        StreamWrite_t* bind(T* self) {
            return bind(self, T::evt::on_write); }

        BufferMgr_t* mgr;
        StreamWrite_t* push(uv_buf_t buf) {
            if (mgr) mgr->push(buf); return this; }
        StreamWrite_t* push(const char* buf) {
            if (mgr) mgr->push(buf); return this; }
        StreamWrite_t* push_z(const char* buf) {
            if (mgr) mgr->push_z(buf); return this; }
        StreamWrite_t* push(const char* buf, unsigned int len) {
            if (mgr) mgr->push(buf, len); return this; }
        template <typename Fn>
        StreamWrite_t* bindCleanup(Fn fn) {
            if (mgr) mgr->bindCleanup(fn); return this; }

        void write() {
            if (!mgr) return;
            Base_t::_uvReqRes( uv_write(*this, _handle, mgr->ptr(), mgr->size(), Base_t::uv->cb) );
            mgr = NULL; // allow mgr to look after itself now
            finalizeRequest(); }
        void write2(uv_stream_t* send_handle) {
            if (!mgr) return;
            Base_t::_uvReqRes( uv_write2(*this, _handle, mgr->ptr(), mgr->size(), send_handle, Base_t::uv->cb) );
            mgr = NULL; // allow mgr to look after itself now
            finalizeRequest(); }
    };
    typedef StreamWrite_t< BufferManager > StreamWrite;

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
            Base_t::_uvRes( uv_queue_work(_as_loop(loop), *this, work_cb, after_cb) ); }
        template <typename T>
        void queue(T* self, uv_loop_t* loop=NULL) {
            Base_t::setData(self); queue(loop, T::evt::on_work, T::evt::on_after_work); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct GetAddrInfo : Ref_t< uv_getaddrinfo_t > {
        typedef Ref_t< uv_getaddrinfo_t > Base_t;

        GetAddrInfo() { bind(blackhole()); }
        GetAddrInfo* bind(void* self, uv_getaddrinfo_cb cb) {
            setCallback(self, cb); return this; }
        template <typename T>
        GetAddrInfo* bind(T* self) {
            return bind(self, T::evt::on_getaddrinfo); }

        /* a little private api inconsistency */
        #ifdef _WIN32
        uv_getaddrinfo_cb callback() {
            return Base_t::uv->getaddrinfo_cb; }
        void setCallback(void* data, uv_getaddrinfo_cb cb) {
            if (!Base_t::uv) return;
            Base_t::uv->data = data;
            Base_t::uv->getaddrinfo_cb = cb; }
        #else
        uv_getaddrinfo_cb callback() {
            return uv->cb; }
        void setCallback(void* data, uv_getaddrinfo_cb cb) {
            Base_t::setCallback(data, cb); }
        #endif

        void lookup(const char* node, const char* service, const struct addrinfo* hints=NULL) {
            lookup(NULL, node, service, hints); }
        void lookup(uv_loop_t* loop, const char* node, const char* service, const struct addrinfo* hints=NULL) {
            Base_t::_uvRes( uv_getaddrinfo(_as_loop(loop), *this, callback(), node, service, hints) ); }

        static void freeaddrinfo(struct addrinfo* ai) { uv_freeaddrinfo(ai); }
    };
}
