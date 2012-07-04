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
    struct FS_Poll : Handle_t< uv_fs_poll_t > {
        typedef Handle_t< uv_fs_poll_t > Base_t;
        FS_Poll(uv_loop_t* loop) : Base_t() { init(loop); }
        FS_Poll() : Base_t() { init(); }

        void init() { init(NULL); }
        void init(uv_loop_t* loop) {
            Base_t::uvRes( uv_fs_poll_init(_as_loop(loop), *this) ); }
        void start(uv_fs_poll_cb cb, const char* path, unsigned int interval) {
            Base_t::uvRes( uv_fs_poll_start(*this, cb, path, interval) ); }
        template <typename T>
        void start(T* self, const char* path, unsigned int interval) {
            Base_t::setData(self); start(T::evt::on_fs_poll, path, interval); }
        void stop() { Base_t::uvRes( uv_fs_poll_stop(*this) ); }
    };

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    struct FS_Event : Handle_t< uv_fs_event_t > {
        typedef Handle_t< uv_fs_event_t > Base_t;
        void init(const char* filename, uv_fs_event_cb cb, int flags) {
            init(NULL, filename, cb, flags); }
        template <typename T>
        void init(T* self, const char* filename, int flags) {
            Base_t::setData(self); init(NULL, filename, T::evt::on_fs_event, flags); }
        void init(uv_loop_t* loop, const char* filename, uv_fs_event_cb cb, int flags) {
            Base_t::uvRes( uv_fs_event_init(_as_loop(loop), *this, filename, cb, flags) ); }
        template <typename T>
        void init(T* self, uv_loop_t* loop, const char* filename, int flags) {
            Base_t::setData(self); init(loop, filename, T::evt::on_fs_event, flags); }
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

        void open(const char* path, int flags, int mode, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_open(loop(), *this, path, flags, mode, cb) ); }
        template <typename T>
        void open(T* self, const char* path, int flags, int mode) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_open>);
            Base_t::setData(self); open(cb, path, flags, mode); }
        void close(uv_file file, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_close(loop(), *this, file, cb) ); }
        template <typename T>
        void close(T* self, uv_file file) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_close>);
            Base_t::setData(self); close(cb, file); }
        void read(uv_file file, void* buf, size_t length, int64_t offset, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_read(loop(), *this, file, buf, length, offset, cb) ); }
        template <typename T>
        void read(T* self, uv_file file, void* buf, size_t length, int64_t offset) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_read>);
            Base_t::setData(self); read(cb, file, buf, length, offset); }
        void write(uv_file file, void* buf, size_t length, int64_t offset, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_write(loop(), *this, file, buf, length, offset, cb) ); }
        template <typename T>
        void write(T* self, uv_file file, void* buf, size_t length, int64_t offset) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_write>);
            Base_t::setData(self); write(cb, file, buf, length, offset); }
        void sendfile(uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_sendfile(loop(), *this, out_fd, in_fd, in_offset, length, cb) ); }
        template <typename T>
        void sendfile(T* self, uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_sendfile>);
            Base_t::setData(self); sendfile(cb, out_fd, in_fd, in_offset, length); }

        void ftruncate(uv_file file, int64_t offset, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_ftruncate(loop(), *this, file, offset, cb) ); }
        template <typename T>
        void ftruncate(T* self, uv_file file, int64_t offset) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_ftruncate>);
            Base_t::setData(self); ftruncate(cb, file, offset); }
        void fsync(uv_file file, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_fsync(loop(), *this, file, cb) ); }
        template <typename T>
        void fsync(T* self, uv_file file) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_fsync>);
            Base_t::setData(self); fsync(cb, file); }
        void fdatasync(uv_file file, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_fdatasync(loop(), *this, file, cb) ); }
        template <typename T>
        void fdatasync(T* self, uv_file file) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_fdatasync>);
            Base_t::setData(self); fdatasync(cb, file); }

        void stat(const char* path, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_stat(loop(), *this, path, cb) ); }
        template <typename T>
        void stat(T* self, const char* path) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_stat>);
            Base_t::setData(self); stat(cb, path); }
        void fstat(uv_file file, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_fstat(loop(), *this, file, cb) ); }
        template <typename T>
        void fstat(T* self, uv_file file) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_fstat>);
            Base_t::setData(self); fstat(cb, file); }
        void lstat(const char* path, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_lstat(loop(), *this, path, cb) ); }
        template <typename T>
        void lstat(T* self, const char* path) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_lstat>);
            Base_t::setData(self); lstat(cb, path); }
        void utime(const char* path, double atime, double mtime, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_utime(loop(), *this, path, atime, mtime, cb) ); }
        template <typename T>
        void utime(T* self, const char* path, double atime, double mtime) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_utime>);
            Base_t::setData(self); utime(cb, path, atime, mtime); }
        void futime(uv_file file, double atime, double mtime, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_futime(loop(), *this, file, atime, mtime, cb) ); }
        template <typename T>
        void futime(T* self, uv_file file, double atime, double mtime) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_futime>);
            Base_t::setData(self); futime(cb, file, atime, mtime); }

        void chmod(const char* path, int mode, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_chmod(loop(), *this, path, mode, cb) ); }
        template <typename T>
        void chmod(T* self, const char* path, int mode) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_chmod>);
            Base_t::setData(self); chmod(cb, path, mode); }
        void fchmod(uv_file file, int mode, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_fchmod(loop(), *this, file, mode, cb) ); }
        template <typename T>
        void fchmod(T* self, uv_file file, int mode) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_fchmod>);
            Base_t::setData(self); fchmod(cb, file, mode); }
        void chown(const char* path, int uid, int gid, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_chown(loop(), *this, path, uid, gid, cb) ); }
        template <typename T>
        void chown(T* self, const char* path, int uid, int gid) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_chown>);
            Base_t::setData(self); chown(cb, path, uid, gid); }
        void fchown(uv_file file, int uid, int gid, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_fchown(loop(), *this, file, uid, gid, cb) ); }
        template <typename T>
        void fchown(T* self, uv_file file, int uid, int gid) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_fchown>);
            Base_t::setData(self); fchown(cb, file, uid, gid); }

        void rename(const char* path, const char* new_path, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_rename(loop(), *this, path, new_path, cb) ); }
        template <typename T>
        void rename(T* self, const char* path, const char* new_path) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_rename>);
            etData(self); rename(cb, path, new_path); }
        void readdir(const char* path, int flags, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_readdir(loop(), *this, path, flags, cb) ); }
        template <typename T>
        void readdir(T* self, const char* path, int flags) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_readdir>);
            Base_t::setData(self); readdir(cb, path, flags); }
        void mkdir(const char* path, int mode, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_mkdir(loop(), *this, path, mode, cb) ); }
        template <typename T>
        void mkdir(T* self, const char* path, int mode) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_mkdir>);
            etData(self); mkdir(cb, path, mode); }
        void rmdir(const char* path, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_rmdir(loop(), *this, path, cb) ); }
        template <typename T>
        void rmdir(T* self, const char* path) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_rmdir>);
            Base_t::setData(self); rmdir(cb, path); }

        void unlink(const char* path, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_unlink(loop(), *this, path, cb) ); }
        template <typename T>
        void unlink(T* self, const char* path) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_unlink>);
            etData(self); unlink(cb, path); }
        void link(const char* path, const char* new_path, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_link(loop(), *this, path, new_path, cb) ); }
        template <typename T>
        void link(T* self, const char* path, const char* new_path) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_link>);
            Base_t::setData(self); link(cb, path, new_path); }
        void symlink(const char* path, const char* new_path, int flags, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_symlink(loop(), *this, path, new_path, flags, cb) ); }
        template <typename T>
        void symlink(T* self, const char* path, const char* new_path, int flags) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_symlink>);
            etData(self); symlink(cb, path, new_path, flags); }
        void readlink(const char* path, uv_fs_cb cb=NULL) {
            Base_t::uvRes( uv_fs_readlink(loop(), *this, path, cb) ); }
        template <typename T>
        void readlink(T* self, const char* path) {
            uv_fs_cb cb(T::evt::template on_fs<&T::on_fs_readlink>);
            Base_t::setData(self); readlink(cb, path); }

        static uv_err_t chdir(const char* path) {
            return uv_chdir(path); }

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
