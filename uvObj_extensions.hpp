/* -*- coding: utf-8 -*- vim: set ts=4 sw=4 expandtab */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##
##~ Copyright (C) 2002-2012  TechGame Networks, LLC.              ##
##~                                                               ##
##~ This library is free software; you can redistribute it        ##
##~ and/or modify it under the terms of the MIT style License as  ##
##~ found in the LICENSE file included with this distribution.    ##
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma once
#include "./uvObj_channels.hpp"


/* Scoped aliases for getenv / setenv system operations */

#if defined(WIN32)
    #define _env_default NULL

namespace uvObj {
    inline char** environ() { return _env_default; }
    inline const char* getenv(const char *name) { return ::getenv(name); }
    inline int setenv(const char *name, const char *value, int overwrite=1) {
        if (!overwrite && getenv(name)) return 0;
        return ::SetEnvironmentVariableA(name, value) ? 0 : -1; }
    struct environ_walk {
        LPTCH root;
        const char* _tip;
        environ_walk() { root = ::GetEnvironmentStringsA(); _tip = root; }
        ~environ_walk() { ::FreeEnvironmentStringsA(root); };
        bool operator !() { return !_tip; }
        operator bool() { return !!_tip; }
        void reset() { _tip = root; }
        const char* tip() { return _tip; }
        const char* next() {
            if (!_tip) return NULL;
            const char* res = _tip;
            _tip += 1+::strlen(_tip);
            if (!_tip[0]) _tip = NULL;
            return res; }
    };
}
#else

    #if defined(__APPLE__) && !TARGET_OS_IPHONE
        #include <crt_externs.h>
        #define _env_default (*_NSGetEnviron())
    #elif defined(__APPLE__) && TARGET_OS_IPHONE
        #define _env_default NULL
    #else
        extern char** environ;
        #define _env_default environ
    #endif

namespace uvObj {
    inline char** environ() { return _env_default; }
    inline const char* getenv(const char *name) { return ::getenv(name); }
    inline int setenv(const char *name, const char *value, int overwrite=1) {
        return ::setenv(name, value, overwrite); }

    struct environ_walk {
        char** root;
        environ_walk() { root = uvObj::environ(); }
        bool operator !() { return !root || !root[0]; }
        operator bool() { return root && root[0]; }
        void reset() { root = uvObj::environ(); }
        const char* tip() { return root ? root[0] : NULL; }
        const char* next() { return root ? root++[0] : NULL; }
    };
}
#endif

namespace uvObj {
    /* Extended Processes object with convenience methods */

    struct ProcessEx : Process {
        ProcessEx() : Process() { init(); }
        ProcessEx(const char* file, const char* cwd=NULL) : Process()
        { init(); target(file, cwd); }

        void init() { ::memset(&opt, 0, sizeof(opt)); env(); }

        ProcessEx& target(const char* file, const char* cwd=NULL) {
            if (v_args.size()<1) v_args.push_back(file);
            v_args[0] = opt.file = file;
            if (cwd) opt.cwd = (char*)cwd;
            return *this; }

        ProcessEx& env(char** env=NULL) {
            opt.env = env ? env : _env_default;
            return *this; }

