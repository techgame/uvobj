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

namespace uvObj {
    //uv_fs_event_cb fs_event_cb
    // -- typedef void (*uv_fs_event_cb)(uv_fs_event_t* handle, const char* filename, int events, int status);
    template <typename self_t=void*>
    struct FS_Event : Handle_t< uv_fs_event_t, self_t > {
        typedef Handle_t< uv_fs_event_t, self_t > Base_t;
        int init(const char* filename, uv_fs_event_cb cb, int flags) {
            return init(NULL, filename, cb, flags); }
        int init(uv_loop_t* loop, const char* filename, uv_fs_event_cb cb, int flags) {
            return uv_fs_event_init(_as_loop(loop), *this, filename, cb, flags); } 
    };

    //uv_fs_cb fs_cb
    // -- typedef void (*uv_fs_cb)(uv_fs_t* req);
    template <typename self_t=void*>
    struct FS : Ref_t< uv_fs_t, self_t > {
        typedef Ref_t< uv_fs_t, self_t > Base_t;
        uv_loop_t* _loop;
        FS(uv_loop_t* loop) : Base_t() { init(loop); }
        FS() : Base_t() { init(); }

        void init(uv_loop_t* loop=NULL) { _loop = loop; }
        void cleanup() { uv_fs_req_cleanup(*this); }

        int open(const char* path, int flags, int mode, uv_fs_cb cb) {
            return uv_fs_open(_loop, *this, path, flags, mode, cb); }
        int close(uv_file file, uv_fs_cb cb) {
            return uv_fs_close(_loop, *this, file, cb); }
        int read(uv_file file, void* buf, size_t length, int64_t offset, uv_fs_cb cb) {
            return uv_fs_read(_loop, *this, file, buf, length, offset, cb); }
        int write(uv_file file, void* buf, size_t length, int64_t offset, uv_fs_cb cb) {
            return uv_fs_write(_loop, *this, file, buf, length, offset, cb); }
        int sendfile(uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length, uv_fs_cb cb) {
            return uv_fs_sendfile(_loop, *this, out_fd, in_fd, in_offset, length, cb); }

        int ftruncate(uv_file file, int64_t offset, uv_fs_cb cb) {
            return uv_fs_ftruncate(_loop, *this, file, offset, cb); }
        int fsync(uv_file file, uv_fs_cb cb) {
            return uv_fs_fsync(_loop, *this, file, cb); }
        int fdatasync(uv_file file, uv_fs_cb cb) {
            return uv_fs_fdatasync(_loop, *this, file, cb); }

        int stat(const char* path, uv_fs_cb cb) {
            return uv_fs_stat(_loop, *this, path, cb); }
        int fstat(uv_file file, uv_fs_cb cb) {
            return uv_fs_fstat(_loop, *this, file, cb); }
        int lstat(const char* path, uv_fs_cb cb) {
            return uv_fs_lstat(_loop, *this, path, cb); }
        int utime(const char* path, double atime, double mtime, uv_fs_cb cb) {
            return uv_fs_utime(_loop, *this, path, atime, mtime, cb); }
        int futime(uv_file file, double atime, double mtime, uv_fs_cb cb) {
            return uv_fs_futime(_loop, *this, file, atime, mtime, cb); }

        int chmod(const char* path, int mode, uv_fs_cb cb) {
            return uv_fs_chmod(_loop, *this, path, mode, cb); }
        int fchmod(uv_file file, int mode, uv_fs_cb cb) {
            return uv_fs_fchmod(_loop, *this, file, mode, cb); }
        int chown(const char* path, int uid, int gid, uv_fs_cb cb) {
            return uv_fs_chown(_loop, *this, path, uid, gid, cb); }
        int fchown(uv_file file, int uid, int gid, uv_fs_cb cb) {
            return uv_fs_fchown(_loop, *this, file, uid, gid, cb); }

        int rename(const char* path, const char* new_path, uv_fs_cb cb) {
            return uv_fs_rename(_loop, *this, path, new_path, cb); }
        int readdir(const char* path, int flags, uv_fs_cb cb) {
            return uv_fs_readdir(_loop, *this, path, flags, cb); }
        int mkdir(const char* path, int mode, uv_fs_cb cb) {
            return uv_fs_mkdir(_loop, *this, path, mode, cb); }
        int rmdir(const char* path, uv_fs_cb cb) {
            return uv_fs_rmdir(_loop, *this, path, cb); }

        int unlink(const char* path, uv_fs_cb cb) {
            return uv_fs_unlink(_loop, *this, path, cb); }
        int link(const char* path, const char* new_path, uv_fs_cb cb) {
            return uv_fs_link(_loop, *this, path, new_path, cb); }
        int symlink(const char* path, const char* new_path, int flags, uv_fs_cb cb) {
            return uv_fs_symlink(_loop, *this, path, new_path, flags, cb); }
        int readlink(const char* path, uv_fs_cb cb) {
            return uv_fs_readlink(_loop, *this, path, cb); }

        static uv_err_t chdir(const char* path) { return uv_chdir(path); }

        static std::string cwd(size_t size=65536) {
            char* buf = new char[size];
            buf[0] = 0;
            if (0 != uv_cwd(buf, size).code)
                buf[0] = 0;
            std::string res(buf);
            delete [] buf; buf = NULL;
            return res; }

        static std::string exepath(size_t size=65536) {
            char* buf = new char[size];
            buf[0] = 0;
            if (0 != uv_exepath(buf, &size))
                buf[0] = 0;
            std::string res(buf);
            delete [] buf; buf = NULL;
            return res; }

    };
}
