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
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*

Simple support for many network protocols with libcurl

This module provides support for libcurl, enabling Chapel programs
to work with many network protocols.

The `curl homepage <https://curl.haxx.se/libcurl/>`_ describes libcurl thus::

 libcurl is a free and easy-to-use client-side URL transfer library, supporting
 DICT, FILE, FTP, FTPS, Gopher, HTTP, HTTPS, IMAP, IMAPS, LDAP, LDAPS, POP3,
 POP3S, RTMP, RTSP, SCP, SFTP, SMTP, SMTPS, Telnet and TFTP.  libcurl supports
 SSL certificates, HTTP POST, HTTP PUT, FTP uploading, HTTP form based upload,
 proxies, cookies, user+password authentication (Basic, Digest, NTLM,
 Negotiate, Kerberos), file transfer resume, http proxy tunneling and more!

Dependencies
------------

The Curl module in Chapel is dependent on libcurl. For more information
 on how to setup libcurl, see the :ref:`readme-auxIO` page.

Using Curl Support in Chapel
----------------------------

Chapel support for libcurl exposes two interfaces:
 1. A standard file I/O interface
 2. Bindings to libcurl's curl_easy_setopt and curl_easy_perform

Interface 1:
  Since we do not support multiple readers on the same Curl handle, it is
  recommended that Chapel programs use :proc:`~IO.openreader` and
  :proc:`~IO.openwriter` in order to open up channels on a URL. Examples of how
  :proc:`~IO.openreader` and :proc:`~IO.openwriter` work can be found below.
  You may also open up a curl handle on a URL via :proc:`~IO.open`
  `(url=..., mode=...)`, and then open up channels, or use the second interface
  described below to set options on that handle before opening up a channel on
  the handle. Note that in this last case, Chapel programs must not open
  multiple channels for the same file backed by a curl handle.

  .. note::

    The URL is the last parameter to :proc:`~IO.open`. Therefore when you are
    opening a URL if you are not using all of the parameters for
    :proc:`~IO.open()` or :proc:`~IO.openreader`/:proc:`~IO.openwriter`, you
    MUST specify all arguments by name, e.g.:

    .. code-block:: chapel

      open(url="https://example.com", iomode.r);

    will result in a compile time error. You would, instead, need to use:

    .. code-block:: chapel

      open(url="https://example.com", mode=iomode.r).


Interface 2:
  Many times when we are connecting to a URL (FTP, IMAP, SMTP, HTTP) we have to
  give extra information to the Curl handle. This is done via the setopt()
  interface.  Documentation on the various options, as well as the functions
  that are referenced below can be found `here
  <https://curl.haxx.se/libcurl/c/libcurl-easy.html>`_

  Types:
    * :record:`slist`
  Methods:
    * :proc:`slist.append`
    * :proc:`slist.free`
    * :proc:`file.setopt`
    * :proc:`file.perform`

   Calling :proc:`~IO.file.close` on the file will disconnect the underlying
   Curl handle.


Here are some simple code snippets demonstrating these two interfaces:

Example 1
---------

.. code-block:: chapel

  // This example uses the first interface

  var writer = openwriter("out.txt");
  // Open a URL and get a reader on a section of the site
  var reader = openreader(url="https://example.com");

  var str:string;

  // While we can read a line from example.com, write it to 'out.txt'
  while(reader.readline(str)) do
    writer.write(str);

Example 2
---------

.. code-block:: chapel

  // This example uses the first interface
  var str:string;
  var reader = openreader(url="https://example.com");
  reader.readstring(str);
  reader.close();

  // Write out to a URL via Curl
  var writer = openwriter("https://127.0.0.1:1080");
  writer.write(str);
  writer.close();

Example 3
---------

.. code-block:: chapel

  // Open a file on our local file system
  var f = openwriter("out.txt");
  // Now get a curl handle
  var c = openreader(url="https://example.com");
  var str:string;

  // Read from example.com and write it out to out.txt
  c.readstring(str);
  f.write(str);

  // Disconnect and free the curl handle and channel, and close the local file and
  // channel
  c.close();
  f.close();


Example 4
---------

.. code-block:: chapel

  // This example uses the second interface

  // Open a file with a curl handle as the internal file
  var c = open(url="https://example.com", mode=iomode.r);
  // do a standard perform operation on the underlying curl handle
  c.perform(); // This will print to stdout
  // disconnect and free the curl handle
  c.close();


Example 5
---------

.. code-block:: chapel

  // This example uses the second interface + the first interface

  var c = open(url="https://example.com", mode=iomode.r);
  var str:string;
  // Set verbose output from curl
  c.setopt(curlopt_verbose, true);

  // now read into the string
  var reader = c.reader();
  reader.readstring(str);
  writeln(str);
  reader.close();
  c.close();


Example 6
---------

.. code-block:: chapel

  // Connect to an IMAP site, and fetch mail from the inbox
  config const username = "user";
  config const password = "xxxx";
  config const imapSite = "your_imap_site_here";

  var handle = open(url=imapSite+"/INBOX/;UID=1", mode=iomode.r);
  var reader = handle.reader();
  var str:string;

  handle.setopt((curlopt_username, username),
                (curlopt_password, password));

  //Calling this would give the same output
  // handle.perform();

  reader.readstring(str);
  write(str);

  reader.close();
  handle.close();

.. _curl-example-7:

Example 7
---------

