/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#ifndef _QIO_PLUGIN_API_H_
#define _QIO_PLUGIN_API_H_

#include "qio.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CHPL_RT_UNIT_TEST
  #define QIO_USE_DUMMY_API 1
#else
  #define QIO_USE_DUMMY_API 0
#endif

#define QIO_RETURN_PLUGIN_BODY(sym__, ...) do {                       \
  if (QIO_USE_DUMMY_API) {                                            \
    QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no plugin");                   \
  } else {                                                            \
    CHPL_RT_PRGINFO_DECLARE(CHPL_RT_ROOT_PROGRAM_PLACEHOLDER, sym__); \
    return sym__(__VA_ARGS__);                                        \
  }                                                                   \
} while (0)

//
// TODO: Make the module code callbacks have 'plugin' in their name.
//

// create a plugin channel to attach to the qio channel
static inline
syserr qio_plugin_setup_plugin_channel(void* file, void** plugin_ch,
                                       int64_t start,
                                       int64_t end,
                                       qio_channel_t* qio_ch) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_setup_plugin_channel, file, plugin_ch,
                         start, end, qio_ch);
}

// Reads amt bytes (or more) into the channel buffer
static inline
syserr qio_plugin_read_atleast(void* plugin_ch, int64_t amt) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_read_atleast, plugin_ch, amt);
}

// Writes amt bytes from the channel buffer
static inline
syserr qio_plugin_write(void* plugin_ch, int64_t amt) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_write, plugin_ch, amt);
}

// close the channel
static inline
syserr qio_plugin_channel_close(void* ch) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_channel_close, ch);
}

// get the length of a file
static inline
syserr qio_plugin_filelength(void* file, int64_t* length) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_filelength, file, length);
}

// get the path to a file
static inline
syserr qio_plugin_getpath(void* file, uint8_t** str, int64_t* len) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_getpath, file, str, len);
}

// sync the file to disk
static inline
syserr qio_plugin_fsync(void* file) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_fsync, file);
}

// get the optimal i/o size for the channel
static inline
syserr qio_plugin_get_chunk(void* file, int64_t* length) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_get_chunk, file, length);
}

// get the locales for a region
// localeNamesPtr should be a pointer to an array of char* to set on output
static inline
syserr qio_plugin_get_locales_for_region(void* file, int64_t start,
                                         int64_t end,
                                         void **localeNamesPtr,
                                         int64_t* nLocales) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_get_locales_for_region, file, start, end,
                         localeNamesPtr, nLocales);
}

// close a file
static inline
syserr qio_plugin_file_close(void* file) {
  QIO_RETURN_PLUGIN_BODY(chpl_qio_file_close, file);
}

#ifdef __cplusplus
}
#endif

#endif
