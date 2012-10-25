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
    struct FSPoll : Handle_t< uv_fs_poll_t > {
        typedef Handle_t< uv_fs_poll_t > Base_t;
        FSPoll(uv_loop_t* loop) : Base_t() { init(loop); }
        FSPoll() : Base_t() { init(); }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::_uvRes( uv_fs_poll_init(_as_loop(loop), *this) ); }
        void start(uv_fs_poll_cb cb, const char* path, unsigned int interval) {
            Base_t::_uvRes( uv_fs_poll_start(*this, cb, path, interval) ); }
        void start(const BoundEvt<uv_fs_poll_cb>& evt, const char* path, unsigned int interval) {
            Base_t::setData(evt.tgt); start(evt.cb, path, interval); }
        void stop() { Base_t::_uvRes( uv_fs_poll_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct FSEvent : Handle_t< uv_fs_event_t > {
        typedef Handle_t< uv_fs_event_t > Base_t;
        void init(const char* filename, uv_fs_event_cb cb, int flags) {
            init(NULL, filename, cb, flags); }
        void init(const BoundEvt<uv_fs_event_cb>& evt, const char* filename, int flags) {
            Base_t::setData(evt.tgt); init(NULL, filename, evt.cb, flags); }
        void init(uv_loop_t* loop, const char* filename, uv_fs_event_cb cb, int flags) {
            Base_t::_uvRes( uv_fs_event_init(_as_loop(loop), *this, filename, cb, flags) ); }
        void init(const BoundEvt<uv_fs_event_cb>& evt, uv_loop_t* loop, const char* filename, int flags) {
            Base_t::setData(evt.tgt); init(loop, filename, evt.cb, flags); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct FS : Ref_t< uv_fs_t > {
        typedef Ref_t< uv_fs_t > Base_t;
        uv_loop_t* _loop;
        FS(uv_loop_t* loop) : Base_t() { init(loop); }
        FS() : Base_t() { init(); }

        void init(uv_loop_t* loop=NULL) { _loop = loop; }
        void cleanup() { uv_fs_req_cleanup(*this); }
        uv_loop_t* loop() { return _loop ? _loop : uv_default_loop(); }

        typedef BoundEvt<uv_fs_cb> BoundFSEvt;

        uv_fs_cb _bind_cb(const BoundFSEvt& evt) {
            Base_t::setData(evt.tgt); return evt.cb; }

        int open(const char* path, int flags, int mode, uv_fs_cb cb=NULL) {
            return uv_fs_open(loop(), *this, path, flags, mode, cb); }
        void open(const BoundFSEvt& evt, const char* path, int flags, int mode) {
            open(path, flags, mode, _bind_cb(evt)); }

        int close(uv_file file, uv_fs_cb cb=NULL) {
            return uv_fs_close(loop(), *this, file, cb); }
        void close(const BoundFSEvt evt, uv_file file) {
            close(file, _bind_cb(evt)); }

        int read(uv_file file, void* buf, size_t length, int64_t offset, uv_fs_cb cb=NULL) {
            return uv_fs_read(loop(), *this, file, buf, length, offset, cb); }
        void read(const BoundFSEvt evt, uv_file file, void* buf, size_t length, int64_t offset) {
            read(file, buf, length, offset, _bind_cb(evt)); }

        int write(uv_file file, void* buf, size_t length, int64_t offset, uv_fs_cb cb=NULL) {
            return uv_fs_write(loop(), *this, file, buf, length, offset, cb); }
        void write(const BoundFSEvt evt, uv_file file, void* buf, size_t length, int64_t offset) {
            write(file, buf, length, offset, _bind_cb(evt)); }

        int sendfile(uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length, uv_fs_cb cb=NULL) {
            return uv_fs_sendfile(loop(), *this, out_fd, in_fd, in_offset, length, cb); }
        void sendfile(const BoundFSEvt evt, uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length) {
            sendfile(out_fd, in_fd, in_offset, length, _bind_cb(evt)); }

        int ftruncate(uv_file file, int64_t offset, uv_fs_cb cb=NULL) {
            return uv_fs_ftruncate(loop(), *this, file, offset, cb); }
        void ftruncate(const BoundFSEvt evt, uv_file file, int64_t offset) {
            ftruncate(file, offset, _bind_cb(evt)); }

        int fsync(uv_file file, uv_fs_cb cb=NULL) {
            return uv_fs_fsync(loop(), *this, file, cb); }
        void fsync(const BoundFSEvt evt, uv_file file) {
            fsync(file, _bind_cb(evt)); }

        int fdatasync(uv_file file, uv_fs_cb cb=NULL) {
            return uv_fs_fdatasync(loop(), *this, file, cb); }
        void fdatasync(const BoundFSEvt evt, uv_file file) {
            fdatasync(file, _bind_cb(evt)); }

        int stat(const char* path, uv_fs_cb cb=NULL) {
            return uv_fs_stat(loop(), *this, path, cb); }
        void stat(const BoundFSEvt evt, const char* path) {
            stat(path, _bind_cb(evt)); }

        int fstat(uv_file file, uv_fs_cb cb=NULL) {
            return uv_fs_fstat(loop(), *this, file, cb); }
        void fstat(const BoundFSEvt evt, uv_file file) {
            fstat(file, _bind_cb(evt)); }

        int lstat(const char* path, uv_fs_cb cb=NULL) {
            return uv_fs_lstat(loop(), *this, path, cb); }
        void lstat(const BoundFSEvt evt, const char* path) {
            lstat(path, _bind_cb(evt)); }

        int utime(const char* path, double atime, double mtime, uv_fs_cb cb=NULL) {
            return uv_fs_utime(loop(), *this, path, atime, mtime, cb); }
        void utime(const BoundFSEvt evt, const char* path, double atime, double mtime) {
            utime(path, atime, mtime, _bind_cb(evt)); }

        int futime(uv_file file, double atime, double mtime, uv_fs_cb cb=NULL) {
            return uv_fs_futime(loop(), *this, file, atime, mtime, cb); }
        void futime(const BoundFSEvt evt, uv_file file, double atime, double mtime) {
            futime(file, atime, mtime, _bind_cb(evt)); }

        int chmod(const char* path, int mode, uv_fs_cb cb=NULL) {
            return uv_fs_chmod(loop(), *this, path, mode, cb); }
        void chmod(const BoundFSEvt evt, const char* path, int mode) {
            chmod(path, mode, _bind_cb(evt)); }

        int fchmod(uv_file file, int mode, uv_fs_cb cb=NULL) {
            return uv_fs_fchmod(loop(), *this, file, mode, cb); }
        void fchmod(const BoundFSEvt evt, uv_file file, int mode) {
            fchmod(file, mode, _bind_cb(evt)); }

        int chown(const char* path, int uid, int gid, uv_fs_cb cb=NULL) {
            return uv_fs_chown(loop(), *this, path, uid, gid, cb); }
        void chown(const BoundFSEvt evt, const char* path, int uid, int gid) {
            chown(path, uid, gid, _bind_cb(evt)); }

        int fchown(uv_file file, int uid, int gid, uv_fs_cb cb=NULL) {
            return uv_fs_fchown(loop(), *this, file, uid, gid, cb); }
        void fchown(const BoundFSEvt evt, uv_file file, int uid, int gid) {
            fchown(file, uid, gid, _bind_cb(evt)); }

        int rename(const char* path, const char* new_path, uv_fs_cb cb=NULL) {
            return uv_fs_rename(loop(), *this, path, new_path, cb); }
        void rename(const BoundFSEvt evt, const char* path, const char* new_path) {
            rename(path, new_path, _bind_cb(evt)); }

        int readdir(const char* path, int flags, uv_fs_cb cb=NULL) {
            return uv_fs_readdir(loop(), *this, path, flags, cb); }
        void readdir(const BoundFSEvt evt, const char* path, int flags) {
            readdir(path, flags, _bind_cb(evt)); }

        int mkdir(const char* path, int mode, uv_fs_cb cb=NULL) {
            return uv_fs_mkdir(loop(), *this, path, mode, cb); }
        void mkdir(const BoundFSEvt evt, const char* path, int mode) {
            mkdir(path, mode, _bind_cb(evt)); }

        int rmdir(const char* path, uv_fs_cb cb=NULL) {
            return uv_fs_rmdir(loop(), *this, path, cb); }
        void rmdir(const BoundFSEvt evt, const char* path) {
            rmdir(path, _bind_cb(evt)); }

        int unlink(const char* path, uv_fs_cb cb=NULL) {
            return uv_fs_unlink(loop(), *this, path, cb); }
        void unlink(const BoundFSEvt evt, const char* path) {
            unlink(path, _bind_cb(evt)); }

        int link(const char* path, const char* new_path, uv_fs_cb cb=NULL) {
            return uv_fs_link(loop(), *this, path, new_path, cb); }
        void link(const BoundFSEvt evt, const char* path, const char* new_path) {
            link(path, new_path, _bind_cb(evt)); }

        int symlink(const char* path, const char* new_path, int flags, uv_fs_cb cb=NULL) {
            return uv_fs_symlink(loop(), *this, path, new_path, flags, cb); }
        void symlink(const BoundFSEvt evt, const char* path, const char* new_path, int flags) {
            symlink(path, new_path, flags, _bind_cb(evt)); }

        int readlink(const char* path, uv_fs_cb cb=NULL) {
            return uv_fs_readlink(loop(), *this, path, cb); }
        void readlink(const BoundFSEvt evt, const char* path) {
            readlink(path, _bind_cb(evt)); }

        static uv_err_t chdir(const char* path) {
            return uv_chdir(path); }

        static std::string cwd(size_t size=16384) {
            char* buf = (char*)::malloc(size);
            buf[0] = 0;
            if (0 != uv_cwd(buf, size).code)
                buf[0] = 0;
            std::string res(buf);
            ::free(buf); buf = NULL;
            return res; }

        static std::string exepath(size_t size=16384) {
            char* buf = (char*)::malloc(size);
            buf[0] = 0;
            if (0 != uv_exepath(buf, &size))
                buf[0] = 0;
            std::string res(buf);
            ::free(buf); buf = NULL;
            return res; }

    };
}