.. code-block:: chapel

  // This example shows a more complex example of how the two interfaces can work
  // together: We use the second (setopt) interface to set the various options we
  // need on a given curl handle, and then open a writer on that handle to write
  // out via curl.

  config const url      = "smtp goes here";

  config const from     = "<some-email>";
  config const to       = "<some-email>";
  config const cc       = "<some-email>";

  config const username = "username";
  config const password = "password";

  config const subject  = "Testing Curl in Chapel (SMTP)";
  config const message  = "Hello! This is a message sent via Chapel!";


  var handle = open(url=url, mode=iomode.cw);

  var recipients:slist;
  recipients.append(to);
  recipients.append(cc);

  var arr = "To: " + to + "\r\n" +
            "From: " + from + "\r\n" +
            "Cc: " + cc + "\r\n" +
            "Subject: " + subject + "\r\n" + "\r\n" + message;


  handle.setopt((curlopt_username       , username),
                (curlopt_password       , password),
                (curlopt_mail_from      , from),
                (curlopt_use_ssl        , 3),
                (curlopt_ssl_verifypeer , false),
                (curlopt_ssl_verifyhost , false),
                (curlopt_mail_rcpt      , recipients.list),
                (curlopt_verbose        , true));

  // Create a writer on the curl handle.
  var writer = handle.writer();

  // Now write out to Curl to send the email
  writer.write(arr);

  // recipients.free(); // BAD: This will free the data while it is in use!
  writer.close();
  // Note here how we free the slist AFTER we close the writer. This way we can
  // ensure that we do not free data that is currently in use by curl.
  recipients.free();
  handle.close();


Curl Support Types and Functions
--------------------------------

 */
