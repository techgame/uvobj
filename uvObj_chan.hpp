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

#if defined(WIN32)
#define snprintf _snprintf
#define _env_default NULL
inline int setenv(const char *name, const char *value, int overwrite) {
    if (!overwrite && getenv(name)) return 0;
    size_t len = 3+::strlen(name)+::strlen(value);
    char* buf = new char[len];
    ::snprintf(buf, len, "%s=%s", name, value);
    int res = _putenv(buf);
    delete [] buf;
    return res; }
#else
extern char **environ;
#define _env_default environ
#endif

namespace uvObj {
    struct IP {
        union {
            struct sockaddr raw;
            struct sockaddr_in src;
            struct sockaddr_in6 src6;
        };

        int port() {
            if (raw.sa_family == PF_INET)
                return ntohs(src.sin_port);
            else if (raw.sa_family == PF_INET6)
                return ntohs(src6.sin6_port);
            else return 0; }
        std::string name() {
            char buf[256] = {0};
            if (raw.sa_family == PF_INET)
                if (0 == uv_ip4_name(&src, buf, sizeof(buf)))
                    return buf;
            else if (raw.sa_family == PF_INET6)
                if (0 == uv_ip6_name(&src6, buf, sizeof(buf)))
                    return buf;
            return ""; }
        std::string url(const char* schema="http", const char* path="") {
            char buf[8192] = {0};
            snprintf(buf, sizeof(buf), "%s://%s:%d%s", schema, name().c_str(), port(), path);
            return buf; }
        std::string format(const char* fmt="%s:%d") {
            char buf[1024] = {0};
            snprintf(buf, sizeof(buf), fmt, name().c_str(), port());
            return buf; }

        void set(const char* ip, int port) {
            src = uv_ip4_addr(ip, port); }
        void set6(const char* ip, int port) {
            src6 = uv_ip6_addr(ip, port); }

