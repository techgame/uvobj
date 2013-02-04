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
#include "./uvObj_requests.hpp"

namespace uvObj {
    struct UDP : Handle_t< uv_udp_t, UV_UDP > {
        typedef Handle_t< uv_udp_t, UV_UDP > Base_t;
        UDP(uv_loop_t* loop) : Base_t() { init(loop); }
        UDP() : Base_t() { init(); }
        explicit UDP(uv_udp_t* ref) : Base_t(ref) { }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_udp_init(_as_loop(loop), *this) ); }

        void bind(const char* ip, int port, unsigned flags) {
            Base_t::_uvRes( uv_udp_bind(*this, IP::addr(ip, port), flags) ); }
        void bind6(const char* ip, int port, unsigned flags) {
            Base_t::_uvRes( uv_udp_bind6(*this, IP::addr6(ip, port), flags) ); }
        void bind(const struct sockaddr_in& addr, unsigned flags) {
            Base_t::_uvRes( uv_udp_bind(*this, addr, flags) ); }
        void bind6(const struct sockaddr_in6& addr, unsigned flags) {
            Base_t::_uvRes( uv_udp_bind6(*this, addr, flags) ); }
        std::string getsockurl(const char* schema="udp", const char* path="") {
            return getsockname().url(schema, path); }
        IP getsockname() { IP addr; int len=sizeof(addr.sa);
            uv_udp_getsockname(*this, &addr.sa.raw, &len);
            return addr; }
        void getsockname(struct sockaddr* name, int* namelen) {
            Base_t::_uvRes( uv_udp_getsockname(*this, name, namelen) ); }

        void set_membership(const char* multicast_addr,
                const char* interface_addr, uv_membership membership) {
            Base_t::_uvRes( uv_udp_set_membership(*this, multicast_addr,
                        interface_addr, membership) ); }
        void set_multicast_loop(int on) {
            Base_t::_uvRes( uv_udp_set_multicast_loop(*this, on) ); }
        void set_multicast_ttl(int ttl) {
            Base_t::_uvRes( uv_udp_set_multicast_ttl(*this, ttl) ); }
        void set_broadcast(int on) {
            Base_t::_uvRes( uv_udp_set_broadcast(*this, on) ); }
        void set_ttl(int ttl) {
            Base_t::_uvRes( uv_udp_set_ttl(*this, ttl) ); }

        void recv_start(uv_alloc_cb alloc_cb, uv_udp_recv_cb recv_cb) {
            Base_t::_uvRes( uv_udp_recv_start(*this, alloc_cb, recv_cb) ); }
        void recv_start(const BoundAllocEvt<uv_udp_recv_cb>& evt) {
            Base_t::setData(evt.tgt); recv_start(evt.alloc, evt.cb); }
        void recv_stop() {
            Base_t::_uvRes( uv_udp_recv_stop(*this) ); }

        void send(uv_udp_send_t* req, uv_buf_t bufs[], int bufcnt,
                const char* ip, int port, uv_udp_send_cb cb) {
            Base_t::_uvRes( uv_udp_send(req, *this, bufs, bufcnt, IP::addr(ip, port), cb) ); }
        void send(uv_udp_send_t* req, uv_buf_t bufs[], int bufcnt,
                const struct sockaddr_in& addr, uv_udp_send_cb cb) {
            Base_t::_uvRes( uv_udp_send(req, *this, bufs, bufcnt, addr, cb) ); }

        void send6(uv_udp_send_t* req, uv_buf_t bufs[], int bufcnt,
                const char* ip, int port, uv_udp_send_cb cb) {
            Base_t::_uvRes( uv_udp_send6(req, *this, bufs, bufcnt, IP::addr6(ip, port), cb) ); }
        void send6(uv_udp_send_t* req, uv_buf_t bufs[], int bufcnt,
                const struct sockaddr_in6& addr, uv_udp_send_cb cb) {
            Base_t::_uvRes( uv_udp_send6(req, *this, bufs, bufcnt, addr, cb) ); }

        UDPSend* sendOp(bool useBufFree) {
            return (new UDPSend(*this, true))->bindCleanup(useBufFree); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename uv_t,uv_handle_type handle_type>
    struct Stream_t : Handle_t< uv_t, handle_type > {
        typedef Handle_t< uv_t, handle_type > Base_t;

        explicit Stream_t(uv_t* ref) : Base_t(ref) {}
        explicit Stream_t(uv_stream_t* ref)
         : Base_t(reinterpret_cast<uv_t*>(ref)) {}
        explicit Stream_t(uv_handle_t* ref)
         : Base_t(reinterpret_cast<uv_t*>(ref)) {}
        Stream_t() : Base_t() {}

        inline uv_stream_t* asStream() {
            return reinterpret_cast<uv_stream_t*>(Base_t::uv); }
        inline operator uv_stream_t*() { return asStream(); }

        void listen(int backlog, uv_connection_cb cb) {
            Base_t::_uvRes( uv_listen(*this, backlog, cb) ); }
        void listen(const BoundEvt<uv_connection_cb>& evt, int backlog) {
            Base_t::setData(evt.tgt); listen(backlog, evt.cb); }

        void accept(uv_stream_t* client) {
            Base_t::_uvRes( uv_accept(*this, client) ); }

        bool wasEOF() { return Base_t::last_error().code == UV_EOF; }

        bool is_readable() { return uv_is_readable(*this) != 0; }
        void read_start(uv_alloc_cb alloc_cb, uv_read_cb read_cb) {
            Base_t::_uvRes( uv_read_start(*this, alloc_cb, read_cb) ); }
        void read_start(const BoundAllocEvt<uv_read_cb>& evt) {
            Base_t::setData(evt.tgt); read_start(evt.alloc, evt.cb); }
        void read2_start(uv_alloc_cb alloc_cb, uv_read2_cb read_cb) {
            Base_t::_uvRes( uv_read2_start(*this, alloc_cb, read_cb) ); }
        void read2_start(const BoundAllocEvt<uv_read2_cb>& evt) {
            Base_t::setData(evt.tgt); read2_start(evt.alloc, evt.cb); }
        void read_stop() {
            Base_t::_uvRes( uv_read_stop(*this) ); }

        bool is_writable() { return uv_is_writable(*this) != 0; }
        void write(uv_write_t* req, uv_buf_t bufs[], int bufcnt, uv_write_cb cb) {
            Base_t::_uvRes( uv_write(req, *this, bufs, bufcnt, cb) ); }
        void write2(uv_write_t* req, uv_buf_t bufs[], int bufcnt, uv_stream_t* send_handle, uv_write_cb cb) {
            Base_t::_uvRes( uv_write2(req, *this, bufs, bufcnt, send_handle, cb) ); }

        StreamWrite* writeOp(bool useBufFree) {
            return (new StreamWrite(*this, true))->bindCleanup(useBufFree); }

        void shutdown(uv_shutdown_t* req, uv_shutdown_cb cb) {
            Base_t::_uvRes( uv_shutdown(req, *this, cb) ); }

        ShutdownOp* shutdownOp() { return new ShutdownOp(*this, true); }
        void shutdown() { shutdownOp()->shutdown(); }
        template <typename T>
        void shutdown(T* self) { shutdownOp()->bind(self)->shutdown(); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct TCP : Stream_t< uv_tcp_t, UV_TCP > {
        typedef Stream_t< uv_tcp_t, UV_TCP > Base_t;

        explicit TCP(uv_handle_t* ref) : Base_t(ref) { }
        explicit TCP(uv_stream_t* ref) : Base_t(ref) { }
        TCP(uv_loop_t* loop) : Base_t() { init(loop); }
        TCP() : Base_t() { init(); }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_tcp_init(_as_loop(loop), *this) ); }
        void nodelay(bool enable) {
            Base_t::_uvRes( uv_tcp_nodelay(*this, enable) ); }
        void keepalive(bool enable, unsigned int delay=0) {
            Base_t::_uvRes( uv_tcp_keepalive(*this, enable, delay) ); }
        void simultaneous_accepts(bool enable) {
            Base_t::_uvRes( uv_tcp_simultaneous_accepts(*this, enable) ); }
        void bind(const char* ip, int port, bool ipv6) {
            if (!ipv6) bind(ip,port); else bind6(ip,port); }
        void bind(const char* ip, int port) {
            Base_t::_uvRes( uv_tcp_bind(*this, IP::addr(ip, port)) ); }
        void bind6(const char* ip, int port) {
            Base_t::_uvRes( uv_tcp_bind6(*this, IP::addr6(ip, port)) ); }
        void bind(const struct sockaddr_in& addr) {
            Base_t::_uvRes( uv_tcp_bind(*this, addr) ); }
        void bind6(const struct sockaddr_in6& addr) {
            Base_t::_uvRes( uv_tcp_bind6(*this, addr) ); }
        std::string getsockurl(const char* schema="tcp", const char* path="") {
            return getsockname().url(schema, path); }
        std::string getpeerurl(const char* schema="tcp", const char* path="") {
            return getpeername().url(schema, path); }
        IP getsockname() { IP addr; int len=sizeof(addr.sa);
            uv_tcp_getsockname(*this, &addr.sa.raw, &len);
            return addr; }
        IP getpeername() { IP addr; int len=sizeof(addr.sa);
            uv_tcp_getpeername(*this, &addr.sa.raw, &len);
            return addr; }
        void getsockname(struct sockaddr* name, int* namelen) {
            Base_t::_uvRes( uv_tcp_getsockname(*this, name, namelen) ); }
        void getpeername(struct sockaddr* name, int* namelen) {
            Base_t::_uvRes( uv_tcp_getpeername(*this, name, namelen) ); }

        void connect(uv_connect_t* req, const char* ip, int port, uv_connect_cb cb) {
            Base_t::_uvRes( uv_tcp_connect(req, *this, IP::addr(ip, port), cb) ); }
        void connect6(uv_connect_t* req, const char* ip, int port, uv_connect_cb cb) {
            Base_t::_uvRes( uv_tcp_connect6(req, *this, IP::addr6(ip, port), cb) ); }
        void connect(uv_connect_t* req, const struct sockaddr_in& addr, uv_connect_cb cb) {
            Base_t::_uvRes( uv_tcp_connect(req, *this, addr, cb) ); }
        void connect6(uv_connect_t* req, const struct sockaddr_in6& addr, uv_connect_cb cb) {
            Base_t::_uvRes( uv_tcp_connect6(req, *this, addr, cb) ); }

        TCPConnectOp* connectOp() { return new TCPConnectOp(*this, true); }
        void connect(const char* ip, int port) { connectOp()->connect(ip, port); }
        void connect(const struct sockaddr_in& addr) { connectOp()->connect(addr); }
        void connect6(const char* ip, int port) { connectOp()->connect6(ip, port); }
        void connect6(const struct sockaddr_in6& addr) { connectOp()->connect6(addr); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct Pipe : Stream_t< uv_pipe_t, UV_NAMED_PIPE > {
        typedef Stream_t< uv_pipe_t, UV_NAMED_PIPE > Base_t;
        explicit Pipe(uv_handle_t* ref) : Base_t(ref) { }
        explicit Pipe(uv_stream_t* ref) : Base_t(ref) { }
        Pipe(uv_loop_t* loop, int ipc=0) : Base_t() { init(loop, ipc); }
        Pipe(int ipc=0) : Base_t() { init(ipc); }

        void init(int ipc=0) { init(NULL, ipc); }
        void init(uv_loop_t* loop, int ipc=0) {
            Base_t::_uvRes( uv_pipe_init(_as_loop(loop), *this, ipc) ); }
        void open(uv_file file) {
            uv_pipe_open(*this, file); }
        void bind(const char* name) {
            Base_t::_uvRes( uv_pipe_bind(*this, name) ); }

        void connect(uv_connect_t* req, const char* name, uv_connect_cb cb) {
            uv_pipe_connect(req, *this, name, cb); }
        PipeConnect* connectOp() { return new PipeConnect(*this, true); }
        void connect(const char* name) { connectOp()->connect(name); }

        void pending_instances(int count) {
            uv_pipe_pending_instances(*this, count); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct TTY : Stream_t< uv_tty_t, UV_TTY > {
        typedef Stream_t< uv_tty_t, UV_TTY > Base_t;
        explicit TTY(uv_handle_t* ref) : Base_t(ref) { }
        explicit TTY(uv_stream_t* ref) : Base_t(ref) { }
        TTY(uv_loop_t* loop, uv_file fd, int readable) : Base_t()
        { init(loop, fd, readable); }
        TTY(uv_file fd, int readable) : Base_t()
        { init(fd, readable); }

        void init(uv_file fd, int readable) {
            return init(NULL, fd, readable); }
        void init(uv_loop_t* loop, uv_file fd, int readable) {
            Base_t::_uvRes( uv_tty_init(_as_loop(loop), *this, fd, readable) ); }
        void set_mode(int mode) {
            Base_t::_uvRes( uv_tty_set_mode(*this, mode) ); }
        void reset_mode() { uv_tty_reset_mode(); }
        void get_winsize(int& width, int& height) {
            Base_t::_uvRes( uv_tty_get_winsize(*this, &width, &height) ); }
        static uv_handle_type guess_handle(uv_file file) {
           return uv_guess_handle(file); }
    };
}