module Curl {

require "-lcurl";
require "CurlHelper/qio_plugin_curl.h";
//require "CurlHelper/qio_plugin_curl.c";

use Sys only ;
use Time only ;

// This corresponds to struct curl_slist* when we have curl enabled
pragma "no doc"
extern type chpl_slist;

// QIO functions used
private extern proc sys_iov_total_bytes(iov:c_ptr(qiovec_t), iovcnt:c_int):int(64);
private extern proc qio_mkerror_errno():syserr;

private extern proc chpl_curl_set_opt(fl:qio_file_ptr_t, opt:c_int, arg...):syserr;
private extern proc chpl_curl_perform(fl:qio_file_ptr_t):syserr;
private extern proc chpl_curl_slist_append(ref list:chpl_slist, str:c_string):syserr;
private extern proc chpl_curl_slist_free(list:chpl_slist);

private extern const CHPL_CURL_SLIST_NULL:chpl_slist;

/* This function is the equivalent to the
   `curl_easy_setopt <https://curl.haxx.se/libcurl/c/curl_easy_setopt.html>`_
   function in libcurl. It sets information on the curl file handle
   that can change libcurl's behavior.

   :arg opt: the curl option to set. This file makes available lower
             case versions of these curl options (e.g. use curlopt_url
             instead of CURLOPT_URL in your Chapel program)
   :arg arg: the value to set the curl option specified by opt.
   :type arg: `int`, `string`, `bool`, or `slist`
*/
proc file.setopt(opt:c_int, arg):bool throws {
  var err:syserr = ENOERR;

  if (arg.type == slist) && (slist.home != this.home) {
    try ioerror(EFAULT:syserr, "in file.setopt(): slist, and curl handle do not reside on the same locale");
  }

  on this.home {
    err = chpl_curl_set_opt(this._file_internal, opt, arg);
  }

  if err then try ioerror(err, "in file.setopt(opt:c_int, arg)");
  return true;
}

/* Set curl options on a curl file. It is equivalent to the
   curl_setopt_array you might find in PHP.

   For example, you might do:

   .. code-block:: chapel

     curlfile.setopt((curlopt_username, username),
                     (curlopt_password, password));

   :arg args: any number of tuples of the form (curl_option, value). For each
              curl_option, this function will setopt it to its value.
 */
proc file.setopt(args ...?k) {
  for param i in 1..k {
    this.setopt(args(i)(1), args(i)(2));
  }
}

/* Perform any blocking file transfer operations on the curl file.
   This function calls
   `curl_easy_perform <https://curl.haxx.se/libcurl/c/curl_easy_perform.html>`_.

   Corresponds to
   `curl_easy_perform <https://curl.haxx.se/libcurl/c/curl_easy_perform.html>`_
   where the file has been opened up by specifying that `url=<some url>`.

   :returns: true on success. This version halts if an error is encountered,
             but future versions will support returning an error code instead
             of halting.
 */
proc file.perform():bool throws {
  var err:syserr = ENOERR;

  on this.home {
    err = chpl_curl_perform(this._file_internal);
  }

  if err then try ioerror(err, "in file.perform()");
  return true;
}

/*
   A linked list of strings used in many curl setopt calls. This type
   corresponds to the libcurl type curl_slist.

.. note::

   The slist type is not reference counted. Therefore the user is responsible
   for freeing the slist when they are done with it with :proc:`slist.free`.

*/
record slist {
  pragma "no doc"
  var home: locale = here;
  // Note: If we do not set the default value of this to NULL, we can get
  // non-deterministic segfaults from libcurl.
  pragma "no doc"
  var list: chpl_slist = CHPL_CURL_SLIST_NULL;
}

/* Append the string argument to an slist. This function is the same
   as calling
   `curl_slist_append <https://curl.haxx.se/libcurl/c/curl_slist_append.html>`_

   This function halts if an error is encountered. Future versions will
   support returning an error code instead of halting.

   :arg str: a string argument to append
  */
proc slist.append(str:string) throws {
  var err: syserr = ENOERR;
  on this.home {
    err = chpl_curl_slist_append(this.list, str.localize().c_str());
  }
  if err then try ioerror(err, "in slist.append()");
}

/* Free an slist. Chapel programs must call this function after using an slist.
   Programs must ensure that there are no ongoing connections using
   this slist when it is freed. For an example, see :ref:`curl-example-7`.
 */
proc slist.free() {
  on this.home {
    chpl_curl_slist_free(this.list);
  }
}

// These are meant to be used with the file.setopt() function. This way, a user
// has access to the easy interface.


/* See https://curl.haxx.se/libcurl/c/CURLOPT_WRITEDATA.html */
extern const curlopt_file                       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_URL.html */
extern const curlopt_url                        : c_int ;
extern const CURLOPT_URL                        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PORT.html */
extern const curlopt_port                       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXY.html */
extern const curlopt_proxy                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_USERPWD.html */
extern const curlopt_userpwd                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYUSERPWD.html */
extern const curlopt_proxyuserpwd               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_RANGE.html */
extern const curlopt_range                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_READDATA.html */
extern const curlopt_infile                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_ERRORBUFFER.html */
extern const curlopt_errorbuffer                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html */
extern const curlopt_writefunction              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_READFUNCTION.html */
extern const curlopt_readfunction               : c_int ;
extern const CURLOPT_READFUNCTION               : c_int ;
extern const CURLOPT_READDATA                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TIMEOUT.html */
extern const curlopt_timeout                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_INFILESIZE.html */
extern const curlopt_infilesize                 : c_int ;
extern const CURLOPT_INFILESIZE                 : c_int ;
extern const CURLOPT_INFILESIZE_LARGE           : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDS.html */
extern const curlopt_postfields                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_REFERER.html */
extern const curlopt_referer                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTPPORT.html */
extern const curlopt_ftpport                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_USERAGENT.html */
extern const curlopt_useragent                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_LOW_SPEED_LIMIT.html */
extern const curlopt_low_speed_limit            : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_LOW_SPEED_TIME.html */
extern const curlopt_low_speed_time             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_RESUME_FROM.html */
extern const curlopt_resume_from                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIE.html */
extern const curlopt_cookie                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPHEADER.html */
extern const curlopt_httpheader                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPPOST.html */
extern const curlopt_httppost                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLCERT.html */
extern const curlopt_sslcert                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_KEYPASSWD.html */
extern const curlopt_keypasswd                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CRLF.html */
extern const curlopt_crlf                       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_QUOTE.html */
extern const curlopt_quote                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HEADERDATA.html */
extern const CURLOPT_HEADERDATA                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIEFILE.html */
extern const curlopt_cookiefile                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLVERSION.html */
extern const curlopt_sslversion                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TIMECONDITION.html */
extern const curlopt_timecondition              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TIMEVALUE.html */
extern const curlopt_timevalue                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CUSTOMREQUEST.html */
extern const curlopt_customrequest              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_STDERR.html */
extern const curlopt_stderr                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTQUOTE.html */
extern const curlopt_postquote                  : c_int ;
/* This curlopt setting is deprecated */
extern const curlopt_writeinfo                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_VERBOSE.html */
extern const curlopt_verbose                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HEADER.html */
extern const curlopt_header                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NOPROGRESS.html */
extern const curlopt_noprogress                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NOBODY.html */
extern const curlopt_nobody                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FAILONERROR.html */
extern const curlopt_failonerror                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_UPLOAD.html */
extern const curlopt_upload                     : c_int ;
extern const CURLOPT_UPLOAD                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POST.html */
extern const curlopt_post                       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DIRLISTONLY.html */
extern const curlopt_dirlistonly                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_APPEND.html */
extern const curlopt_append                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NETRC.html */
extern const curlopt_netrc                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FOLLOWLOCATION.html */
extern const curlopt_followlocation             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TRANSFERTEXT.html */
extern const curlopt_transfertext               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PUT.html */
extern const curlopt_put                        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROGRESSFUNCTION.html */
extern const curlopt_progressfunction           : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROGRESSDATA.html */
extern const curlopt_progressdata               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_AUTOREFERER.html */
extern const curlopt_autoreferer                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYPORT.html */
extern const curlopt_proxyport                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDSIZE.html */
extern const curlopt_postfieldsize              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPPROXYTUNNEL.html */
extern const curlopt_httpproxytunnel            : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_INTERFACE.html */
extern const curlopt_interface                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_KRBLEVEL.html */
extern const curlopt_krblevel                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_VERIFYPEER.html */
extern const curlopt_ssl_verifypeer             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CAINFO.html */
extern const curlopt_cainfo                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAXREDIRS.html */
extern const curlopt_maxredirs                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FILETIME.html */
extern const curlopt_filetime                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TELNETOPTIONS.html */
extern const curlopt_telnetoptions              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAXCONNECTS.html */
extern const curlopt_maxconnects                : c_int ;
/* This option is deprecated */
extern const curlopt_closepolicy                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FRESH_CONNECT.html */
extern const curlopt_fresh_connect              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FORBID_REUSE.html */
extern const curlopt_forbid_reuse               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_RANDOM_FILE.html */
extern const curlopt_random_file                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_EGDSOCKET.html */
extern const curlopt_egdsocket                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONNECTTIMEOUT.html */
extern const curlopt_connecttimeout             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HEADERFUNCTION.html */
extern const curlopt_headerfunction             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPGET.html */
extern const curlopt_httpget                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_VERIFYHOST.html */
extern const curlopt_ssl_verifyhost             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIEJAR.html */
extern const curlopt_cookiejar                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_CIPHER_LIST.html */
extern const curlopt_ssl_cipher_list            : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTP_VERSION.html */
extern const curlopt_http_version               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_USE_EPSV.html */
extern const curlopt_ftp_use_epsv               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLCERTTYPE.html */
extern const curlopt_sslcerttype                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLKEY.html */
extern const curlopt_sslkey                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLKEYTYPE.html */
extern const curlopt_sslkeytype                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLENGINE.html */
extern const curlopt_sslengine                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLENGINE_DEFAULT.html */
extern const curlopt_sslengine_default          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DNS_USE_GLOBAL_CACHE.html */
extern const curlopt_dns_use_global_cache       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DNS_CACHE_TIMEOUT.html */
extern const curlopt_dns_cache_timeout          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PREQUOTE.html */
extern const curlopt_prequote                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DEBUGFUNCTION.html */
extern const curlopt_debugfunction              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DEBUGDATA.html */
extern const curlopt_debugdata                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIESESSION.html */
extern const curlopt_cookiesession              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CAPATH.html */
extern const curlopt_capath                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_BUFFERSIZE.html */
extern const curlopt_buffersize                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NOSIGNAL.html */
extern const curlopt_nosignal                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SHARE.html */
extern const curlopt_share                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYTYPE.html */
extern const curlopt_proxytype                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_ACCEPT_ENCODING.html */
extern const curlopt_encoding                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PRIVATE.html */
extern const curlopt_private                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTP200ALIASES.html */
extern const curlopt_http200aliases             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_UNRESTRICTED_AUTH.html */
extern const curlopt_unrestricted_auth          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_USE_EPRT.html */
extern const curlopt_ftp_use_eprt               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPAUTH.html */
extern const curlopt_httpauth                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_CTX_FUNCTION.html */
extern const curlopt_ssl_ctx_function           : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_CTX_DATA.html */
extern const curlopt_ssl_ctx_data               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_CREATE_MISSING_DIRS.html */
extern const curlopt_ftp_create_missing_dirs    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYAUTH.html */
extern const curlopt_proxyauth                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_RESPONSE_TIMEOUT.html */
extern const curlopt_ftp_response_timeout       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_IPRESOLVE.html */
extern const curlopt_ipresolve                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAXFILESIZE.html */
extern const curlopt_maxfilesize                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_INFILESIZE_LARGE.html */
extern const curlopt_infilesize_large           : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_RESUME_FROM_LARGE.html */
extern const curlopt_resume_from_large          : c_int ;
extern const CURLOPT_RESUME_FROM_LARGE          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAXFILESIZE_LARGE.html */
extern const curlopt_maxfilesize_large          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NETRC_FILE.html */
extern const curlopt_netrc_file                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_USE_SSL.html */
extern const curlopt_use_ssl                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDSIZE_LARGE.html */
extern const curlopt_postfieldsize_large        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TCP_NODELAY.html */
extern const curlopt_tcp_nodelay                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTPSSLAUTH.html */
extern const curlopt_ftpsslauth                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_IOCTLFUNCTION.html */
extern const curlopt_ioctlfunction              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_IOCTLDATA.html */
extern const curlopt_ioctldata                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_ACCOUNT.html */
extern const curlopt_ftp_account                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIELIST.html */
extern const curlopt_cookielist                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_IGNORE_CONTENT_LENGTH.html */
extern const curlopt_ignore_content_length      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_SKIP_PASV_IP.html */
extern const curlopt_ftp_skip_pasv_ip           : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_FILEMETHOD.html */
extern const curlopt_ftp_filemethod             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_LOCALPORT.html */
extern const curlopt_localport                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_LOCALPORTRANGE.html */
extern const curlopt_localportrange             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONNECT_ONLY.html */
extern const curlopt_connect_only               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONV_FROM_NETWORK_FUNCTION.html */
extern const curlopt_conv_from_network_function : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONV_TO_NETWORK_FUNCTION.html */
extern const curlopt_conv_to_network_function   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONV_FROM_UTF8_FUNCTION.html */
extern const curlopt_conv_from_utf8_function    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAX_SEND_SPEED_LARGE.html */
extern const curlopt_max_send_speed_large       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAX_RECV_SPEED_LARGE.html */
extern const curlopt_max_recv_speed_large       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_ALTERNATIVE_TO_USER.html */
extern const curlopt_ftp_alternative_to_user    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SOCKOPTFUNCTION.html */
extern const curlopt_sockoptfunction            : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SOCKOPTDATA.html */
extern const curlopt_sockoptdata                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_SESSIONID_CACHE.html */
extern const curlopt_ssl_sessionid_cache        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSH_AUTH_TYPES.html */
extern const curlopt_ssh_auth_types             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSH_PUBLIC_KEYFILE.html */
extern const curlopt_ssh_public_keyfile         : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSH_PRIVATE_KEYFILE.html */
extern const curlopt_ssh_private_keyfile        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_SSL_CCC.html */
extern const curlopt_ftp_ssl_ccc                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TIMEOUT_MS.html */
extern const curlopt_timeout_ms                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONNECTTIMEOUT_MS.html */
extern const curlopt_connecttimeout_ms          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTP_TRANSFER_DECODING.html */
extern const curlopt_http_transfer_decoding     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTP_CONTENT_DECODING.html */
extern const curlopt_http_content_decoding      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NEW_FILE_PERMS.html */
extern const curlopt_new_file_perms             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NEW_DIRECTORY_PERMS.html */
extern const curlopt_new_directory_perms        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTREDIR.html */
extern const curlopt_postredir                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSH_HOST_PUBLIC_KEY_MD5.html */
extern const curlopt_ssh_host_public_key_md5    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_OPENSOCKETFUNCTION.html */
extern const curlopt_opensocketfunction         : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_OPENSOCKETDATA.html */
extern const curlopt_opensocketdata             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COPYPOSTFIELDS.html */
extern const curlopt_copypostfields             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_TRANSFER_MODE.html */
extern const curlopt_proxy_transfer_mode        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SEEKFUNCTION.html */
extern const curlopt_seekfunction               : c_int ;
extern const CURLOPT_WRITEFUNCTION              : c_int ;
extern const CURLOPT_WRITEDATA                  : c_int ;
extern const CURLOPT_NOBODY                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SEEKDATA.html */
extern const curlopt_seekdata                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CRLFILE.html */
extern const curlopt_crlfile                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_ISSUERCERT.html */
extern const curlopt_issuercert                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_ADDRESS_SCOPE.html */
extern const curlopt_address_scope              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CERTINFO.html */
extern const curlopt_certinfo                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_USERNAME.html */
extern const curlopt_username                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PASSWORD.html */
extern const curlopt_password                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYUSERNAME.html */
extern const curlopt_proxyusername              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYPASSWORD.html */
extern const curlopt_proxypassword              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NOPROXY.html */
extern const curlopt_noproxy                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TFTP_BLKSIZE.html */
extern const curlopt_tftp_blksize               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SOCKS5_GSSAPI_SERVICE.html */
extern const curlopt_socks5_gssapi_service      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SOCKS5_GSSAPI_NEC.html */
extern const curlopt_socks5_gssapi_nec          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROTOCOLS.html */
extern const curlopt_protocols                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_REDIR_PROTOCOLS.html */
extern const curlopt_redir_protocols            : c_int ;
/* All other curlopt values will be less than this one */
extern const curlopt_lastentry                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAIL_FROM.html */
extern const curlopt_mail_from                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAIL_RCPT.html */
extern const curlopt_mail_rcpt                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAIL_AUTH.html */
extern const curlopt_mail_auth                  : c_int ;

extern const CURLINFO_CONTENT_LENGTH_DOWNLOAD   : c_int ;
extern const CURLE_RANGE_ERROR: c_int;
extern const CURLE_OK: c_int;

pragma "no doc"
extern type CURL;
pragma "no doc"
extern type CURLM;

extern type CURLoption=c_int;
extern type CURLcode=c_int;
extern type CURLMcode=c_int;
extern type CURLINFO=c_int;

extern proc curl_easy_init():c_ptr(CURL);
extern proc chpl_curl_easy_setopt_long(curl:c_ptr(CURL), option:CURLoption, arg:c_long):CURLcode;
extern proc chpl_curl_easy_setopt_ptr(curl:c_ptr(CURL), option:CURLoption, arg:c_void_ptr):CURLcode;
extern proc chpl_curl_easy_setopt_offset(curl:c_ptr(CURL), option:CURLoption, offset:int(64)):CURLcode;
extern proc chpl_curl_easy_getinfo_ptr(curl:c_ptr(CURL), info:CURLINFO, c_void_ptr):CURLcode;

extern proc curl_easy_perform(curl:c_ptr(CURL)):CURLcode;
extern proc curl_easy_cleanup(curl:c_ptr(CURL)):void;
extern proc curl_easy_pause(curl:c_ptr(CURL), bitmask:c_int):CURLcode;

extern proc curl_multi_init():c_ptr(CURLM);
extern proc curl_multi_add_handle(curlm:c_ptr(CURLM), curl:c_ptr(CURL)):CURLMcode;
extern proc curl_multi_timeout(curlm:c_ptr(CURLM), ref timeout:c_long):CURLMcode;
extern proc curl_multi_fdset(curlm:c_ptr(CURLM), read_fd_set:c_ptr(fd_set), write_fd_set:c_ptr(fd_set), exc_fd_set:c_ptr(fd_set), ref max_fd:c_int):CURLMcode;
extern proc curl_multi_perform(curlm:c_ptr(CURLM), ref running_handles):CURLMcode;
extern proc curl_multi_remove_handle(curlm:c_ptr(CURLM), curl:c_ptr(CURL)):CURLMcode;
extern proc curl_multi_cleanup(curlm:c_ptr(CURLM)):CURLcode;

extern const CURL_READFUNC_PAUSE:size_t;
extern const CURL_WRITEFUNC_PAUSE:size_t;
extern const CURLPAUSE_ALL: c_int;
extern const CURLPAUSE_CONT: c_int;

// Since a curl handle does not hold where it has read to, we need to do
// this here.
// As well, since we can many times request byte-ranges (for HTTP/HTTPS)
// we keep track of that here as well.

class CurlFile : QioPluginFile {

