#ifndef QIOPLUGIN_CURL_H_
#define QIOPLUGIN_CURL_H_

#include "sys_basic.h"
#include "qio.h"
#include <stdarg.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct curl_handle curl_handle;
typedef curl_handle* curl_handle_ptr;

extern qio_file_functions_t curl_function_struct;
extern const qio_file_functions_ptr_t curl_function_struct_ptr;

qioerr curl_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs);
qioerr curl_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs);
qioerr curl_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t see_to_offset, ssize_t* num_read_out, void* fs);
qioerr curl_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs);
qioerr curl_open(void** fd, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs);
qioerr curl_close(void* fl, void* fs);
qioerr curl_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs);
qioerr curl_getpath(void* file, const char** string_out, void* fs);
qioerr curl_getlength(void* fl, int64_t* len_out, void* fs);

qioerr chpl_curl_perform(qio_file_t* fl);
qioerr chpl_curl_stream_file(qio_file_t* fl_curl, qio_file_t* fl_local);
qioerr chpl_curl_stream_string(qio_file_t* fl, const char** str);
qioerr chpl_curl_set_opt(qio_file_t* fl, int opt, ...);

// These are used by the user in file.setopt();
extern const int curlopt_file                       ;
extern const int curlopt_url                        ;
extern const int curlopt_port                       ;
extern const int curlopt_proxy                      ;
extern const int curlopt_userpwd                    ;
extern const int curlopt_proxyuserpwd               ;
extern const int curlopt_range                      ;
extern const int curlopt_infile                     ;
extern const int curlopt_errorbuffer                ;
extern const int curlopt_writefunction              ;
extern const int curlopt_readfunction               ;
extern const int curlopt_timeout                    ;
extern const int curlopt_infilesize                 ;
extern const int curlopt_postfields                 ;
extern const int curlopt_referer                    ;
extern const int curlopt_ftpport                    ;
extern const int curlopt_useragent                  ;
extern const int curlopt_low_speed_limit            ;
extern const int curlopt_low_speed_time             ;
extern const int curlopt_resume_from                ;
extern const int curlopt_cookie                     ;
extern const int curlopt_httpheader                 ;
extern const int curlopt_httppost                   ;
extern const int curlopt_sslcert                    ;
extern const int curlopt_keypasswd                  ;
extern const int curlopt_crlf                       ;
extern const int curlopt_quote                      ;
extern const int curlopt_writeheader                ;
extern const int curlopt_cookiefile                 ;
extern const int curlopt_sslversion                 ;
extern const int curlopt_timecondition              ;
extern const int curlopt_timevalue                  ;
extern const int curlopt_customrequest              ;
extern const int curlopt_stderr                     ;
extern const int curlopt_postquote                  ;
extern const int curlopt_writeinfo                  ;
extern const int curlopt_verbose                    ;
extern const int curlopt_header                     ;
extern const int curlopt_noprogress                 ;
extern const int curlopt_nobody                     ;
extern const int curlopt_failonerror                ;
extern const int curlopt_upload                     ;
extern const int curlopt_post                       ;
extern const int curlopt_dirlistonly                ;
extern const int curlopt_append                     ;
extern const int curlopt_netrc                      ;
extern const int curlopt_followlocation             ;
extern const int curlopt_transfertext               ;
extern const int curlopt_put                        ;
extern const int curlopt_progressfunction           ;
extern const int curlopt_progressdata               ;
extern const int curlopt_autoreferer                ;
extern const int curlopt_proxyport                  ;
extern const int curlopt_postfieldsize              ;
extern const int curlopt_httpproxytunnel            ;
extern const int curlopt_interface                  ;
extern const int curlopt_krblevel                   ;
extern const int curlopt_ssl_verifypeer             ;
extern const int curlopt_cainfo                     ;
extern const int curlopt_maxredirs                  ;
extern const int curlopt_filetime                   ;
extern const int curlopt_telnetoptions              ;
extern const int curlopt_maxconnects                ;
extern const int curlopt_closepolicy                ;
extern const int curlopt_fresh_connect              ;
extern const int curlopt_forbid_reuse               ;
extern const int curlopt_random_file                ;
extern const int curlopt_egdsocket                  ;
extern const int curlopt_connecttimeout             ;
extern const int curlopt_headerfunction             ;
extern const int curlopt_httpget                    ;
extern const int curlopt_ssl_verifyhost             ;
extern const int curlopt_cookiejar                  ;
extern const int curlopt_ssl_cipher_list            ;
extern const int curlopt_http_version               ;
extern const int curlopt_ftp_use_epsv               ;
extern const int curlopt_sslcerttype                ;
extern const int curlopt_sslkey                     ;
extern const int curlopt_sslkeytype                 ;
extern const int curlopt_sslengine                  ;
extern const int curlopt_sslengine_default          ;
extern const int curlopt_dns_use_global_cache       ;
extern const int curlopt_dns_cache_timeout          ;
extern const int curlopt_prequote                   ;
extern const int curlopt_debugfunction              ;
extern const int curlopt_debugdata                  ;
extern const int curlopt_cookiesession              ;
extern const int curlopt_capath                     ;
extern const int curlopt_buffersize                 ;
extern const int curlopt_nosignal                   ;
extern const int curlopt_share                      ;
extern const int curlopt_proxytype                  ;
extern const int curlopt_encoding                   ;
extern const int curlopt_private                    ;
extern const int curlopt_http200aliases             ;
extern const int curlopt_unrestricted_auth          ;
extern const int curlopt_ftp_use_eprt               ;
extern const int curlopt_httpauth                   ;
extern const int curlopt_ssl_ctx_function           ;
extern const int curlopt_ssl_ctx_data               ;
extern const int curlopt_ftp_create_missing_dirs    ;
extern const int curlopt_proxyauth                  ;
extern const int curlopt_ftp_response_timeout       ;
extern const int curlopt_ipresolve                  ;
extern const int curlopt_maxfilesize                ;
extern const int curlopt_infilesize_large           ;
extern const int curlopt_resume_from_large          ;
extern const int curlopt_maxfilesize_large          ;
extern const int curlopt_netrc_file                 ;
extern const int curlopt_use_ssl                    ;
extern const int curlopt_postfieldsize_large        ;
extern const int curlopt_tcp_nodelay                ;
extern const int curlopt_ftpsslauth                 ;
extern const int curlopt_ioctlfunction              ;
extern const int curlopt_ioctldata                  ;
extern const int curlopt_ftp_account                ;
extern const int curlopt_cookielist                 ;
extern const int curlopt_ignore_content_length      ;
extern const int curlopt_ftp_skip_pasv_ip           ;
extern const int curlopt_ftp_filemethod             ;
extern const int curlopt_localport                  ;
extern const int curlopt_localportrange             ;
extern const int curlopt_connect_only               ;
extern const int curlopt_conv_from_network_function ;
extern const int curlopt_conv_to_network_function   ;
extern const int curlopt_conv_from_utf8_function    ;
extern const int curlopt_max_send_speed_large       ;
extern const int curlopt_max_recv_speed_large       ;
extern const int curlopt_ftp_alternative_to_user    ;
extern const int curlopt_sockoptfunction            ;
extern const int curlopt_sockoptdata                ;
extern const int curlopt_ssl_sessionid_cache        ;
extern const int curlopt_ssh_auth_types             ;
extern const int curlopt_ssh_public_keyfile         ;
extern const int curlopt_ssh_private_keyfile        ;
extern const int curlopt_ftp_ssl_ccc                ;
extern const int curlopt_timeout_ms                 ;
extern const int curlopt_connecttimeout_ms          ;
extern const int curlopt_http_transfer_decoding     ;
extern const int curlopt_http_content_decoding      ;
extern const int curlopt_new_file_perms             ;
extern const int curlopt_new_directory_perms        ;
extern const int curlopt_postredir                  ;
extern const int curlopt_ssh_host_public_key_md5    ;
extern const int curlopt_opensocketfunction         ;
extern const int curlopt_opensocketdata             ;
extern const int curlopt_copypostfields             ;
extern const int curlopt_proxy_transfer_mode        ;
extern const int curlopt_seekfunction               ;
extern const int curlopt_seekdata                   ;
extern const int curlopt_crlfile                    ;
extern const int curlopt_issuercert                 ;
extern const int curlopt_address_scope              ;
extern const int curlopt_certinfo                   ;
extern const int curlopt_username                   ;
extern const int curlopt_password                   ;
extern const int curlopt_proxyusername              ;
extern const int curlopt_proxypassword              ;
extern const int curlopt_noproxy                    ;
extern const int curlopt_tftp_blksize               ;
extern const int curlopt_socks5_gssapi_service      ;
extern const int curlopt_socks5_gssapi_nec          ;
extern const int curlopt_protocols                  ;
extern const int curlopt_redir_protocols            ;
extern const int curlopt_lastentry                  ;

#ifdef __cplusplus
} // end extern "C"
#endif

#endif



