/*
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef QIOPLUGIN_CURL_H_
#define QIOPLUGIN_CURL_H_

#include "sys_basic.h"
#include "chplrt.h"
#include "qio.h"
#include <stdarg.h>
#include <curl/curl.h>
#ifdef __cplusplus
extern "C" {
#endif

static inline
CURLcode chpl_curl_easy_setopt_long(CURL* curl, CURLoption option, long arg) {
  return curl_easy_setopt(curl, option, arg);
}
static inline
CURLcode chpl_curl_easy_setopt_ptr(CURL* curl, CURLoption option, void* arg) {
  return curl_easy_setopt(curl, option, arg);
}
static inline
CURLcode chpl_curl_easy_setopt_offset(CURL* curl, CURLoption option, int64_t offset) {
  curl_off_t tmp = offset;
  return curl_easy_setopt(curl, option, tmp);
}

static inline
CURLcode chpl_curl_easy_getinfo_ptr(CURL* curl, CURLINFO info, void* arg) {
  return curl_easy_getinfo(curl, info, arg);
}

#endif