  var curl: c_ptr(CURL);  // Curl handle
  var curlm: c_ptr(CURLM);  // Curl multi handle
  var running_handles: c_int;
  var pathnm: c_string;     // Path/URL
  var length: ssize_t;    // length of what we are reading, -1 if we can't get

  // Note: This is racy if we open up more than one channel on a given curl
  // handle.  It is expected of the user that they will not open more than one
  // channel at once on a given Curl handle and use openreader() and
  // openwriter() in order to open up channels for Curl.

  var current_offset: int;
  var seekable: bool;

  override proc writev(iov:c_ptr(qiovec_t), iovcnt:c_int, out amtWritten:ssize_t):syserr {
    return curl_writev(this, iov, iovcnt, amtWritten);
  }
  override proc readv(iov:c_ptr(qiovec_t), iovcnt:c_int, out amtRead:ssize_t):syserr {
    writeln("in readv");
    return curl_preadv_internal(this, iov, iovcnt, -1, amtRead);
  }

  override proc pwritev(iov:c_ptr(qiovec_t), iovcnt:c_int, offset:int(64), out amtWritten:ssize_t):syserr {
    return ENOSYS;
  }
  override proc preadv(iov:c_ptr(qiovec_t), iovcnt:c_int, offset:int(64), out amtRead:ssize_t):syserr {
    writeln("in preadv");
    return curl_preadv_internal(this, iov, iovcnt, offset, amtRead);
  }

