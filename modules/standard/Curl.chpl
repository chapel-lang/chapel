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

/* Libcurl support 
 */
module Curl {
/***************************** Libcurl support *****************/
// This corresponds to struct curl_slist* when we have curl enabled
extern type chpl_slist;

extern proc chpl_curl_set_opt(fl:qio_file_ptr_t, opt:c_int, arg...):syserr;
extern proc chpl_curl_perform(fl:qio_file_ptr_t):syserr;
extern proc chpl_curl_slist_append(ref list:chpl_slist, str:c_string):syserr;
extern proc chpl_curl_slist_free(list:chpl_slist);

extern const CHPL_CURL_SLIST_NULL:chpl_slist;

// arg: opt -> the curl option to set
// arg: arg -> the value to set the curl option specified by @opt.
// Brief: This function is the equivalent to the curl_easy_setopt function in
//        libcurl.
proc file.setopt(opt:c_int, arg):bool {
  var err:syserr = ENOERR;

  if (arg.type == slist) && (slist.home != this.home) {
    ioerror(EFAULT:syserr, "in file.setopt(): slist, and curl handle do not reside on the same locale");
  }

  on this.home {
    err = chpl_curl_set_opt(this._file_internal, opt, arg);
  }

  if err then ioerror(err, "in file.setopt(opt:c_int, arg)");
  return true;
}

// args: Takes any number of tuples of the form (curl_option, value) and for each
//       curl_option, sets it to that value.
// Brief: This function is equivalent to the curl_setopt_array you might find in PHP.
proc file.setopt(args ...?k) {
  for param i in 1..k {
    this.setopt(args(i)(1), args(i)(2));
  }
}

// Brief: This calls curl_easy_perform on the underlying CURL handle
proc file.perform():bool {
  var err:syserr = ENOERR;

  on this.home {
    err = chpl_curl_perform(this._file_internal);
  }

  if err then ioerror(err, "in file.peform()");
  return true;
}

// Note: list is not ref counted. Therefore the user is responsible for freeing
// the slist when they are done with it.
record slist {
  var home: locale = here;
  // Note: If we do not set the default value of this to NULL, we can get
  // non-deterministic segfaults from libcurl.
  var list: chpl_slist = CHPL_CURL_SLIST_NULL;
}

// arg: str -> string to append to the slist
// Brief: This replicates the semantics of curl_slist_append
proc slist.append(str:string) {
  var err: syserr = ENOERR;
  on this.home {
    err = chpl_curl_slist_append(this.list, str.c_str());
  }
  if err then ioerror(err, "in slist.append()");
}

proc slist.free() {
  on this.home {
    chpl_curl_slist_free(this.list);
  }
}

// These are meant to be ued with the file.setopt() function. This way, a user has
// access to the easy interface.
extern const curlopt_file                       : c_int ;
extern const curlopt_url                        : c_int ;
extern const curlopt_port                       : c_int ;
extern const curlopt_proxy                      : c_int ;
extern const curlopt_userpwd                    : c_int ;
extern const curlopt_proxyuserpwd               : c_int ;
extern const curlopt_range                      : c_int ;
extern const curlopt_infile                     : c_int ;
extern const curlopt_errorbuffer                : c_int ;
extern const curlopt_writefunction              : c_int ;
extern const curlopt_readfunction               : c_int ;
extern const curlopt_timeout                    : c_int ;
extern const curlopt_infilesize                 : c_int ;
extern const curlopt_postfields                 : c_int ;
extern const curlopt_referer                    : c_int ;
extern const curlopt_ftpport                    : c_int ;
extern const curlopt_useragent                  : c_int ;
extern const curlopt_low_speed_limit            : c_int ;
extern const curlopt_low_speed_time             : c_int ;
extern const curlopt_resume_from                : c_int ;
extern const curlopt_cookie                     : c_int ;
extern const curlopt_httpheader                 : c_int ;
extern const curlopt_httppost                   : c_int ;
extern const curlopt_sslcert                    : c_int ;
extern const curlopt_keypasswd                  : c_int ;
extern const curlopt_crlf                       : c_int ;
extern const curlopt_quote                      : c_int ;
extern const curlopt_writeheader                : c_int ;
extern const curlopt_cookiefile                 : c_int ;
extern const curlopt_sslversion                 : c_int ;
extern const curlopt_timecondition              : c_int ;
extern const curlopt_timevalue                  : c_int ;
extern const curlopt_customrequest              : c_int ;
extern const curlopt_stderr                     : c_int ;
extern const curlopt_postquote                  : c_int ;
extern const curlopt_writeinfo                  : c_int ;
extern const curlopt_verbose                    : c_int ;
extern const curlopt_header                     : c_int ;
extern const curlopt_noprogress                 : c_int ;
extern const curlopt_nobody                     : c_int ;
extern const curlopt_failonerror                : c_int ;
extern const curlopt_upload                     : c_int ;
extern const curlopt_post                       : c_int ;
extern const curlopt_dirlistonly                : c_int ;
extern const curlopt_append                     : c_int ;
extern const curlopt_netrc                      : c_int ;
extern const curlopt_followlocation             : c_int ;
extern const curlopt_transfertext               : c_int ;
extern const curlopt_put                        : c_int ;
extern const curlopt_progressfunction           : c_int ;
extern const curlopt_progressdata               : c_int ;
extern const curlopt_autoreferer                : c_int ;
extern const curlopt_proxyport                  : c_int ;
extern const curlopt_postfieldsize              : c_int ;
extern const curlopt_httpproxytunnel            : c_int ;
extern const curlopt_interface                  : c_int ;
extern const curlopt_krblevel                   : c_int ;
extern const curlopt_ssl_verifypeer             : c_int ;
extern const curlopt_cainfo                     : c_int ;
extern const curlopt_maxredirs                  : c_int ;
extern const curlopt_filetime                   : c_int ;
extern const curlopt_telnetoptions              : c_int ;
extern const curlopt_maxconnects                : c_int ;
extern const curlopt_closepolicy                : c_int ;
extern const curlopt_fresh_connect              : c_int ;
extern const curlopt_forbid_reuse               : c_int ;
extern const curlopt_random_file                : c_int ;
extern const curlopt_egdsocket                  : c_int ;
extern const curlopt_connecttimeout             : c_int ;
extern const curlopt_headerfunction             : c_int ;
extern const curlopt_httpget                    : c_int ;
extern const curlopt_ssl_verifyhost             : c_int ;
extern const curlopt_cookiejar                  : c_int ;
extern const curlopt_ssl_cipher_list            : c_int ;
extern const curlopt_http_version               : c_int ;
extern const curlopt_ftp_use_epsv               : c_int ;
extern const curlopt_sslcerttype                : c_int ;
extern const curlopt_sslkey                     : c_int ;
extern const curlopt_sslkeytype                 : c_int ;
extern const curlopt_sslengine                  : c_int ;
extern const curlopt_sslengine_default          : c_int ;
extern const curlopt_dns_use_global_cache       : c_int ;
extern const curlopt_dns_cache_timeout          : c_int ;
extern const curlopt_prequote                   : c_int ;
extern const curlopt_debugfunction              : c_int ;
extern const curlopt_debugdata                  : c_int ;
extern const curlopt_cookiesession              : c_int ;
extern const curlopt_capath                     : c_int ;
extern const curlopt_buffersize                 : c_int ;
extern const curlopt_nosignal                   : c_int ;
extern const curlopt_share                      : c_int ;
extern const curlopt_proxytype                  : c_int ;
extern const curlopt_encoding                   : c_int ;
extern const curlopt_private                    : c_int ;
extern const curlopt_http200aliases             : c_int ;
extern const curlopt_unrestricted_auth          : c_int ;
extern const curlopt_ftp_use_eprt               : c_int ;
extern const curlopt_httpauth                   : c_int ;
extern const curlopt_ssl_ctx_function           : c_int ;
extern const curlopt_ssl_ctx_data               : c_int ;
extern const curlopt_ftp_create_missing_dirs    : c_int ;
extern const curlopt_proxyauth                  : c_int ;
extern const curlopt_ftp_response_timeout       : c_int ;
extern const curlopt_ipresolve                  : c_int ;
extern const curlopt_maxfilesize                : c_int ;
extern const curlopt_infilesize_large           : c_int ;
extern const curlopt_resume_from_large          : c_int ;
extern const curlopt_maxfilesize_large          : c_int ;
extern const curlopt_netrc_file                 : c_int ;
extern const curlopt_use_ssl                    : c_int ;
extern const curlopt_postfieldsize_large        : c_int ;
extern const curlopt_tcp_nodelay                : c_int ;
extern const curlopt_ftpsslauth                 : c_int ;
extern const curlopt_ioctlfunction              : c_int ;
extern const curlopt_ioctldata                  : c_int ;
extern const curlopt_ftp_account                : c_int ;
extern const curlopt_cookielist                 : c_int ;
extern const curlopt_ignore_content_length      : c_int ;
extern const curlopt_ftp_skip_pasv_ip           : c_int ;
extern const curlopt_ftp_filemethod             : c_int ;
extern const curlopt_localport                  : c_int ;
extern const curlopt_localportrange             : c_int ;
extern const curlopt_connect_only               : c_int ;
extern const curlopt_conv_from_network_function : c_int ;
extern const curlopt_conv_to_network_function   : c_int ;
extern const curlopt_conv_from_utf8_function    : c_int ;
extern const curlopt_max_send_speed_large       : c_int ;
extern const curlopt_max_recv_speed_large       : c_int ;
extern const curlopt_ftp_alternative_to_user    : c_int ;
extern const curlopt_sockoptfunction            : c_int ;
extern const curlopt_sockoptdata                : c_int ;
extern const curlopt_ssl_sessionid_cache        : c_int ;
extern const curlopt_ssh_auth_types             : c_int ;
extern const curlopt_ssh_public_keyfile         : c_int ;
extern const curlopt_ssh_private_keyfile        : c_int ;
extern const curlopt_ftp_ssl_ccc                : c_int ;
extern const curlopt_timeout_ms                 : c_int ;
extern const curlopt_connecttimeout_ms          : c_int ;
extern const curlopt_http_transfer_decoding     : c_int ;
extern const curlopt_http_content_decoding      : c_int ;
extern const curlopt_new_file_perms             : c_int ;
extern const curlopt_new_directory_perms        : c_int ;
extern const curlopt_postredir                  : c_int ;
extern const curlopt_ssh_host_public_key_md5    : c_int ;
extern const curlopt_opensocketfunction         : c_int ;
extern const curlopt_opensocketdata             : c_int ;
extern const curlopt_copypostfields             : c_int ;
extern const curlopt_proxy_transfer_mode        : c_int ;
extern const curlopt_seekfunction               : c_int ;
extern const curlopt_seekdata                   : c_int ;
extern const curlopt_crlfile                    : c_int ;
extern const curlopt_issuercert                 : c_int ;
extern const curlopt_address_scope              : c_int ;
extern const curlopt_certinfo                   : c_int ;
extern const curlopt_username                   : c_int ;
extern const curlopt_password                   : c_int ;
extern const curlopt_proxyusername              : c_int ;
extern const curlopt_proxypassword              : c_int ;
extern const curlopt_noproxy                    : c_int ;
extern const curlopt_tftp_blksize               : c_int ;
extern const curlopt_socks5_gssapi_service      : c_int ;
extern const curlopt_socks5_gssapi_nec          : c_int ;
extern const curlopt_protocols                  : c_int ;
extern const curlopt_redir_protocols            : c_int ;
extern const curlopt_lastentry                  : c_int ;
extern const curlopt_mail_from                  : c_int ;
extern const curlopt_mail_rcpt                  : c_int ;
extern const curlopt_mail_auth                  : c_int ;

} /* end of module */
