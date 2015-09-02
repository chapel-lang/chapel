/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef QIOPLUGIN_POPEN_H_
#define QIOPLUGIN_POPEN_H_

#include "sys_basic.h"
#include "qio.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QIO_FD_FORWARD (-1)
#define QIO_FD_CLOSE (-2)
#define QIO_FD_PIPE (-3)
#define QIO_FD_TO_STDOUT (-4)


qioerr qio_openproc(const char** argv,
                    const char** envp,
                    const char* executable,
                    int* stdin_fd,
                    int* stdout_fd,
                    int* stderr_fd,
                    int64_t *pid_out);

qioerr qio_waitpid(int64_t pid,
                   int blocking, int* done, int* exitcode);

qioerr qio_proc_communicate(
    const int threadsafe,
    qio_channel_t* restrict input,
    qio_channel_t* restrict output,
    qio_channel_t* restrict error);


#ifdef __cplusplus
} // end extern "C"
#endif

#endif