  override proc seek(amount:int(64), whence:c_int, out offset:int(64)):syserr {
    return ENOSYS;
  }

  override proc filelength(out length:int(64)):syserr {
    if this.length == -1 then {
      return ENOTSUP;
    }

    length = this.length;
    return ENOERR;
  }
  override proc getpath(out path:string):syserr {
    //var s = new string(this.pathnm);
    //path = s;
    path = this.pathnm;
    return ENOERR;
  }

  override proc fsync():syserr {
    return ENOSYS;
  }
  override proc getChunk(out length:int(64)):syserr {
    return ENOSYS;
  }
  override proc getLocalesForRegion(start:int(64), end:int(64), out
      localeNames:c_ptr(c_string), ref nLocales:int(64)):syserr {
    return ENOSYS;
  }

  override proc close():syserr {
    c_free(pathnm:c_void_ptr);
    pathnm = nil;
    curl_multi_remove_handle(curlm, curl);
    curl_easy_cleanup(curl);
    curl_multi_cleanup(curlm);
    return ENOERR;
  }
}

pragma "no doc"
class CurlFilesystem : QioPluginFilesystem {
  // TODO: should these throw instead of returning qio errors?

/*
  override proc open(path:c_string, pathlen:ssize_t, access:c_string, hints:int(64), out file:QioPluginFile):syserr {
    return ENOSYS;
  }
  override proc getCwd(out path:c_string, out len:ssize_t):syserr {
    return ENOSYS;
  }
  override proc getFsType(out fsType:int(64)):syserr {
    return ENOSYS;
  }*/
  /*proc handlesUrl(url: string): bool {
    return url.startsWith("http://", "https://",
                          "ftp://", "ftps://",
                          "smtp://", "smtps://",
                          "imap://", "imaps://");
  }*/
}

// Since the callback is called many times from a call to curl_easy_perform,
// and since we know the amount that we need to read into the iovec passed into
// preadv/readv resp. we put the entire iovec (that is passed into readv/preadv)
// into the vec field of this struct. This way, we only call
// curl_easy_perform once (and thus, get rid of the overhead that multiple
// calls could cause).

record curl_iovec_t {
  var vec:c_ptr(qiovec_t); // iovec to read into --
                           // (the iovec passed in curl_readv/curl_preadv)
  var total_read:size_t;   // total amount read
  var amt_read:size_t;     // amount read into the current iovec buffer
  var count:c_int;         // number of buffers in the iovec
  var offset:size_t;       // offset that we want to skip to
                           // (in the case where we cannot request byteranges)
  var curr:c_int;          // the index of the current buffer
};

// userdata, is a curl_iovec_t. This is set to be passed into this function,
// when we
// call CURLOPT_WRITEDATA in curl_preadv and curl_readv.
// FUTURE: If we have filled the iovec, but have not finished reading from the curl
// handle, pause it (i.e., return CURL_WRITE_PAUSE).

proc pause_writer(ptr:c_void_ptr, size:size_t, nmemb:size_t, userdata:c_void_ptr):size_t
{
  writeln("in pause_writer");
  return CURL_WRITEFUNC_PAUSE;
}
proc pause_reader(ptr:c_void_ptr, size:size_t, nmemb:size_t, userdata:c_void_ptr):size_t
{
  writeln("in pause_reader");
  return CURL_READFUNC_PAUSE;
}


proc buf_writer(ptr:c_void_ptr, size:size_t, nmemb:size_t, userdata:c_void_ptr):size_t
{
  var ptr_data = ptr:c_ptr(uint(8));
  var realsize:size_t = size*nmemb;
  var real_realsize:size_t = realsize;
  var retptr = userdata:c_ptr(curl_iovec_t);
  ref ret = retptr.deref();

  if size > sys_iov_total_bytes(ret.vec, ret.count) {
    writeln("in buf_writer PAUSE");
    return CURL_WRITEFUNC_PAUSE;
  }

  writeln("in buf_writer");

  // so that we can "seek" when we cannot request byteranges
  if realsize < ret.offset {
    ret.offset -= realsize;
    writeln("A: ", realsize);
    return realsize;
  } else {
    // nop when we are done -- non-zero the first time through
    ptr_data = ptr_data + ret.offset;
    realsize -= ret.offset;
    ret.offset = 0;
    writeln("B: ", realsize);
  }

  // The amount that we have been given by curl is more than we can stick into a
  // single iovbuf. So we need to go from one iovbuf to the other
  while (realsize > ret.vec[ret.curr].iov_len - ret.amt_read) {
    writeln("C: ", realsize);
    var curbase = (ret.vec[ret.curr].iov_base):c_ptr(uint(8));
    var dst = curbase + ret.amt_read;
    var amt = ret.vec[ret.curr].iov_len - ret.amt_read;
    c_memcpy(dst, ptr_data, amt);
    ret.total_read += amt;
    realsize -= amt;
    ptr_data = ptr_data + amt;
    // Reset the amount that we have read into this vector.
    ret.amt_read = 0;
    if (ret.curr == ret.count-1) { // last iovbuf in this vector - stop reading
      return 0; // stop reading
    } else { // go to the next buf in this vector
      ret.curr += 1;
    }
  }

  // The amount of data that we have been given by curl is <= to the amount
  // of space that we have left in this iovbuf. So we can simply read it all in.
  if (realsize <= (ret.vec[ret.curr].iov_len - ret.amt_read)) {
    writeln("D: ", realsize);
    var curbase = (ret.vec[ret.curr].iov_base):c_ptr(uint(8));
    var dst = curbase + ret.amt_read;
    var amt = realsize;
    c_memcpy(dst, ptr_data, amt);
    ret.total_read += realsize;
    ret.amt_read += realsize;
    // We have fully populated this iovbuf
    if (ret.vec[ret.curr].iov_len == ret.amt_read) {
      if ret.curr == ret.count - 1 { // last iovbuf in this vector
        return 0; // stop reading
      } else { // else, step to the next buf.
        ret.curr += 1;
        ret.amt_read = 0;
      }
    }
  }
  writeln("E: ", real_realsize);
  return real_realsize;
}

// We want to upload data from the iovec passed into curl_writev. We populate a
// curl_iovec_t in curl_writev, and set it to be passed in when we call
// CURLOPT_READDATA. Note, that we can only return a chunk of memory at most
// size*nmemb big to curl (i.e., sizeof(ptr) <= size*nmemb after this function
// is called by CURL).
proc read_data(ptr:c_void_ptr, size:size_t, nmemb:size_t, userp:c_void_ptr):size_t
{
  var cur:c_void_ptr = ptr;
  var realsize:size_t = size*nmemb;
  var retptr = userp:c_ptr(curl_iovec_t);
  ref ret = retptr.deref();

  if (size == 0) || (nmemb == 0) || ((size*nmemb) < 1) ||
     (ret.curr >= ret.count) {
    return 0; // stop putting data
  }

  // We can upload more than one iovbuf at once, so do it.
  while (realsize > ret.vec[ret.curr].iov_len - ret.amt_read)  {
    var curbase = (ret.vec[ret.curr].iov_base):c_ptr(uint(8));
    var src = curbase + ret.amt_read;
    var amt = ret.vec[ret.curr].iov_len - ret.amt_read;
    c_memcpy(cur, src, amt);
    ret.total_read += amt;
    realsize -= amt;
    cur = (cur:c_ptr(uint(8)) + amt):c_void_ptr;
    // Reset the amount that we have read out of this vector.
    ret.amt_read = 0;
    // go to the next vector
    ret.curr += 1;
    if ret.curr >= ret.count then
      return ret.total_read;
  }

  // The amount of data that we need to hand to curl is <= the amount of space
  // that we have left in this iovbuf, so we have to be careful not to exceed it
  if (realsize <= (ret.vec[ret.curr].iov_len - ret.amt_read)) {
    var curbase = ret.vec[ret.curr].iov_base;
    var src = (curbase:c_ptr(uint(8)) + ret.amt_read):c_void_ptr;
    c_memcpy(cur, src, realsize);
    ret.total_read += realsize;
    ret.amt_read += realsize;
    // We have fully read this iovbuf
    if (ret.vec[ret.curr].iov_len == ret.amt_read) {
      ret.curr += 1;
      ret.amt_read = 0;
    }
  }
  return ret.total_read;
}

// TODO: while it's OK to "seek" to a particular offset,
// it's not good that we're restarting the transfer
// on every call here with the readv case.. not sure how that's happening
proc curl_preadv_internal(local_handle:CurlFile, vector:c_ptr(qiovec_t), count:c_int, in offset:int(64), out num_read_out:ssize_t)
{
  var err:CURLcode = 0;
  var serr:syserr = 0;
  var got_total:ssize_t;
  var err_out:syserr = ENOERR;
  var write_vec:curl_iovec_t;

  writeln("in curl_preadv_internal offset ", offset);
  //STARTING_SLOW_SYSCALL;

  got_total = 0;
  write_vec.total_read = 0;
  write_vec.count = count;
  write_vec.vec = vector;
  write_vec.amt_read = 0;
  write_vec.offset = 0;
  write_vec.curr = 0;

  // offset = -1 if we want readv
  if offset == -1 then
    offset = local_handle.current_offset;

  if local_handle.seekable {  // we can request byteranges
    chpl_curl_easy_setopt_offset(local_handle.curl, CURLOPT_RESUME_FROM_LARGE, offset);
    writeln("setting resume from large");
  } else {
    write_vec.offset = offset:size_t;
    writeln("setting offset");
  }

  chpl_curl_easy_setopt_ptr(local_handle.curl, CURLOPT_WRITEFUNCTION, c_ptrTo(buf_writer):c_void_ptr);

  // Read into write_vec
  chpl_curl_easy_setopt_ptr(local_handle.curl, CURLOPT_WRITEDATA, c_ptrTo(write_vec):c_void_ptr);

  // Continue anything that is paused
  err = curl_easy_pause(local_handle.curl, CURLPAUSE_CONT);

  var timeoutMillis:c_long;
  curl_multi_timeout(local_handle.curlm, timeoutMillis);
  if timeoutMillis < 0 then
    timeoutMillis = 0;

  var timeout:timeval;
  timeout.tv_sec = timeoutMillis / 1000;
  timeout.tv_usec = (timeoutMillis % 1000) * 1000;

  var fdread: fd_set;
  var fdwrite: fd_set;
  var fdexcept: fd_set;
  var maxfd:c_int = -1;
  sys_fd_zero(fdread);
  sys_fd_zero(fdwrite);
  sys_fd_zero(fdexcept);

  err  = curl_multi_fdset(local_handle.curlm, c_ptrTo(fdread), c_ptrTo(fdwrite), c_ptrTo(fdexcept), maxfd);
  if maxfd == -1 {
    // we can't wait with sockets for some reason, so wait for operation
    Time.sleep(0.1);
    writeln("sleeping");
  } else {
    writeln("selecting ", maxfd);
    var nset:c_int;
    serr = Sys.sys_select(maxfd+1, c_ptrTo(fdread), c_ptrTo(fdwrite), c_ptrTo(fdexcept), c_ptrTo(timeout), nset);
    if serr != 0 then
      return serr;
  }

  writeln("performing");
  err = curl_multi_perform(local_handle.curlm, local_handle.running_handles);

  //DONE_SLOW_SYSCALL;
  got_total = write_vec.total_read:int(64);
  local_handle.current_offset += got_total;

  if (err == CURLE_RANGE_ERROR ||  err == CURLE_OK) &&
     got_total == 0 &&
     sys_iov_total_bytes(vector, count) != 0 then
    err_out = EEOF;

  chpl_curl_easy_setopt_ptr(local_handle.curl, CURLOPT_WRITEFUNCTION, c_ptrTo(pause_writer):c_void_ptr);
  chpl_curl_easy_setopt_ptr(local_handle.curl, CURLOPT_WRITEDATA, nil:c_void_ptr);

  num_read_out = got_total;
  return err_out;
}

proc curl_writev(fl:CurlFile, iov:c_ptr(qiovec_t), iovcnt:c_int, out num_written_out:ssize_t):syserr
{
  var ret:CURLcode = 0;
  var err_out:syserr = 0;
  var write_vec:curl_iovec_t;

  write_vec.total_read = 0;
  write_vec.count = iovcnt;
  write_vec.vec = iov;
  write_vec.amt_read = 0;
  write_vec.offset = 0;
  write_vec.curr = 0;

  //STARTING_SLOW_SYSCALL;
  /*tell it to "upload" to the URL*/
  chpl_curl_easy_setopt_long(fl.curl, CURLOPT_UPLOAD, 1);
  // set it up to write over curl
  chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_READFUNCTION, c_ptrTo(read_data):c_void_ptr);
  // Tell curl how much data to expect
  chpl_curl_easy_setopt_offset(fl.curl, CURLOPT_INFILESIZE_LARGE, sys_iov_total_bytes(iov, iovcnt));
  chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_READDATA, c_ptrTo(write_vec));
  ret = curl_easy_pause(fl.curl, CURLPAUSE_CONT);
  num_written_out = write_vec.total_read:int(64);

  if ret != CURLE_OK then
    err_out = qio_mkerror_errno();

  chpl_curl_easy_setopt_long(fl.curl, CURLOPT_UPLOAD, 0);
  chpl_curl_easy_setopt_long(fl.curl, CURLOPT_INFILESIZE_LARGE, 0);
  chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_READFUNCTION, c_ptrTo(pause_reader):c_void_ptr);
  chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_READDATA, nil:c_void_ptr);

  //DONE_SLOW_SYSCALL;

  return err_out;
}

