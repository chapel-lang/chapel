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

require "CurlHelper/curlHelp.h";
require "-lcurl";

use Sys only ;
use Time only ;

/* This function is the equivalent to the
   `curl_easy_setopt <https://curl.haxx.se/libcurl/c/curl_easy_setopt.html>`_
   function in libcurl. It sets information on the curl file handle
   that can change libcurl's behavior.

   :arg opt: the curl option to set.
   :arg arg: the value to set the curl option specified by opt.
   :type arg: `int`, `string`, `bool`, or `slist`
*/
proc channel.setopt(opt:c_int, arg):bool throws {
  var err:syserr = ENOERR;

  if (arg.type == slist) && (slist.home != this.home) {
    throw new owned SystemError(EINVAL, "in channel.setopt(): slist, and curl handle do not reside on the same locale");
  }

  // Invalid argument type for option if the below conditionals
  // don't handle it.
  err = EINVAL;

  on this.home {
    var plugin = this.channelPlugin():CurlChannel?;
    if plugin == nil then
      throw new owned SystemError(EINVAL, "in channel.setopt(): not a curl channel");

    var curl = plugin.curl;

    // This reasoning is pulled from the libcurl source
    if (opt < CURLOPTTYPE_OBJECTPOINT) {
      if isIntegralType(arg.type) {
        var tmp:c_long = arg:c_long;
        err = qio_int_to_err(chpl_curl_easy_setopt_long(curl, opt:CURLoption, tmp));
      }
    } else if (opt < CURLOPTTYPE_OFF_T) {
      if isPointerType(arg.type) {
        var tmp:c_void_ptr = arg:c_void_ptr;
        err = qio_int_to_err(chpl_curl_easy_setopt_ptr(curl, opt:CURLoption, tmp));
      } else if arg.type == slist {
        var tmp:c_void_ptr = arg.list:c_void_ptr;
        err = qio_int_to_err(chpl_curl_easy_setopt_ptr(curl, opt:CURLoption, tmp));
      }
    } else {
      if isIntegralType(arg.type) {
        var tmp:curl_off_t = arg:curl_off_t;
        err = qio_int_to_err(chpl_curl_easy_setopt_offset(curl, opt:CURLoption, tmp));
      }
    }
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
proc channel.setopt(args ...?k) {
  for param i in 1..k {
    this.setopt(args(i)(1), args(i)(2));
  }
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
  var list: c_ptr(curl_slist) = nil;
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
    this.list = curl_slist_append(this.list, str.localize().c_str());
    if this.list == nil then
      err = EINVAL;
  }
  if err then try ioerror(err, "in slist.append()");
}

/* Free an slist. Chapel programs must call this function after using an slist.
   Programs must ensure that there are no ongoing connections using
   this slist when it is freed. For an example, see :ref:`curl-example-7`.
 */
proc slist.free() {
  on this.home {
    curl_slist_free_all(this.list);
  }
}

// These are meant to be used with the file.setopt() function. This way, a user
// has access to the easy interface.


/* See https://curl.haxx.se/libcurl/c/CURLOPT_WRITEDATA.html */
extern const CURLOPT_FILE                       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_URL.html */
extern const CURLOPT_URL                        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PORT.html */
extern const CURLOPT_PORT                       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXY.html */
extern const CURLOPT_PROXY                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_USERPWD.html */
extern const CURLOPT_USERPWD                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYUSERPWD.html */
extern const CURLOPT_PROXYUSERPWD               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_RANGE.html */
extern const CURLOPT_RANGE                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_READDATA.html */
extern const CURLOPT_INFILE                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_ERRORBUFFER.html */
extern const CURLOPT_ERRORBUFFER                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_READFUNCTION.html */
extern const CURLOPT_READFUNCTION               : c_int ;
extern const CURLOPT_READDATA                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TIMEOUT.html */
extern const CURLOPT_TIMEOUT                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_INFILESIZE.html */
extern const CURLOPT_INFILESIZE                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDS.html */
extern const CURLOPT_POSTFIELDS                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_REFERER.html */
extern const CURLOPT_REFERER                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTPPORT.html */
extern const CURLOPT_FTPPORT                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_USERAGENT.html */
extern const CURLOPT_USERAGENT                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_LOW_SPEED_LIMIT.html */
extern const CURLOPT_LOW_SPEED_LIMIT            : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_LOW_SPEED_TIME.html */
extern const CURLOPT_LOW_SPEED_TIME             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_RESUME_FROM.html */
extern const CURLOPT_RESUME_FROM                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIE.html */
extern const CURLOPT_COOKIE                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPHEADER.html */
extern const CURLOPT_HTTPHEADER                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPPOST.html */
extern const CURLOPT_HTTPPOST                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLCERT.html */
extern const CURLOPT_SSLCERT                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_KEYPASSWD.html */
extern const CURLOPT_KEYPASSWD                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CRLF.html */
extern const CURLOPT_CRLF                       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_QUOTE.html */
extern const CURLOPT_QUOTE                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HEADERDATA.html */
extern const CURLOPT_HEADERDATA                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIEFILE.html */
extern const CURLOPT_COOKIEFILE                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLVERSION.html */
extern const CURLOPT_SSLVERSION                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TIMECONDITION.html */
extern const CURLOPT_TIMECONDITION              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TIMEVALUE.html */
extern const CURLOPT_TIMEVALUE                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CUSTOMREQUEST.html */
extern const CURLOPT_CUSTOMREQUEST              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_STDERR.html */
extern const CURLOPT_STDERR                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTQUOTE.html */
extern const CURLOPT_POSTQUOTE                  : c_int ;
/* This curlopt setting is deprecated */
extern const CURLOPT_WRITEINFO                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_VERBOSE.html */
extern const CURLOPT_VERBOSE                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HEADER.html */
extern const CURLOPT_HEADER                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NOPROGRESS.html */
extern const CURLOPT_NOPROGRESS                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FAILONERROR.html */
extern const CURLOPT_FAILONERROR                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_UPLOAD.html */
extern const CURLOPT_UPLOAD                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POST.html */
extern const CURLOPT_POST                       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DIRLISTONLY.html */
extern const CURLOPT_DIRLISTONLY                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_APPEND.html */
extern const CURLOPT_APPEND                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NETRC.html */
extern const CURLOPT_NETRC                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FOLLOWLOCATION.html */
extern const CURLOPT_FOLLOWLOCATION             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TRANSFERTEXT.html */
extern const CURLOPT_TRANSFERTEXT               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PUT.html */
extern const CURLOPT_PUT                        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROGRESSFUNCTION.html */
extern const CURLOPT_PROGRESSFUNCTION           : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROGRESSDATA.html */
extern const CURLOPT_PROGRESSDATA               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_AUTOREFERER.html */
extern const CURLOPT_AUTOREFERER                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYPORT.html */
extern const CURLOPT_PROXYPORT                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDSIZE.html */
extern const CURLOPT_POSTFIELDSIZE              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPPROXYTUNNEL.html */
extern const CURLOPT_HTTPPROXYTUNNEL            : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_INTERFACE.html */
extern const CURLOPT_INTERFACE                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_KRBLEVEL.html */
extern const CURLOPT_KRBLEVEL                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_VERIFYPEER.html */
extern const CURLOPT_SSL_VERIFYPEER             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CAINFO.html */
extern const CURLOPT_CAINFO                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAXREDIRS.html */
extern const CURLOPT_MAXREDIRS                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FILETIME.html */
extern const CURLOPT_FILETIME                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TELNETOPTIONS.html */
extern const CURLOPT_TELNETOPTIONS              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAXCONNECTS.html */
extern const CURLOPT_MAXCONNECTS                : c_int ;
/* This option is deprecated */
extern const CURLOPT_CLOSEPOLICY                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FRESH_CONNECT.html */
extern const CURLOPT_FRESH_CONNECT              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FORBID_REUSE.html */
extern const CURLOPT_FORBID_REUSE               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_RANDOM_FILE.html */
extern const CURLOPT_RANDOM_FILE                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_EGDSOCKET.html */
extern const CURLOPT_EGDSOCKET                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONNECTTIMEOUT.html */
extern const CURLOPT_CONNECTTIMEOUT             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HEADERFUNCTION.html */
extern const CURLOPT_HEADERFUNCTION             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPGET.html */
extern const CURLOPT_HTTPGET                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_VERIFYHOST.html */
extern const CURLOPT_SSL_VERIFYHOST             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIEJAR.html */
extern const CURLOPT_COOKIEJAR                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_CIPHER_LIST.html */
extern const CURLOPT_SSL_CIPHER_LIST            : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTP_VERSION.html */
extern const CURLOPT_HTTP_VERSION               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_USE_EPSV.html */
extern const CURLOPT_FTP_USE_EPSV               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLCERTTYPE.html */
extern const CURLOPT_SSLCERTTYPE                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLKEY.html */
extern const CURLOPT_SSLKEY                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLKEYTYPE.html */
extern const CURLOPT_SSLKEYTYPE                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLENGINE.html */
extern const CURLOPT_SSLENGINE                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSLENGINE_DEFAULT.html */
extern const CURLOPT_SSLENGINE_DEFAULT          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DNS_USE_GLOBAL_CACHE.html */
extern const CURLOPT_DNS_USE_GLOBAL_CACHE       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DNS_CACHE_TIMEOUT.html */
extern const CURLOPT_DNS_CACHE_TIMEOUT          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PREQUOTE.html */
extern const CURLOPT_PREQUOTE                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DEBUGFUNCTION.html */
extern const CURLOPT_DEBUGFUNCTION              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_DEBUGDATA.html */
extern const CURLOPT_DEBUGDATA                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIESESSION.html */
extern const CURLOPT_COOKIESESSION              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CAPATH.html */
extern const CURLOPT_CAPATH                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_BUFFERSIZE.html */
extern const CURLOPT_BUFFERSIZE                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NOSIGNAL.html */
extern const CURLOPT_NOSIGNAL                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SHARE.html */
extern const CURLOPT_SHARE                      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYTYPE.html */
extern const CURLOPT_PROXYTYPE                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_ACCEPT_ENCODING.html */
extern const CURLOPT_ENCODING                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PRIVATE.html */
extern const CURLOPT_PRIVATE                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTP200ALIASES.html */
extern const CURLOPT_HTTP200ALIASES             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_UNRESTRICTED_AUTH.html */
extern const CURLOPT_UNRESTRICTED_AUTH          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_USE_EPRT.html */
extern const CURLOPT_FTP_USE_EPRT               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTPAUTH.html */
extern const CURLOPT_HTTPAUTH                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_CTX_FUNCTION.html */
extern const CURLOPT_SSL_CTX_FUNCTION           : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_CTX_DATA.html */
extern const CURLOPT_SSL_CTX_DATA               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_CREATE_MISSING_DIRS.html */
extern const CURLOPT_FTP_CREATE_MISSING_DIRS    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYAUTH.html */
extern const CURLOPT_PROXYAUTH                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_RESPONSE_TIMEOUT.html */
extern const CURLOPT_FTP_RESPONSE_TIMEOUT       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_IPRESOLVE.html */
extern const CURLOPT_IPRESOLVE                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAXFILESIZE.html */
extern const CURLOPT_MAXFILESIZE                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_INFILESIZE_LARGE.html */
extern const CURLOPT_INFILESIZE_LARGE           : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_RESUME_FROM_LARGE.html */
extern const CURLOPT_RESUME_FROM_LARGE          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAXFILESIZE_LARGE.html */
extern const CURLOPT_MAXFILESIZE_LARGE          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NETRC_FILE.html */
extern const CURLOPT_NETRC_FILE                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_USE_SSL.html */
extern const CURLOPT_USE_SSL                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDSIZE_LARGE.html */
extern const CURLOPT_POSTFIELDSIZE_LARGE        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TCP_NODELAY.html */
extern const CURLOPT_TCP_NODELAY                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTPSSLAUTH.html */
extern const CURLOPT_FTPSSLAUTH                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_IOCTLFUNCTION.html */
extern const CURLOPT_IOCTLFUNCTION              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_IOCTLDATA.html */
extern const CURLOPT_IOCTLDATA                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_ACCOUNT.html */
extern const CURLOPT_FTP_ACCOUNT                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COOKIELIST.html */
extern const CURLOPT_COOKIELIST                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_IGNORE_CONTENT_LENGTH.html */
extern const CURLOPT_IGNORE_CONTENT_LENGTH      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_SKIP_PASV_IP.html */
extern const CURLOPT_FTP_SKIP_PASV_IP           : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_FILEMETHOD.html */
extern const CURLOPT_FTP_FILEMETHOD             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_LOCALPORT.html */
extern const CURLOPT_LOCALPORT                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_LOCALPORTRANGE.html */
extern const CURLOPT_LOCALPORTRANGE             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONNECT_ONLY.html */
extern const CURLOPT_CONNECT_ONLY               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONV_FROM_NETWORK_FUNCTION.html */
extern const CURLOPT_CONV_FROM_NETWORK_FUNCTION : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONV_TO_NETWORK_FUNCTION.html */
extern const CURLOPT_CONV_TO_NETWORK_FUNCTION   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONV_FROM_UTF8_FUNCTION.html */
extern const CURLOPT_CONV_FROM_UTF8_FUNCTION    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAX_SEND_SPEED_LARGE.html */
extern const CURLOPT_MAX_SEND_SPEED_LARGE       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAX_RECV_SPEED_LARGE.html */
extern const CURLOPT_MAX_RECV_SPEED_LARGE       : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_ALTERNATIVE_TO_USER.html */
extern const CURLOPT_FTP_ALTERNATIVE_TO_USER    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SOCKOPTFUNCTION.html */
extern const CURLOPT_SOCKOPTFUNCTION            : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SOCKOPTDATA.html */
extern const CURLOPT_SOCKOPTDATA                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSL_SESSIONID_CACHE.html */
extern const CURLOPT_SSL_SESSIONID_CACHE        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSH_AUTH_TYPES.html */
extern const CURLOPT_SSH_AUTH_TYPES             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSH_PUBLIC_KEYFILE.html */
extern const CURLOPT_SSH_PUBLIC_KEYFILE         : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSH_PRIVATE_KEYFILE.html */
extern const CURLOPT_SSH_PRIVATE_KEYFILE        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_FTP_SSL_CCC.html */
extern const CURLOPT_FTP_SSL_CCC                : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TIMEOUT_MS.html */
extern const CURLOPT_TIMEOUT_MS                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CONNECTTIMEOUT_MS.html */
extern const CURLOPT_CONNECTTIMEOUT_MS          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTP_TRANSFER_DECODING.html */
extern const CURLOPT_HTTP_TRANSFER_DECODING     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_HTTP_CONTENT_DECODING.html */
extern const CURLOPT_HTTP_CONTENT_DECODING      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NEW_FILE_PERMS.html */
extern const CURLOPT_NEW_FILE_PERMS             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NEW_DIRECTORY_PERMS.html */
extern const CURLOPT_NEW_DIRECTORY_PERMS        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_POSTREDIR.html */
extern const CURLOPT_POSTREDIR                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SSH_HOST_PUBLIC_KEY_MD5.html */
extern const CURLOPT_SSH_HOST_PUBLIC_KEY_MD5    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_OPENSOCKETFUNCTION.html */
extern const CURLOPT_OPENSOCKETFUNCTION         : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_OPENSOCKETDATA.html */
extern const CURLOPT_OPENSOCKETDATA             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_COPYPOSTFIELDS.html */
extern const CURLOPT_COPYPOSTFIELDS             : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_TRANSFER_MODE.html */
extern const CURLOPT_PROXY_TRANSFER_MODE        : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SEEKFUNCTION.html */
extern const CURLOPT_SEEKFUNCTION               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html */
extern const CURLOPT_WRITEFUNCTION              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_WRITEDATA.html */
extern const CURLOPT_WRITEDATA                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NOBODY.html */
extern const CURLOPT_NOBODY                     : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SEEKDATA.html */
extern const CURLOPT_SEEKDATA                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CRLFILE.html */
extern const CURLOPT_CRLFILE                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_ISSUERCERT.html */
extern const CURLOPT_ISSUERCERT                 : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_ADDRESS_SCOPE.html */
extern const CURLOPT_ADDRESS_SCOPE              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_CERTINFO.html */
extern const CURLOPT_CERTINFO                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_USERNAME.html */
extern const CURLOPT_USERNAME                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PASSWORD.html */
extern const CURLOPT_PASSWORD                   : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYUSERNAME.html */
extern const CURLOPT_PROXYUSERNAME              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROXYPASSWORD.html */
extern const CURLOPT_PROXYPASSWORD              : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_NOPROXY.html */
extern const CURLOPT_NOPROXY                    : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_TFTP_BLKSIZE.html */
extern const CURLOPT_TFTP_BLKSIZE               : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SOCKS5_GSSAPI_SERVICE.html */
extern const CURLOPT_SOCKS5_GSSAPI_SERVICE      : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_SOCKS5_GSSAPI_NEC.html */
extern const CURLOPT_SOCKS5_GSSAPI_NEC          : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_PROTOCOLS.html */
extern const CURLOPT_PROTOCOLS                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_REDIR_PROTOCOLS.html */
extern const CURLOPT_REDIR_PROTOCOLS            : c_int ;
/* All other curlopt values will be less than this one */
extern const CURLOPT_LASTENTRY                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAIL_FROM.html */
extern const CURLOPT_MAIL_FROM                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAIL_RCPT.html */
extern const CURLOPT_MAIL_RCPT                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLOPT_MAIL_AUTH.html */
extern const CURLOPT_MAIL_AUTH                  : c_int ;
/* See https://curl.haxx.se/libcurl/c/CURLINFO_CONTENT_LENGTH_DOWNLOAD.html */
extern const CURLINFO_CONTENT_LENGTH_DOWNLOAD   : c_int ;

// extern QIO functions
private extern proc sys_iov_total_bytes(iov:c_ptr(qiovec_t), iovcnt:c_int):int(64);
private extern proc qio_strdup(s: c_string): c_string;
private extern proc qio_mkerror_errno():syserr;
private extern proc qio_int_to_err(a:int(32)):syserr;
private extern proc qio_channel_nbytes_available_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_copy_to_available_unlocked(ch:qio_channel_ptr_t, ptr:c_void_ptr, len:ssize_t):syserr;
private extern proc qio_channel_nbytes_write_behind_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_copy_from_buffered_unlocked(ch:qio_channel_ptr_t, ptr:c_void_ptr, len:ssize_t, ref n_written_out:ssize_t):syserr;
private extern proc qio_channel_end_offset_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_offset_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_writable(ch:qio_channel_ptr_t):bool;

extern const CURLE_RANGE_ERROR: c_int;
extern const CURLE_OK: c_int;
extern const CURLM_OK: c_int;
extern const CURLOPTTYPE_OBJECTPOINT: c_int;
extern const CURLOPTTYPE_OFF_T: c_int;

// extern Curl types
extern type CURL;
extern type CURLM;
extern type curl_slist;

extern type CURLoption=c_int;
extern type CURLcode=c_int;
extern type CURLMcode=c_int;
extern type CURLINFO=c_int;
extern type curl_off_t=int(64);

// extern curl functions
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
extern const CURL_READFUNC_ABORT:size_t;
extern const CURL_WRITEFUNC_PAUSE:size_t;
extern const CURLPAUSE_ALL: c_int;
extern const CURLPAUSE_CONT: c_int;

// Since a curl handle does not hold where it has read to, we need to do
// this here.
// As well, since we can many times request byte-ranges (for HTTP/HTTPS)
// we keep track of that here as well.

class CurlFile : QioPluginFile {

  var url_c: c_string;     // Path/URL
  var length: ssize_t;    // length of what we are reading, -1 if we can't get

  var seekable: bool;

  override proc setupChannel(out pluginChannel:unmanaged QioPluginChannel,
                      start:int(64),
                      end:int(64),
                      qioChannelPtr:qio_channel_ptr_t):syserr {
    var curlch = new unmanaged CurlChannel();
    curlch.curlf = this;
    curlch.qio_ch = qioChannelPtr;
    pluginChannel = curlch;
    return start_channel(curlch, start, end);
  }

  override proc filelength(out length:int(64)):syserr {
    if this.length == -1 then {
      return ENOTSUP;
    }

    length = this.length;
    return ENOERR;
  }
  override proc getpath(out path:c_string, out len:ssize_t):syserr {
    path = qio_strdup(this.url_c);
    len = url_c.length;
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
    c_free(url_c:c_void_ptr);
    url_c = nil;
    return ENOERR;
  }
}

class CurlChannel : QioPluginChannel {
  var curlf: CurlFile;
  var qio_ch:qio_channel_ptr_t;
  var curl: c_ptr(CURL);  // Curl handle
  var curlm: c_ptr(CURLM);  // Curl multi handle
  var running_handles: c_int;
  var have_channel_lock:bool;
  var saved_error:syserr = ENOERR;

  override proc readAtLeast(amt:int(64)):syserr {
    return read_atleast(this, amt);
  }
  override proc write(amt:int(64)):syserr {
    return write_amount(this, amt);
  }

  override proc close():syserr {
    curl_multi_remove_handle(curlm, curl);
    curl_easy_cleanup(curl);
    curl_multi_cleanup(curlm);
    return ENOERR;
  }
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
private proc pause_writer(ptr:c_void_ptr, size:size_t, nmemb:size_t, userdata:c_void_ptr):size_t
{
  //writeln("in pause_writer");
  return CURL_WRITEFUNC_PAUSE;
}
private proc pause_reader(ptr:c_void_ptr, size:size_t, nmemb:size_t, userdata:c_void_ptr):size_t
{
  //writeln("in pause_reader");
  return CURL_READFUNC_PAUSE;
}


private proc buf_writer(ptr:c_void_ptr, size:size_t, nmemb:size_t, userdata:c_void_ptr):size_t
{
  var ptr_data = ptr:c_ptr(uint(8));
  var realsize:size_t = size*nmemb;
  var real_realsize:size_t = realsize;
  var retptr = userdata:c_ptr(curl_iovec_t);
  ref ret = retptr.deref();

  if size > sys_iov_total_bytes(ret.vec, ret.count) {
    //writeln("in buf_writer PAUSE");
    return CURL_WRITEFUNC_PAUSE;
  }

  //writeln("in buf_writer");

  // so that we can "seek" when we cannot request byteranges
  if realsize < ret.offset {
    ret.offset -= realsize;
    //writeln("A: ", realsize);
    return realsize;
  } else {
    // nop when we are done -- non-zero the first time through
    ptr_data = ptr_data + ret.offset;
    realsize -= ret.offset;
    ret.offset = 0;
    //writeln("B: ", realsize);
  }

  // The amount that we have been given by curl is more than we can stick into a
  // single iovbuf. So we need to go from one iovbuf to the other
  while (realsize > ret.vec[ret.curr].iov_len - ret.amt_read) {
    //writeln("C: ", realsize);
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
    //writeln("D: ", realsize);
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
  //writeln("E: ", real_realsize);
  return real_realsize;
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

  if startsWith(fl.url_c, "http://") || startsWith(fl.url_c, "https://") {
    // We're on HTTP/HTTPS so we should look for byte ranges to see if we
    // can request them

    // The size doesn't really matter, we just want a place on the heap. This
    // will get expanded in curl_write_string.

    // Headers tend to be ~800, although they can grow much larger than this. If
    // it is larger than this, we'll take care of it in chpl_curl_write_string.

    buf.mem = c_calloc(uint(8), 800);
    buf.len = 0;
    buf.alloced = 800;

    var curl:c_ptr(CURL);

    curl = curl_easy_init();
    chpl_curl_easy_setopt_ptr(curl, CURLOPT_URL, fl.url_c:c_void_ptr);
    chpl_curl_easy_setopt_ptr(curl, CURLOPT_WRITEFUNCTION, c_ptrTo(chpl_curl_write_string):c_void_ptr);
    chpl_curl_easy_setopt_ptr(curl, CURLOPT_HEADERDATA, c_ptrTo(buf));
    chpl_curl_easy_setopt_long(curl, CURLOPT_NOBODY, 1);

    curl_easy_perform(curl);

    extern proc strstr(haystack:c_string, needle:c_string):c_string;
    // Does this URL accept range requests?
    if strstr(buf.mem:c_string, c"Accept-Ranges: bytes"):c_void_ptr == nil:c_void_ptr {
      ret = false;
    } else {
      ret = true;
    }

    c_free(buf.mem);

    var lengthDouble: real(64);
    // Get the content length (for HTTP only)
    chpl_curl_easy_getinfo_ptr(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, c_ptrTo(lengthDouble));
    length = lengthDouble: int(64);
    // One day, use CURLINFO_CONTENT_LENGTH_DOWNLOAD_T

    curl_easy_cleanup(curl);
  }

  // FTP always supports CURLOPT_RESUME_FROM_LARGE
  if startsWith(fl.url_c, "ftp://") || startsWith(fl.url_c, "sftp://") then
    ret = true;

  return ret;
}


private proc start_channel(cc:CurlChannel,
                           start:int(64),
                           end:int(64)): syserr {

  //writeln("start_channel");

  var curl = curl_easy_init();
  var curlm = curl_multi_init();
  cc.curl = curl;
  cc.curlm = curlm;

  if curl == nil then
    return ENOMEM;
  if curlm == nil then
    return ENOMEM;

  var err:CURLcode = 0;
  var merr:CURLMcode = 0;
  merr = curl_multi_add_handle(curlm, curl);
  if merr != 0 then
    return ENOMEM;

  // Setopt with the url
  chpl_curl_easy_setopt_ptr(curl, CURLOPT_URL, cc.curlf.url_c:c_void_ptr);

  var writer = qio_channel_writable(cc.qio_ch);

  if writer {
    //writeln("Setting up upload");
    // Set the function to get the data to send
    err = chpl_curl_easy_setopt_long(curl, CURLOPT_UPLOAD, 1);
    if err then return EINVAL;
    err =chpl_curl_easy_setopt_ptr(curl, CURLOPT_READFUNCTION, c_ptrTo(chpl_curl_read_buffered):c_void_ptr);
    if err then return EINVAL;
    err = chpl_curl_easy_setopt_ptr(curl, CURLOPT_READDATA, cc:c_void_ptr);
    if err then return EINVAL;

    // TODO -- is this necessary?
    //err = chpl_curl_easy_setopt_offset(curl, CURLOPT_INFILESIZE_LARGE, 14);
    //if err then return EINVAL;
  } else {
    //writeln("Setting up download");
    // Set the function to process the received data
    err = chpl_curl_easy_setopt_ptr(curl, CURLOPT_WRITEFUNCTION, c_ptrTo(chpl_curl_write_received):c_void_ptr);
    if err then return EINVAL;
    err = chpl_curl_easy_setopt_ptr(curl, CURLOPT_WRITEDATA, cc:c_void_ptr);
    if err then return EINVAL;
  }
  // If it's seekable, start at the right offset
  if cc.curlf.seekable {  // we can request byteranges
    err = chpl_curl_easy_setopt_offset(curl, CURLOPT_RESUME_FROM_LARGE, start);
    if err then return EINVAL;
  } else {
    if start != 0 then
      return EINVAL;
  }

  /** TODO -- removed this for setopt reasons
  if writer == false {
    // Start some action by calling multi_perform right away
    cc.have_channel_lock = true;
    merr = curl_multi_perform(curlm, cc.running_handles);
    if merr != 0 then
      return ENOSYS;
    // TODO: temporarily try waiting here

    // From here on, we'll wait for data in the reader function
    cc.have_channel_lock = false;
  }*/

  //writeln("finished start_channel");

  return ENOERR;
}

private proc chpl_curl_write_received(contents: c_void_ptr, size:size_t, nmemb:size_t, userp: c_void_ptr):size_t {
  var realsize:size_t = size * nmemb;
  var cc = userp:CurlChannel;
  var err:syserr = ENOERR;

  // lock the channel if it's not already locked
  assert(cc.have_channel_lock);

  var amt = realsize.safeCast(int(64));

  //writeln("chpl_curl_write_received offset=", qio_channel_offset_unlocked(cc.qio_ch), " len=", amt);

  // make sure the channel has room in the buffer for the data
  // copy the data to the channel's buffer
  err = qio_channel_copy_to_available_unlocked(cc.qio_ch, contents, amt);

  // unlock the channel if we locked it

  if err != ENOERR {
    cc.saved_error = err;
    return 0;
  }

  return realsize;
}


private proc read_atleast(cc:CurlChannel, requestedAmount:int(64)):syserr {
  // mark the channel as already locked
  cc.have_channel_lock = true;
  defer {
    // mark the channel as not locked
    cc.have_channel_lock = false;
  }

  var ch:qio_channel_ptr_t = cc.qio_ch;
  var curl = cc.curl;
  var curlm = cc.curlm;
  var mcode: CURLMcode;
  var serr:syserr = ENOERR;
  var fdread: fd_set;
  var fdwrite: fd_set;
  var fdexcept: fd_set;
  var maxfd:c_int = -1;
  var space:int(64);

  var amt:int(64) = requestedAmount;
  var max_amt:int(64) = amt;

  max_amt = qio_channel_end_offset_unlocked(cc.qio_ch) -
            qio_channel_offset_unlocked(cc.qio_ch);
  if max_amt < 0 then
    max_amt = 0;
  if amt > max_amt then
    amt = max_amt;

  cc.saved_error = 0;

  //writeln("performing 1");
  mcode = curl_multi_perform(curlm, cc.running_handles);
  if mcode != CURLM_OK then
    return EINVAL; // or something...

  // loop
  //   wait for some activity on the channel
  //   check if the channel has read enough data, stop if it has
  while cc.running_handles > 0 {
    // check for activity on something
    maxfd = -1;
    sys_fd_zero(fdread);
    sys_fd_zero(fdwrite);
    sys_fd_zero(fdexcept);

    // Compute the timout curl recommends
    var timeoutMillis:c_long = 1;
    mcode = curl_multi_timeout(curlm, timeoutMillis);
    if mcode != CURLM_OK then
      return EINVAL; // or something...
    if timeoutMillis < 0 then
      timeoutMillis = 0;

    var timeout:timeval;
    timeout.tv_sec = timeoutMillis / 1000;
    timeout.tv_usec = (timeoutMillis % 1000) * 1000;

    mcode = curl_multi_fdset(curlm, c_ptrTo(fdread), c_ptrTo(fdwrite), c_ptrTo(fdexcept), maxfd);
    if mcode != CURLM_OK then
      return EINVAL; // or something...

    if maxfd == -1 {
      // we can't wait with sockets for some reason, so wait for operation
      var waitMillis = 100;
      if 0 < timeoutMillis && timeoutMillis < waitMillis then
        waitMillis = timeoutMillis;

      //writeln("sleeping ", waitMillis);
      var waitSeconds = waitMillis:real / 1000.0;
      Time.sleep(waitSeconds);
    } else {
      //writeln("selecting ", timeoutMillis);
      var nset:c_int;
      serr = Sys.sys_select(maxfd+1, c_ptrTo(fdread), c_ptrTo(fdwrite), c_ptrTo(fdexcept), c_ptrTo(timeout), nset);
      if serr != 0 then
        return serr;
    }

    //writeln("performing 2");
    mcode = curl_multi_perform(curlm, cc.running_handles);
    if mcode != CURLM_OK then
      return EINVAL; // or something...

    // stop if we have read enough data
    space = qio_channel_nbytes_available_unlocked(ch);
    if space >= amt then
      break;

    // stop if there was an error saving the data to the buffer
    if cc.saved_error != ENOERR then
      return cc.saved_error;
  }

  // Return EEOF if the connection is no longer running
  space = qio_channel_nbytes_available_unlocked(ch);
  if cc.running_handles == 0 && space < amt then
    return EEOF;

  return ENOERR;
}

// Send some data somewhere with curl
// Returning 0 will signal end-of-file to the curl library
// and cause it to stop the transfer.
private proc chpl_curl_read_buffered(contents: c_void_ptr, size:size_t, nmemb:size_t, userp: c_void_ptr):size_t {
  var realsize:size_t = size * nmemb;
  var cc = userp:CurlChannel;
  var err:syserr = ENOERR;

  // lock the channel if it's not already locked
  assert(cc.have_channel_lock);

  var amt = realsize.safeCast(ssize_t);

  // Write from the buffer's start position up until the start
  // of the user-visible data.
  /*{
    var space = qio_channel_nbytes_write_behind_unlocked(cc.qio_ch);
    writeln("chpl_curl_read_buffered initiating ", realsize, " space=", space);
  }*/


  var gotamt: ssize_t = 0;
  // copy the data from the channel's buffer
  err = qio_channel_copy_from_buffered_unlocked(cc.qio_ch, contents, amt, gotamt);

  /*{
    var space = qio_channel_nbytes_write_behind_unlocked(cc.qio_ch);
    writeln("chpl_curl_read_buffered returning ", gotamt, " space=", space);
  }*/

  // unlock the channel if we locked it

  // If there was an error from the channel, abort the connection
  if err != ENOERR {
    cc.saved_error = err;
    return CURL_READFUNC_ABORT;
  }
  // If the channel is not closed, but we would
  // otherwise return 0, pause the connection, so that
  // the connection is not clossed until the channel is.
  if gotamt == 0 && ! qio_channel_isclosed(0, cc.qio_ch) {
    return CURL_READFUNC_PAUSE;
  }

  return gotamt:size_t;
}

private proc write_amount(cc:CurlChannel, requestedAmount:int(64)):syserr {
  // mark the channel as already locked
  cc.have_channel_lock = true;
  defer {
    // mark the channel as not locked
    cc.have_channel_lock = false;
  }

  //writeln("in write_amount");

  var ch:qio_channel_ptr_t = cc.qio_ch;
  var curl = cc.curl;
  var curlm = cc.curlm;
  var ccode: CURLcode;
  var mcode: CURLMcode;
  var serr:syserr = ENOERR;
  var fdread: fd_set;
  var fdwrite: fd_set;
  var fdexcept: fd_set;
  var maxfd:c_int = -1;
  var space:int(64);
  var starting_space:int(64);
  var target_space:int(64);

  starting_space = qio_channel_nbytes_write_behind_unlocked(ch);

  target_space = starting_space - requestedAmount;

  cc.saved_error = 0;

  //writeln("performing 3");
  mcode = curl_multi_perform(curlm, cc.running_handles);
  if mcode != CURLM_OK then
    return EINVAL; // or something...

  // loop
  //   wait for some activity on the channel
  //   check if the channel has written enough data, stop if it has
  while cc.running_handles > 0 {
    // check for activity on something
    maxfd = -1;
    sys_fd_zero(fdread);
    sys_fd_zero(fdwrite);
    sys_fd_zero(fdexcept);

    // Continue anything that is paused
    ccode = curl_easy_pause(cc.curl, CURLPAUSE_CONT);
    if ccode != CURLE_OK then
      return EINVAL; // or something...

    // Compute the timout curl recommends
    var timeoutMillis:c_long = 1;
    mcode = curl_multi_timeout(curlm, timeoutMillis);
    if mcode != CURLM_OK then
      return EINVAL; // or something...
    if timeoutMillis < 0 then
      timeoutMillis = 0;

    var timeout:timeval;
    timeout.tv_sec = timeoutMillis / 1000;
    timeout.tv_usec = (timeoutMillis % 1000) * 1000;

    mcode = curl_multi_fdset(curlm, c_ptrTo(fdread), c_ptrTo(fdwrite), c_ptrTo(fdexcept), maxfd);
    if mcode != CURLM_OK then
      return EINVAL; // or something...

    if maxfd == -1 {
      // we can't wait with sockets for some reason, so wait for operation
      var waitMillis = 100;
      if 0 < timeoutMillis && timeoutMillis < waitMillis then
        waitMillis = timeoutMillis;

      //writeln("sleeping ", waitMillis);
      var waitSeconds = waitMillis:real / 1000.0;
      Time.sleep(waitSeconds);
    } else {
      //writeln("selecting ", timeoutMillis);
      var nset:c_int;
      serr = Sys.sys_select(maxfd+1, c_ptrTo(fdread), c_ptrTo(fdwrite), c_ptrTo(fdexcept), c_ptrTo(timeout), nset);
      if serr != 0 then
        return serr;
    }

    /*{
      space = qio_channel_nbytes_write_behind_unlocked(ch);
      writeln("performing 4 offset=", qio_channel_offset_unlocked(ch),
              " writebehind=", space);
    }*/

    mcode = curl_multi_perform(curlm, cc.running_handles);
    if mcode != CURLM_OK then
      return EINVAL; // or something...

    // stop if we have read enough data
    space = qio_channel_nbytes_write_behind_unlocked(ch);
    if space <= target_space then
      break;

    // stop if there was an error saving the data to the buffer
    if cc.saved_error != ENOERR then
      return cc.saved_error;
  }

  // Return EEOF if the connection is no longer running
  space = qio_channel_nbytes_write_behind_unlocked(ch);
  if cc.running_handles == 0 && space > target_space {
    writeln("RETURNING EOF");
    return EEOF;
  }

  return ENOERR;
}


private proc openUrlFile(url:string,
                         mode:iomode = iomode.r,
                         style:iostyle = defaultIOStyle()) throws {

  var err_out: syserr = ENOERR;
  var rc = 0;
  var filelength: int(64);

  var fl = new unmanaged CurlFile();

  // TODO:
  // future
  // support POST
  // e.g.
  // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");

  // Save the url requested
  var url_c = c_calloc(uint(8), url.length+1);
  c_memcpy(url_c:c_void_ptr, url.localize().c_str():c_void_ptr, url.length);

  fl.url_c = url_c:c_string;

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

  var ret: file;

  try {
    ret = openplugin(fl, mode, fl.seekable, style);
  } catch e {
    fl.close();
    delete fl;
    throw e;
  }

  return ret;
}

proc downloadUrl(url:string,
                 param kind=iokind.dynamic, param locking=true,
                 start:int(64) = 0, end:int(64) = max(int(64)),
                 style:iostyle = defaultIOStyle())
                : channel(false, kind, locking) throws {
  var f = openUrlFile(url, iomode.r, style);
  return f.reader(kind=kind, locking=locking,
                  start=start, end=end);
}

proc uploadUrl(url:string,
               param kind=iokind.dynamic, param locking=true,
               start:int(64) = 0, end:int(64) = max(int(64)),
               style:iostyle = defaultIOStyle())
              : channel(true, kind, locking) throws {
  var f = openUrlFile(url, iomode.cw, style);
  return f.writer(kind=kind, locking=locking,
                  start=start, end=end);
}

} /* end of module */
