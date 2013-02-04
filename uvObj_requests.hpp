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
    template <typename uv_t, uv_req_type req_type, typename uv_handle_t>
    struct RequestRef_t : Request_t< uv_t, req_type > {
        RequestRef_t(uv_handle_t* handle_p, bool finalizeSelf=false)
         : _handle(handle_p), _finalizeSelf(finalizeSelf) {}
        virtual ~RequestRef_t() {}
        virtual void finalizeRequest() {
            _handle = NULL;
            if (_finalizeSelf) delete this; }
        bool isValid() { return !!_handle; }
        bool _uvReqRes(int res) { return uvResult(res, _handle); }

        uv_handle_t* _handle;
        bool _finalizeSelf;
    };

    struct TCPConnectOp : RequestRef_t< uv_connect_t, UV_CONNECT, uv_tcp_t > {
        typedef RequestRef_t< uv_connect_t, UV_CONNECT, uv_tcp_t > Base_t;

        TCPConnectOp(uv_tcp_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf) { bind(blackhole()); }

        TCPConnectOp* bind(void* self, uv_connect_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        TCPConnectOp* bind(const BoundEvt<uv_connect_cb>& evt) {
            return bind(evt.tgt, evt.cb); }

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

    struct PipeConnect : RequestRef_t< uv_connect_t, UV_CONNECT, uv_pipe_t > {
        typedef RequestRef_t< uv_connect_t, UV_CONNECT, uv_pipe_t > Base_t;

        PipeConnect(uv_pipe_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf) { bind(blackhole()); }

        PipeConnect* bind(void* self, uv_connect_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        PipeConnect* bind(const BoundEvt<uv_connect_cb>& evt) {
            return bind(evt.tgt, evt.cb); }

        void connect(const char* name) {
            uv_pipe_connect(*this, _handle, name, Base_t::uv->cb);
            finalizeRequest(); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct ShutdownOp : RequestRef_t< uv_shutdown_t, UV_SHUTDOWN, uv_stream_t > {
        typedef RequestRef_t< uv_shutdown_t, UV_SHUTDOWN, uv_stream_t > Base_t;

        ShutdownOp(uv_stream_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf) { bind(blackhole()); }

        ShutdownOp* bind(void* self, uv_shutdown_cb cb) {
            Base_t::setCallback(self, cb); return this; }
        ShutdownOp* bind(const BoundEvt<uv_shutdown_cb>& evt) {
            return bind(evt.tgt, evt.cb); }

        void shutdown() {
            Base_t::_uvReqRes( uv_shutdown(*this, _handle, Base_t::uv->cb) );
            finalizeRequest(); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    typedef void (*uv_buf_release_cb)(uv_buf_t& buf);
    struct BufferManager {
        typedef std::vector<uv_buf_t> vec;
        typedef evt_t<BufferManager> evt;

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
        bool on_udp_send(uv_udp_send_t* req, int status) {
            if (cb.send) {
                req->data = data; req->cb = cb.send;
                cb.send(req, status);
            }
            finalize();
            // only cleanup if cb.send was not called
            return !cb.send; }
        bool on_write(uv_write_t* req, int status) {
            uv_write_cb write = cb.write;
            if (write) {
                req->data = data; req->cb = write;
                write(req, status);
            }
            finalize();
            // only cleanup if cb.write was not called
            return !write; }

    protected:
        void* data;
        union { void* cb; uv_udp_send_cb send; uv_write_cb write; } cb;
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename BufferMgr_t>
    struct UDPSend_t : RequestRef_t< uv_udp_send_t, UV_UDP_SEND, uv_udp_t > {
        typedef RequestRef_t< uv_udp_send_t, UV_UDP_SEND, uv_udp_t > Base_t;

        UDPSend_t(uv_udp_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf), mgr(new BufferMgr_t())
        { Base_t::setCallback(BufferMgr_t::evt::on_udp_send(mgr)); }
        ~UDPSend_t() { delete mgr; }

        UDPSend_t* bind(void* self, uv_udp_send_cb cb) {
            if (mgr) mgr->bind(self, cb); return this; }
        UDPSend_t* bind(const BoundEvt<uv_udp_send_cb>& evt) {
            return bind(evt.tgt, evt.cb); }

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
    struct StreamWrite_t : RequestRef_t< uv_write_t, UV_WRITE, uv_stream_t > {
        typedef RequestRef_t< uv_write_t, UV_WRITE, uv_stream_t > Base_t;

        StreamWrite_t(uv_stream_t* handle, bool finalizeSelf=false)
         : Base_t(handle, finalizeSelf), mgr(new BufferMgr_t())
        { Base_t::setCallback(BufferMgr_t::evt::on_write(mgr)); }
        ~StreamWrite_t() { delete mgr; }

        StreamWrite_t* bind(void* self, uv_write_cb cb) {
            if (mgr) mgr->bind(self, cb); return this; }
        StreamWrite_t* bind(const BoundEvt<uv_write_cb>& evt) {
            return bind(evt.tgt, evt.cb); }

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
            Base_t::_uvReqRes( uv_write(*this, _handle, mgr->ptr(), (int)mgr->size(), Base_t::uv->cb) );
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

    struct Work : Request_t< uv_work_t, UV_WORK > {
        typedef Request_t< uv_work_t, UV_WORK > Base_t;

        Work() {}
        Work(const BoundWorkEvt& evt, uv_loop_t* loop=NULL) {
            queue(evt, loop); }
        void queue(uv_work_cb work_cb, uv_after_work_cb after_cb) {
            queue(NULL, work_cb, after_cb); }
        void queue(uv_loop_t* loop, uv_work_cb work_cb, uv_after_work_cb after_cb) {
            Base_t::_uvRes( uv_queue_work(_as_loop(loop), *this, work_cb, after_cb) ); }
        void queue(const BoundWorkEvt& evt, uv_loop_t* loop=NULL) {
            Base_t::setData(evt.tgt); queue(loop, evt.work, evt.after_work); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct GetAddrInfo : Request_t< uv_getaddrinfo_t, UV_GETADDRINFO > {
        typedef Request_t< uv_getaddrinfo_t, UV_GETADDRINFO > Base_t;

        GetAddrInfo() { bind(blackhole()); }
        GetAddrInfo* bind(void* self, uv_getaddrinfo_cb cb) {
            setCallback(self, cb); return this; }
        GetAddrInfo* bind(const BoundEvt<uv_getaddrinfo_cb>& evt) {
            return bind(evt.tgt, evt.cb); }

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
