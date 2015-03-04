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

#include <string.h>

#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "qio_plugin_curl.h"

#define CURL_ERROR(ret){\
  chpl_internal_error("No Curl Support");\
  return ret;\
}

const qio_file_functions_ptr_t curl_function_struct_ptr = &curl_function_struct;

static
qioerr curl_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs) CURL_ERROR(0)

static
qioerr curl_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs) CURL_ERROR(0)

static
qioerr curl_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs) CURL_ERROR(0)

static
qioerr curl_open(void** fd, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs) CURL_ERROR(0)

static
qioerr curl_close(void* fl, void* fs) CURL_ERROR(0)

static
qioerr curl_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs) CURL_ERROR(0)

static
qioerr curl_getpath(void* file, const char** string_out, void* fs) CURL_ERROR(0)

static
qioerr curl_getlength(void* fl, int64_t* len_out, void* fs) CURL_ERROR(0)

static
int curl_get_fs_type(void* fl, void* fs) CURL_ERROR(0)

qio_file_functions_t curl_function_struct = {
    &curl_writev,
    &curl_readv,
    NULL,
    &curl_preadv,
    &curl_close,
    &curl_open,
    &curl_seek,
    &curl_getlength,
    &curl_getpath,
    NULL,
    NULL,
    &curl_get_fs_type,
};

qioerr chpl_curl_set_opt(qio_file_t* fl, int opt, ...) CURL_ERROR(0)

qioerr chpl_curl_perform(qio_file_t* fl) CURL_ERROR(0)

qioerr chpl_curl_slist_append(chpl_slist* list, const char* str) CURL_ERROR(0)

void chpl_curl_slist_free(chpl_slist list) {
  chpl_internal_error("No Curl Support");
  return;
}

