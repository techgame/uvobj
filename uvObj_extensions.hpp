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
    inline int setenv(const char *name, const char *value, int overwrite=1) {
        if (!overwrite && getenv(name)) return 0;
        size_t len = 3+::strlen(name)+::strlen(value);
        char* buf = new char[len];
        ::snprintf(buf, len, "%s=%s", name, value);
        int res = _putenv(buf);
        delete [] buf;
        return res; }
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
    inline int setenv(const char *name, const char *value, int overwrite=1) {
        return ::setenv(name, value, overwrite); }
}
#endif

namespace uvObj {
    inline const char* getenv(const char *name) { return ::getenv(name); }
    inline char** environ() { return _env_default; }
}


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
            while (v_args.back() == NULL) v_args.pop_back();
            /* var-args without the possibility of missing the last NULL */
            if (arg0) v_args.push_back(arg0);
            if (arg1) v_args.push_back(arg1);
            if (arg2) v_args.push_back(arg2);
            if (arg3) v_args.push_back(arg3);
            return *this; }
        ProcessEx& arg(const char* arg) {
            while (v_args.back() == NULL) v_args.pop_back();
            if (arg) v_args.push_back(arg);
            return *this; }

        ProcessEx& clear_stdio() { v_stdio.empty(); return *this; }
        ProcessEx& stdio(uv_stdio_flags flags, uv_stream_t* stream) {
            uv_stdio_container_t c = {flags};
            c.data.stream = stream;
            v_stdio.push_back(c);
            return *this; }
        ProcessEx& stdio(uv_stdio_flags flags, int fd) {
            uv_stdio_container_t c = {flags};
            c.data.fd = fd;
            v_stdio.push_back(c);
            return *this; }
        ProcessEx& stdio(int fd) { return stdio(UV_INHERIT_FD, fd); }
        ProcessEx& stdin() { return stdio(UV_INHERIT_FD, 0); }
        ProcessEx& stdout() { return stdio(UV_INHERIT_FD, 1); }
        ProcessEx& stderr() { return stdio(UV_INHERIT_FD, 2); }
        ProcessEx& stdio_all() { return stdio(UV_INHERIT_FD, 0)
            .stdio(UV_INHERIT_FD, 1).stdio(UV_INHERIT_FD, 2); }

        void spawn(uv_exit_cb cb=NULL) { spawn(NULL, cb); }
        void spawn(uv_loop_t* loop, uv_exit_cb cb=NULL) {
            if (cb) opt.exit_cb = cb;
            if (v_args.back() != NULL)
                v_args.push_back(NULL);
            opt.args = const_cast<char**>(v_args.size() ? &v_args[0] : NULL);
            opt.stdio_count = v_stdio.size();
            opt.stdio = v_stdio.size() ? &v_stdio[0] : NULL;
            uvRes( uv_spawn(_as_loop(loop), *this, opt) ); }
        template <typename T>
        void spawn(T* self, uv_loop_t* loop=NULL) {
            Base_t::setData(self); spawn(loop, T::evt::on_exit); }
        template <typename T>
        void spawn(T* self, uv_exit_cb cb, uv_loop_t* loop=NULL) {
            Base_t::setData(self); spawn(loop, cb); }

        uv_process_options_t opt;
        std::vector<const char*> v_args;
        std::vector<uv_stdio_container_t> v_stdio;
    };
}
