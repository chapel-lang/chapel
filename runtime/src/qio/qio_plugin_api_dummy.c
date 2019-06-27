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

syserr chpl_qio_writev(void* file, struct iovec* iov, int iovcnt, ssize_t* amtWritten) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
syserr chpl_qio_readv(void* file, struct iovec* iov, int iovcnt, ssize_t* amtRead) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}

syserr chpl_qio_pwritev(void* file, struct iovec* iov, int iovcnt, int64_t offset, ssize_t* amtWritten) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
syserr chpl_qio_preadv(void* file, struct iovec* iov, int iovcnt, int64_t offset, ssize_t* amtRead) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}

syserr chpl_qio_seek(void* file, int64_t amount, int whence, int64_t* offset) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
syserr chpl_qio_filelength(void* file, int64_t* length) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
syserr chpl_qio_getpath(void* file, const char** str, ssize_t* len) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
syserr chpl_qio_fsync(void* file) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
syserr chpl_qio_get_chunk(void* file, int64_t* length) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
syserr chpl_qio_chpl_qio_get_locales_for_region(void* file, int64_t start, int64_t end, const char*** localeNames, int64_t* nLocales) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
syserr chpl_qio_close(void* file) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}

/*syserr chpl_qio_open(void* filesystem, const char* path, ssize_t pathlen, int flags, int64_t mode, int64_t hints, void** file) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}*/
syserr chpl_qio_get_cwd(void* filesystem, const char** path, ssize_t* len) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
syserr chpl_qio_get_fs_type(void* filesystem, int64_t* fsType) {
  QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
}