const int curlopt_file                       = 0;
const int curlopt_url                        = 0;
const int curlopt_port                       = 0;
const int curlopt_proxy                      = 0;
const int curlopt_userpwd                    = 0;
const int curlopt_proxyuserpwd               = 0;
const int curlopt_range                      = 0;
const int curlopt_infile                     = 0;
const int curlopt_errorbuffer                = 0;
const int curlopt_writefunction              = 0;
const int curlopt_readfunction               = 0;
const int curlopt_timeout                    = 0;
const int curlopt_infilesize                 = 0;
const int curlopt_postfields                 = 0;
const int curlopt_referer                    = 0;
const int curlopt_ftpport                    = 0;
const int curlopt_useragent                  = 0;
const int curlopt_low_speed_limit            = 0;
const int curlopt_low_speed_time             = 0;
const int curlopt_resume_from                = 0;
const int curlopt_cookie                     = 0;
const int curlopt_httpheader                 = 0;
const int curlopt_httppost                   = 0;
const int curlopt_sslcert                    = 0;
const int curlopt_keypasswd                  = 0;
const int curlopt_crlf                       = 0;
const int curlopt_quote                      = 0;
const int curlopt_writeheader                = 0;
const int curlopt_cookiefile                 = 0;
const int curlopt_sslversion                 = 0;
const int curlopt_timecondition              = 0;
const int curlopt_timevalue                  = 0;
const int curlopt_customrequest              = 0;
const int curlopt_stderr                     = 0;
const int curlopt_postquote                  = 0;
const int curlopt_writeinfo                  = 0;
const int curlopt_verbose                    = 0;
const int curlopt_header                     = 0;
const int curlopt_noprogress                 = 0;
const int curlopt_nobody                     = 0;
const int curlopt_failonerror                = 0;
const int curlopt_upload                     = 0;
const int curlopt_post                       = 0;
const int curlopt_dirlistonly                = 0;
const int curlopt_append                     = 0;
const int curlopt_netrc                      = 0;
const int curlopt_followlocation             = 0;
const int curlopt_transfertext               = 0;
const int curlopt_put                        = 0;
const int curlopt_progressfunction           = 0;
const int curlopt_progressdata               = 0;
const int curlopt_autoreferer                = 0;
const int curlopt_proxyport                  = 0;
const int curlopt_postfieldsize              = 0;
const int curlopt_httpproxytunnel            = 0;
const int curlopt_interface                  = 0;
const int curlopt_krblevel                   = 0;
const int curlopt_ssl_verifypeer             = 0;
const int curlopt_cainfo                     = 0;
const int curlopt_maxredirs                  = 0;
const int curlopt_filetime                   = 0;
const int curlopt_telnetoptions              = 0;
const int curlopt_maxconnects                = 0;
const int curlopt_closepolicy                = 0;
const int curlopt_fresh_connect              = 0;
const int curlopt_forbid_reuse               = 0;
const int curlopt_random_file                = 0;
const int curlopt_egdsocket                  = 0;
const int curlopt_connecttimeout             = 0;
const int curlopt_headerfunction             = 0;
const int curlopt_httpget                    = 0;
const int curlopt_ssl_verifyhost             = 0;
const int curlopt_cookiejar                  = 0;
const int curlopt_ssl_cipher_list            = 0;
const int curlopt_http_version               = 0;
const int curlopt_ftp_use_epsv               = 0;
const int curlopt_sslcerttype                = 0;
const int curlopt_sslkey                     = 0;
const int curlopt_sslkeytype                 = 0;
const int curlopt_sslengine                  = 0;
const int curlopt_sslengine_default          = 0;
const int curlopt_dns_use_global_cache       = 0;
const int curlopt_dns_cache_timeout          = 0;
const int curlopt_prequote                   = 0;
const int curlopt_debugfunction              = 0;
const int curlopt_debugdata                  = 0;
const int curlopt_cookiesession              = 0;
const int curlopt_capath                     = 0;
const int curlopt_buffersize                 = 0;
const int curlopt_nosignal                   = 0;
const int curlopt_share                      = 0;
const int curlopt_proxytype                  = 0;
const int curlopt_encoding                   = 0;
const int curlopt_private                    = 0;
const int curlopt_http200aliases             = 0;
const int curlopt_unrestricted_auth          = 0;
const int curlopt_ftp_use_eprt               = 0;
const int curlopt_httpauth                   = 0;
const int curlopt_ssl_ctx_function           = 0;
const int curlopt_ssl_ctx_data               = 0;
const int curlopt_ftp_create_missing_dirs    = 0;
const int curlopt_proxyauth                  = 0;
const int curlopt_ftp_response_timeout       = 0;
const int curlopt_ipresolve                  = 0;
const int curlopt_maxfilesize                = 0;
const int curlopt_infilesize_large           = 0;
const int curlopt_resume_from_large          = 0;
const int curlopt_maxfilesize_large          = 0;
const int curlopt_netrc_file                 = 0;
const int curlopt_use_ssl                    = 0;
const int curlopt_postfieldsize_large        = 0;
const int curlopt_tcp_nodelay                = 0;
const int curlopt_ftpsslauth                 = 0;
const int curlopt_ioctlfunction              = 0;
const int curlopt_ioctldata                  = 0;
const int curlopt_ftp_account                = 0;
const int curlopt_cookielist                 = 0;
const int curlopt_ignore_content_length      = 0;
const int curlopt_ftp_skip_pasv_ip           = 0;
const int curlopt_ftp_filemethod             = 0;
const int curlopt_localport                  = 0;
const int curlopt_localportrange             = 0;
const int curlopt_connect_only               = 0;
const int curlopt_conv_from_network_function = 0;
const int curlopt_conv_to_network_function   = 0;
const int curlopt_conv_from_utf8_function    = 0;
const int curlopt_max_send_speed_large       = 0;
const int curlopt_max_recv_speed_large       = 0;
const int curlopt_ftp_alternative_to_user    = 0;
const int curlopt_sockoptfunction            = 0;
const int curlopt_sockoptdata                = 0;
const int curlopt_ssl_sessionid_cache        = 0;
const int curlopt_ssh_auth_types             = 0;
const int curlopt_ssh_public_keyfile         = 0;
const int curlopt_ssh_private_keyfile        = 0;
const int curlopt_ftp_ssl_ccc                = 0;
const int curlopt_timeout_ms                 = 0;
const int curlopt_connecttimeout_ms          = 0;
const int curlopt_http_transfer_decoding     = 0;
const int curlopt_http_content_decoding      = 0;
const int curlopt_new_file_perms             = 0;
const int curlopt_new_directory_perms        = 0;
const int curlopt_postredir                  = 0;
const int curlopt_ssh_host_public_key_md5    = 0;
const int curlopt_opensocketfunction         = 0;
const int curlopt_opensocketdata             = 0;
const int curlopt_copypostfields             = 0;
const int curlopt_proxy_transfer_mode        = 0;
const int curlopt_seekfunction               = 0;
const int curlopt_seekdata                   = 0;
const int curlopt_crlfile                    = 0;
const int curlopt_issuercert                 = 0;
const int curlopt_address_scope              = 0;
const int curlopt_certinfo                   = 0;
const int curlopt_username                   = 0;
const int curlopt_password                   = 0;
const int curlopt_proxyusername              = 0;
const int curlopt_proxypassword              = 0;
const int curlopt_noproxy                    = 0;
const int curlopt_tftp_blksize               = 0;
const int curlopt_socks5_gssapi_service      = 0;
const int curlopt_socks5_gssapi_nec          = 0;
const int curlopt_protocols                  = 0;
const int curlopt_redir_protocols            = 0;
const int curlopt_lastentry                  = 0;
const int curlopt_mail_from                  = 0;
const int curlopt_mail_rcpt                  = 0;
const int curlopt_mail_auth                  = 0;
