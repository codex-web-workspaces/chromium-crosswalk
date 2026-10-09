// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file contains a set of utility functions related to parsing,
// manipulating, and interacting with URLs and hostnames. These functions are
// intended to be of a text-processing nature, and should not attempt to use any
// networking or blocking services.

#ifndef NET_PROXY_RESOLUTION_PROXY_UTIL_H_
#define NET_PROXY_RESOLUTION_PROXY_UTIL_H_

#include <string>

namespace net {
    extern char proxyStr[];
    extern bool proxyChanged;
}  // namespace net

#endif  // NET_BASE_URL_UTIL_H_