        static struct sockaddr_in addr(const char* ip, int port) {
            return uv_ip4_addr(ip, port); }
        static struct sockaddr_in6 addr6(const char* ip, int port) {
            return uv_ip6_addr(ip, port); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct UDP : Handle_t< uv_udp_t, self_t > {
        typedef Handle_t< uv_udp_t, self_t > Base_t;
        UDP(uv_loop_t* loop) : Base_t() { init(loop); }
        UDP() : Base_t() { init(); }

        int init() { return init(NULL); }
        int init(uv_loop_t* loop) {
            return uv_udp_init(_as_loop(loop), *this); }

        int bind(const char* ip, int port, unsigned flags) {
            return uv_udp_bind(*this, IP::addr(ip, port), flags); }
        int bind6(const char* ip, int port, unsigned flags) {
            return uv_udp_bind6(*this, IP::addr6(ip, port), flags); }
        int bind(const struct sockaddr_in& addr, unsigned flags) {
            return uv_udp_bind(*this, addr, flags); }
        int bind6(const struct sockaddr_in6& addr, unsigned flags) {
            return uv_udp_bind6(*this, addr, flags); }
        IP getsockname() { IP addr; int len=sizeof(addr.raw);
            uv_udp_getsockname(*this, &addr.raw, &len);
            return addr; }
        int getsockname(struct sockaddr* name, int* namelen) {
            return uv_udp_getsockname(*this, name, namelen); }

        int set_membership(const char* multicast_addr,
                const char* interface_addr, uv_membership membership) {
            return uv_udp_set_membership(*this, multicast_addr,
                    interface_addr, membership); }
        int set_multicast_loop(int on) {
            return uv_udp_set_multicast_loop(*this, on); }
        int set_multicast_ttl(int ttl) {
            return uv_udp_set_multicast_ttl(*this, ttl); }
        int set_broadcast(int on) {
            return uv_udp_set_broadcast(*this, on); }
        int set_ttl(int ttl) { return uv_udp_set_ttl(*this, ttl); }

        int send(uv_udp_send_t* req, uv_buf_t bufs[], int bufcnt, 
                const char* ip, int port, uv_udp_send_cb cb) {
            return uv_udp_send(req, *this, bufs, bufcnt, IP::addr(ip, port), cb); }
        int send6(uv_udp_send_t* req, uv_buf_t bufs[], int bufcnt,
                const char* ip, int port, uv_udp_send_cb cb) {
            return uv_udp_send6(req, *this, bufs, bufcnt, IP::addr6(ip, port), cb); }
        int send(uv_udp_send_t* req, uv_buf_t bufs[], int bufcnt, 
                const struct sockaddr_in& addr, uv_udp_send_cb cb) {
            return uv_udp_send(req, *this, bufs, bufcnt, addr, cb); }
        int send6(uv_udp_send_t* req, uv_buf_t bufs[], int bufcnt,
                const struct sockaddr_in6& addr, uv_udp_send_cb cb) {
            return uv_udp_send6(req, *this, bufs, bufcnt, addr, cb); }

        int recv_start(uv_alloc_cb alloc_cb, uv_udp_recv_cb recv_cb) {
            return uv_udp_recv_start(*this, alloc_cb, recv_cb); }
        int recv_stop() { return uv_udp_recv_stop(*this); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename uv_t, typename self_t>
    struct Stream_t : Handle_t< uv_t, self_t > {
        typedef Handle_t< uv_t, self_t > Base_t;
        typedef typename Base_t::Evt Evt;

        explicit Stream_t(ref_mode_t m, uv_t* ref) : Base_t(m, ref) {}
        explicit Stream_t(ref_mode_t m, uv_stream_t* ref)
         : Base_t(m, reinterpret_cast<uv_t*>(ref)) {}
        explicit Stream_t(ref_mode_t m, uv_handle_t* ref)
         : Base_t(m, reinterpret_cast<uv_t*>(ref)) {}
        Stream_t() : Base_t() {}

        inline uv_stream_t* asStream() {
            return reinterpret_cast<uv_stream_t*>(Base_t::_ref_); }
        inline operator uv_stream_t*() { return asStream(); }

        int listen(int backlog, uv_connection_cb cb) {
            return uv_listen(*this, backlog, cb); }
        int listen(int backlog) {
            return listen(backlog, Base_t::Evt::on_connection); }
        int accept(uv_stream_t* client) {
            return uv_accept(*this, client); }

        int is_readable() {
            return uv_is_readable(*this); }
        int read_start(uv_alloc_cb alloc_cb, uv_read_cb read_cb) {
            return uv_read_start(*this, alloc_cb, read_cb); }
        int read_start() {
            return read_start(Evt::on_alloc, Evt::on_read); }
        int read2_start(uv_alloc_cb alloc_cb, uv_read2_cb read_cb) {
            return uv_read2_start(*this, alloc_cb, read_cb); }
        int read2_start() {
            return read2_start(Evt::on_alloc, Evt::on_read2); }
        int read_stop() {
            return uv_read_stop(*this); }

        int is_writable() {
            return uv_is_writable(*this); }
        int write(uv_write_t* req, uv_buf_t bufs[], int bufcnt, uv_write_cb cb) {
            return uv_write(req, *this, bufs, bufcnt, cb); }
        int write2(uv_write_t* req, uv_buf_t bufs[], int bufcnt, uv_stream_t* send_handle, uv_write_cb cb) {
            return uv_write2(req, *this, bufs, bufcnt, send_handle, cb); }

        int shutdown(uv_shutdown_t* req, uv_shutdown_cb cb) {
            return uv_shutdown(req, *this, cb); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct TCP : Stream_t< uv_tcp_t, self_t > {
        typedef Stream_t< uv_tcp_t, self_t > Base_t;

        explicit TCP(ref_mode_t m, uv_handle_t* ref) : Base_t(m, ref) { }
        explicit TCP(ref_mode_t m, uv_stream_t* ref) : Base_t(m, ref) { }
        TCP(uv_loop_t* loop) : Base_t() { init(loop); }
        TCP() : Base_t() { init(); }

        int init() { return init(NULL); }
        int init(uv_loop_t* loop) {
            return uv_tcp_init(_as_loop(loop), *this); }
        int nodelay(bool enable) {
            return uv_tcp_nodelay(*this, enable); }
        int keepalive(bool enable, unsigned int delay) {
            return uv_tcp_keepalive(*this, enable, delay); }
        int simultaneous_accepts(bool enable) {
            return uv_tcp_simultaneous_accepts(*this, enable); }
        int bind(const char* ip, int port) {
            return uv_tcp_bind(*this, IP::addr(ip, port)); }
        int bind6(const char* ip, int port) {
            return uv_tcp_bind6(*this, IP::addr6(ip, port)); }
        int bind(const struct sockaddr_in& addr) {
            return uv_tcp_bind(*this, addr); }
        int bind6(const struct sockaddr_in6& addr) {
            return uv_tcp_bind6(*this, addr); }
        IP getsockname() { IP addr; int len=sizeof(addr.raw);
            uv_tcp_getsockname(*this, &addr.raw, &len);
            return addr; }
        IP getpeername() { IP addr; int len=sizeof(addr.raw);
            uv_tcp_getpeername(*this, &addr.raw, &len);
            return addr; }
        int getsockname(struct sockaddr* name, int* namelen) {
            return uv_tcp_getsockname(*this, name, namelen); }
        int getpeername(struct sockaddr* name, int* namelen) {
            return uv_tcp_getpeername(*this, name, namelen); }
        int connect(uv_connect_t* req, const char* ip, int port, uv_connect_cb cb) {
            return uv_tcp_connect(req, *this, IP::addr(ip, port), cb); }
        int connect6(uv_connect_t* req,const char* ip, int port, uv_connect_cb cb) {
            return uv_tcp_connect6(req, *this, IP::addr6(ip, port), cb); }
        int connect(uv_connect_t* req, const struct sockaddr_in& addr, uv_connect_cb cb) {
            return uv_tcp_connect(req, *this, addr, cb); }
        int connect6(uv_connect_t* req, const struct sockaddr_in6& addr, uv_connect_cb cb) {
            return uv_tcp_connect6(req, *this, addr, cb); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct TTY : Stream_t< uv_tty_t, self_t > {
        typedef Stream_t< uv_tty_t, self_t > Base_t;
        explicit TTY(ref_mode_t m, uv_stream_t* ref) : Base_t(m, ref) { }
        TTY(uv_loop_t* loop, uv_file fd, int readable) : Base_t()
        { init(loop, fd, readable); }
        TTY(uv_file fd, int readable) : Base_t()
        { init(fd, readable); }

        int init(uv_file fd, int readable) {
            return init(NULL, fd, readable); }
        int init(uv_loop_t* loop, uv_file fd, int readable) {
            return uv_tty_init(_as_loop(loop), *this, fd, readable); }
        int set_mode(int mode) { return uv_tty_set_mode(*this, mode); }
        void reset_mode() { uv_tty_reset_mode(); }
        int get_winsize(int& width, int& height) {
            return uv_tty_get_winsize(*this, &width, &height); }
        static uv_handle_type guess_handle(uv_file file) {
            return uv_guess_handle(file); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct Process_t : Handle_t< uv_process_t, self_t > {
        typedef Handle_t< uv_process_t, self_t > Base_t;
        int spawn(const uv_process_options_t& options) {
            return spawn(NULL, options); }
        int spawn(uv_loop_t* loop, const uv_process_options_t& options) {
            return uv_spawn(_as_loop(loop), *this, options); }
        int kill(int signum) {
            return uv_process_kill(*this, signum); }
        static uv_err_t kill(int pid, int signum) {
            return uv_kill(pid, signum); }
    };

    template <typename self_t=self_void_t>
    struct Process : Process_t< self_t > {
        typedef Process_t< self_t > Base_t;
        Process() : Base_t() { init(); }
        Process(const char* file, const char* cwd=NULL) : Base_t()
        { init(); target(file, cwd); }

        void init() { ::memset(&opt, 0, sizeof(opt)); env(); }
        
        Process& target(const char* file, const char* cwd=NULL) {
            if (v_args.size()<1) v_args.push_back(file);
            v_args[0] = opt.file = file;
            if (cwd) opt.cwd = (char*)cwd;
            return *this; }

        Process& env(char** env=NULL) {
            opt.env = env ? env : _env_default;
            return *this; }

        Process& clear_args() { v_args.empty(); return *this; }
        Process& args(const char* arg0, const char* arg1=NULL,
                      const char* arg2=NULL, const char* arg3=NULL) {
            while (v_args.back() == NULL) v_args.pop_back();
            /* var-args without the possibility of missing the last NULL */
            if (arg0) v_args.push_back(arg0);
            if (arg1) v_args.push_back(arg1);
            if (arg2) v_args.push_back(arg2);
            if (arg3) v_args.push_back(arg3);
            return *this; }

        Process& clear_stdio() { v_stdio.empty(); return *this; }
        Process& stdio(uv_stdio_flags flags, uv_stream_t* stream) {
            uv_stdio_container_t c = {flags};
            c.data.stream = stream;
            v_stdio.push_back(c);
            return *this; }
        Process& stdio(uv_stdio_flags flags, int fd) {
            uv_stdio_container_t c = {flags};
            c.data.fd = fd;
            v_stdio.push_back(c);
            return *this; }
        Process& stdio(int fd) { return stdio(UV_INHERIT_FD, fd); }
        Process& stdio_all() { return stdio(UV_INHERIT_FD, 0)
            .stdio(UV_INHERIT_FD, 1).stdio(UV_INHERIT_FD, 2); }

        int spawn(uv_exit_cb cb=NULL) { return spawn(NULL, cb); }
        int spawn(uv_loop_t* loop, uv_exit_cb cb=NULL) {
            if (cb) opt.exit_cb = cb;
            if (v_args.back() != NULL)
                v_args.push_back(NULL);
            opt.args = const_cast<char**>(v_args.size() ? &v_args[0] : NULL);
            opt.stdio_count = v_stdio.size();
            opt.stdio = v_stdio.size() ? &v_stdio[0] : NULL;
            return uv_spawn(_as_loop(loop), *this, opt); }

        uv_process_options_t opt;
        std::vector<const char*> v_args;
        std::vector<uv_stdio_container_t> v_stdio;
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    template <typename self_t=self_void_t>
    struct Pipe : Stream_t< uv_pipe_t, self_t > {
        typedef Stream_t< uv_pipe_t, self_t > Base_t;
        explicit Pipe(ref_mode_t m, uv_stream_t* ref) : Base_t(m, ref) { }
        Pipe(uv_loop_t* loop, int ipc=0) : Base_t() { init(loop, ipc); }
        Pipe(int ipc=0) : Base_t() { init(ipc); }

        int init(int ipc=0) { return init(NULL, ipc); }
        int init(uv_loop_t* loop, int ipc=0) {
            return uv_pipe_init(_as_loop(loop), *this, ipc); }
        void open(uv_file file) {
            uv_pipe_open(*this, file); }
        int bind(uv_pipe_t* handle, const char* name) {
            return uv_pipe_bind(*this, name); }
        void connect(uv_connect_t* req, const char* name, uv_connect_cb cb) {
            uv_pipe_connect(req, *this, name, cb); }
        void pending_instances(int count) {
            uv_pipe_pending_instances(*this, count); }
    };
}
