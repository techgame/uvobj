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
    struct IP {
        union {
            struct sockaddr raw;
            struct sockaddr_in src;
            struct sockaddr_in6 src6;
        } sa;
        IP() { ::memset(&sa, 0, sizeof(sa)); }
        IP(const struct sockaddr& raw) {
            ::memset(&sa, 0, sizeof(sa));
            sa.raw = raw; }
        IP(const struct sockaddr_in& src) {
            ::memset(&sa, 0, sizeof(sa));
            sa.src = src; }
        IP(const struct sockaddr_in6& src6) {
            ::memset(&sa, 0, sizeof(sa));
            sa.src6 = src6; }

        int port() {
            if (sa.raw.sa_family == PF_INET)
                return ntohs(sa.src.sin_port);
            else if (sa.raw.sa_family == PF_INET6)
                return ntohs(sa.src6.sin6_port);
            else return 0; }
        std::string name() {
            char buf[256] = {0};
            if (sa.raw.sa_family == PF_INET)
                if (0 == uv_ip4_name(&sa.src, buf, sizeof(buf)))
                    return buf;
            else if (sa.raw.sa_family == PF_INET6)
                if (0 == uv_ip6_name(&sa.src6, buf, sizeof(buf)))
                    return buf;
            return ""; }
        std::string url(const char* schema=NULL, const char* path="") {
            char buf[8192] = {0};
            if (schema)
                snprintf(buf, sizeof(buf), "%s://%s:%d%s",
                    schema, name().c_str(), port(), path);
            else
                snprintf(buf, sizeof(buf), "%s:%d%s",
                    name().c_str(), port(), path);
            return buf; }
        std::string format(const char* fmt="%s:%d") {
            char buf[1024] = {0};
            snprintf(buf, sizeof(buf), fmt, name().c_str(), port());
            return buf; }

        void set(const char* ip, int port) {
            sa.src = uv_ip4_addr(ip, port); }
        void set6(const char* ip, int port) {
            sa.src6 = uv_ip6_addr(ip, port); }

        static struct sockaddr_in addr(const char* ip, int port) {
            return uv_ip4_addr(ip, port); }
        static struct sockaddr_in addr4(const char* ip, int port) {
            return uv_ip4_addr(ip, port); }
        static struct sockaddr_in6 addr6(const char* ip, int port) {
            return uv_ip6_addr(ip, port); }
    };
}