record curl_str_buf {
  var mem:c_ptr(uint(8));
  var len: size_t;
  var alloced: size_t;
}


private proc startsWith(haystack:c_string, needle:c_string) {
  extern proc strncmp(s1:c_string, s2:c_string, n:size_t):c_int;

  return strncmp(haystack, needle, needle.length:size_t) == 0;
}

private proc chpl_curl_write_string(contents: c_void_ptr, size:size_t, nmemb:size_t, userp: c_void_ptr) {
  var realsize:size_t = size * nmemb;
  var bufptr = userp:c_ptr(curl_str_buf);
  ref buf = bufptr.deref(); 

  if buf.len + realsize < buf.alloced {
    // OK
  } else {
    var newsize = 2 * buf.alloced + realsize;
    var oldsize = buf.len;
    var newbuf:c_ptr(uint(8));
    newbuf = c_calloc(uint(8), newsize);
    if newbuf == nil then
      return 0;
    c_memcpy(newbuf, buf.mem, oldsize);
    c_free(buf.mem);
    buf.mem = newbuf;
  }

  c_memcpy(c_ptrTo(buf.mem[buf.len]), contents, realsize);
  buf.len += realsize;
  buf.mem[buf.len] = 0;

  return realsize;
}
 
private proc seekable(fl:CurlFile, out length:int(64)):bool {
  var buf:curl_str_buf;
  var ret = false;

  if startsWith(fl.pathnm, "http://") || startsWith(fl.pathnm, "https://") {
    // We're on HTTP/HTTPS so we should look for byte ranges to see if we
    // can request them

    // The size doesn't really matter, we just want a place on the heap. This
    // will get expanded in curl_write_string.

    // Headers tend to be ~800, although they can grow much larger than this. If
    // it is larger than this, we'll take care of it in chpl_curl_write_string.

    buf.mem = c_calloc(uint(8), 800);
    buf.len = 0;
    buf.alloced = 800;

    chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_WRITEFUNCTION, c_ptrTo(chpl_curl_write_string):c_void_ptr);
    chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_HEADERDATA, c_ptrTo(buf));
    chpl_curl_easy_setopt_long(fl.curl, CURLOPT_NOBODY, 1);

    curl_easy_perform(fl.curl);

    chpl_curl_easy_setopt_long(fl.curl, CURLOPT_NOBODY, 0);
    chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_WRITEFUNCTION, c_ptrTo(pause_writer):c_void_ptr);
    chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_HEADERDATA, nil:c_void_ptr);

    extern proc strstr(haystack:c_string, needle:c_string):c_string;
    // Does this URL accept range requests?
    if strstr(buf.mem:c_string, c"Accept-Ranges: bytes"):c_void_ptr == nil:c_void_ptr {
      ret = false;
    } else {
      ret = true;
    }

    c_free(buf.mem);
  }

  var lengthDouble: real(64); 
  // This works for things other than http
  chpl_curl_easy_getinfo_ptr(fl.curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, c_ptrTo(lengthDouble));
  length = lengthDouble: int(64);
  // One day, use CURLINFO_CONTENT_LENGTH_DOWNLOAD_T

  // We can always "seek" on ftp (with the REST command/RESUME_FROM_LARGE)
  if startsWith(fl.pathnm, "ftp://") then
    ret = true;

  return ret;
}