        ProcessEx& clear_args() { v_args.empty(); return *this; }
        ProcessEx& args(const char* arg0, const char* arg1=NULL,
                      const char* arg2=NULL, const char* arg3=NULL) {
            /* var-args without the possibility of missing the last NULL */
            if (arg0) v_args.push_back(arg0);
            if (arg1) v_args.push_back(arg1);
            if (arg2) v_args.push_back(arg2);
            if (arg3) v_args.push_back(arg3);
            return *this; }
        ProcessEx& arg(const std::string& arg) {
            v_args.push_back(arg);
            return *this; }
        ProcessEx& arg(const char* arg) {
            if (arg) v_args.push_back(arg);
            return *this; }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        ProcessEx& clear_stdio() { v_stdio.empty(); return *this; }
        ProcessEx& add_stdio(uv_stdio_flags flags, uv_stream_t* stream) {
            uv_stdio_container_t c = {flags};
            c.data.stream = stream;
            v_stdio.push_back(c);
            return *this; }
        ProcessEx& add_stdio(uv_stdio_flags flags, int fd) {
            uv_stdio_container_t c = {flags};
            c.data.fd = fd;
            v_stdio.push_back(c);
            return *this; }
        ProcessEx& add_ignore(int fd=-1) { return add_stdio(UV_IGNORE, fd); }
        ProcessEx& add_pipe(uv_pipe_t* pipe, bool readable, bool writeable) {
            int f = UV_CREATE_PIPE | (readable?UV_READABLE_PIPE:0) | (writeable?UV_WRITABLE_PIPE:0);
            return add_stdio((uv_stdio_flags)f, (uv_stream_t*)pipe); }
        ProcessEx& add_rpipe(uv_pipe_t* pipe) { return add_pipe(pipe, true, false); }
        ProcessEx& add_wpipe(uv_pipe_t* pipe) { return add_pipe(pipe, false, true); }
        ProcessEx& add_rwpipe(uv_pipe_t* pipe) { return add_pipe(pipe, true, true); }

        ProcessEx& inherit_stream(uv_stream_t* stream) {
            return add_stdio(UV_INHERIT_STREAM, stream); }
        ProcessEx& inherit_stdio(int fd) {
            return add_stdio(UV_INHERIT_FD, fd); }
        ProcessEx& inherit_stdin(bool inherit=true) {
            return inherit ? inherit_stdio(0) : add_ignore(); }
        ProcessEx& inherit_stdout(bool inherit=true) {
            return inherit ? inherit_stdio(1) : add_ignore(); }
        ProcessEx& inherit_stderr(bool inherit=true) {
            return inherit ? inherit_stdio(2) : add_ignore(); }
        ProcessEx& inherit_all() { return inherit_stdin().inherit_stdout().inherit_stderr(); }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        void setUserId(uv_uid_t uid) {
            if (uid) opt.flags |= UV_PROCESS_SETUID;
            else opt.flags &= ~UV_PROCESS_SETUID;
            opt.uid = uid; }
        void setGroupId(uv_gid_t gid) {
            if (gid) opt.flags |= UV_PROCESS_SETGID;
            else opt.flags &= ~UV_PROCESS_SETGID;
            opt.gid = gid; }

        bool useVerbatimArguments() {
            return !!(opt.flags & UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS); }
        void setVerbatimArguments(bool verbatim=true) {
            if (verbatim)
                opt.flags |= UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS;
            else opt.flags &= ~UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS; }

        bool isDetached() { return !!(opt.flags & UV_PROCESS_DETACHED); }
        void setDetached(bool detached=true) {
            if (detached)
                opt.flags |= UV_PROCESS_DETACHED;
            else opt.flags &= ~UV_PROCESS_DETACHED; }

        void setHideWindow(bool hide=true) {
            if (hide)
                opt.flags |= UV_PROCESS_WINDOWS_HIDE;
            else opt.flags &= ~UV_PROCESS_WINDOWS_HIDE; }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        void spawn(uv_exit_cb cb=NULL) { spawn(NULL, cb); }
        void spawn(uv_loop_t* loop, uv_exit_cb cb=NULL) {
            if (cb) opt.exit_cb = cb;
            std::vector<const char*> args;
            {
                std::vector<std::string>::const_iterator
                    iter=v_args.begin(),end=v_args.end();
                for (;iter!=end;++iter)
                    args.push_back(iter->c_str());
                args.push_back(NULL);
            }
            opt.args = const_cast<char**>(args.size() ? &args[0] : NULL);
            opt.stdio_count = (int) v_stdio.size();
            opt.stdio = v_stdio.size() ? &v_stdio[0] : NULL;
            _uvRes( uv_spawn(_as_loop(loop), *this, opt) ); }
        void spawn(const BoundEvt<uv_exit_cb>& evt, uv_loop_t* loop=NULL) {
            Base_t::setData(evt.tgt); spawn(loop, evt.cb); }

        uv_process_options_t opt;
        std::vector<std::string> v_args;
        std::vector<uv_stdio_container_t> v_stdio;
    };
}
