/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

// This header file exists to:
//  * work around issues with openssl defining I in conflict with C std
//  * insulate the Chapel code from fiddly differences between 1.0.2 and 1.1
//  * work around issues with constant pointers in generated C code
//  * work around problems with macro-functions for the LLVM backend

#undef I
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/opensslv.h>
#undef I
#ifdef _Imaginary_I
#define I _Imaginary_I
#else
#define I _Complex_I
#endif
typedef const EVP_MD* CONST_EVP_MD_PTR;
typedef const EVP_CIPHER* CONST_EVP_CIPHER_PTR;

#if OPENSSL_VERSION_NUMBER < 0x1010000fL
// OpenSSL 1.0 used EVP_MD_CTX_init, EVP_MD_CTX_cleanup
// EVP_CIPHER_CTX_init, EVP_CIPHER_CTX_cleanup
typedef EVP_MD_CTX CHPL_EVP_MD_CTX; 
typedef EVP_CIPHER_CTX CHPL_EVP_CIPHER_CTX;
static inline CHPL_EVP_MD_CTX CHPL_EVP_MD_CTX_new(void) {
  EVP_MD_CTX ret;
  EVP_MD_CTX_init(&ret);
  return ret;
}
static inline void CHPL_EVP_MD_CTX_free(CHPL_EVP_MD_CTX* arg) {
  EVP_MD_CTX_cleanup(arg);
}
static inline EVP_MD_CTX* CHPL_EVP_MD_CTX_ptr(CHPL_EVP_MD_CTX* arg) {
  return arg;
}
static inline CHPL_EVP_CIPHER_CTX CHPL_EVP_CIPHER_CTX_new(void) {
  EVP_CIPHER_CTX ret;
  EVP_CIPHER_CTX_init(&ret);
  return ret;
}
static inline void CHPL_EVP_CIPHER_CTX_free(CHPL_EVP_CIPHER_CTX* arg) {
  EVP_CIPHER_CTX_cleanup(arg);
}
static inline EVP_CIPHER_CTX* CHPL_EVP_CIPHER_CTX_ptr(CHPL_EVP_CIPHER_CTX* arg) {
  return arg;
}
#else
// OpenSSL 1.1 uses EVP_MD_CTX_new, EVP_MD_CTX_free
// EVP_CIPHER_CTX_new, EVP_CIPHER_CTX_free
typedef EVP_MD_CTX* CHPL_EVP_MD_CTX;
typedef EVP_CIPHER_CTX* CHPL_EVP_CIPHER_CTX;
static inline CHPL_EVP_MD_CTX CHPL_EVP_MD_CTX_new(void) {
  return EVP_MD_CTX_new();
}
static inline void CHPL_EVP_MD_CTX_free(CHPL_EVP_MD_CTX* arg) {
  EVP_MD_CTX_free(*arg);
  *arg = NULL;
}
static inline EVP_MD_CTX* CHPL_EVP_MD_CTX_ptr(CHPL_EVP_MD_CTX* arg) {
  return *arg;
}
static inline CHPL_EVP_CIPHER_CTX CHPL_EVP_CIPHER_CTX_new(void) {
  return EVP_CIPHER_CTX_new();
}
static inline void CHPL_EVP_CIPHER_CTX_free(CHPL_EVP_CIPHER_CTX* arg) {
  EVP_CIPHER_CTX_free(*arg);
  *arg = NULL;
}
static inline EVP_CIPHER_CTX* CHPL_EVP_CIPHER_CTX_ptr(CHPL_EVP_CIPHER_CTX* arg) {
  return *arg;
}

#endif

static inline void CHPL_OpenSSL_add_all_digests(void) {
  // OpenSSL_add_all_digests is a complex macro in some implementations
  OpenSSL_add_all_digests();
}