proc openurl(url:string,
             mode:iomode = iomode.r,
             style:iostyle = defaultIOStyle()) throws {

  var err_out: syserr = ENOERR;
  var rc = 0;
  var filelength: int(64);

  var fl = new unmanaged CurlFile();

  //STARTING_SLOW_SYSCALL;

  fl.curl =  curl_easy_init();
  fl.curlm = curl_multi_init();

  if fl.curl == nil then
    throw SystemError.fromSyserr(ENOSYS, "Unable to create a Curl handle"); 
  if fl.curlm == nil then
    throw SystemError.fromSyserr(ENOSYS, "Unable to create a Curl mhandle"); 

  // Save the url requested
  var url_c = c_calloc(uint(8), url.length+1);
  c_memcpy(url_c:c_void_ptr, url.localize().c_str():c_void_ptr, url.length);

  // Setopt with the url
  chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_URL, url_c:c_void_ptr);

  fl.pathnm = url_c:c_string;
  fl.current_offset = 0;

  // Read the header in order to get the length of the thing we are reading
  // If we are writing, we can't really get this information (even if we try
  // to do a 0 length read).
  if (mode == iomode.cw || mode == iomode.cwr) {
    fl.length = -1;
    fl.seekable = false;
  } else {
    fl.seekable = seekable(fl, filelength);
    fl.length = filelength:ssize_t;
  }

  var err: CURLcode = 0;

  // Pause the curl handle now
  err = curl_easy_pause(fl.curl, CURLPAUSE_ALL);

  chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_READFUNCTION, c_ptrTo(pause_reader):c_void_ptr);
  chpl_curl_easy_setopt_ptr(fl.curl, CURLOPT_WRITEFUNCTION, c_ptrTo(pause_writer):c_void_ptr);

  writeln("X");
  err = curl_multi_add_handle(fl.curlm, fl.curl);

  writeln("Y");

  writeln(fl.seekable);
  writeln(fl.length);

  //DONE_SLOW_SYSCALL;

  var ret: file;

  try {
    ret = openplugin(fl, mode, fl.seekable, style);
  } catch e {
    delete fl;
    throw e;
  }

  return ret;
}

} /* end of module */
