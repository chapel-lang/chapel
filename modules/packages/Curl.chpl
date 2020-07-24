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
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*

Low-level support for many network protocols with libcurl

This module provides support for libcurl, enabling Chapel programs
to work with many network protocols. This module is a low-level C
wrapper. For a simpler interface not specific to Curl, please see
:mod:`URL`.

The `curl homepage <https://curl.haxx.se/libcurl/>`_ describes libcurl thus::

 libcurl is a free and easy-to-use client-side URL transfer library, supporting
 DICT, FILE, FTP, FTPS, Gopher, HTTP, HTTPS, IMAP, IMAPS, LDAP, LDAPS, POP3,
 POP3S, RTMP, RTSP, SCP, SFTP, SMTP, SMTPS, Telnet and TFTP.  libcurl supports
 SSL certificates, HTTP POST, HTTP PUT, FTP uploading, HTTP form based upload,
 proxies, cookies, user+password authentication (Basic, Digest, NTLM,
 Negotiate, Kerberos), file transfer resume, http proxy tunneling and more!

Dependencies
------------

The Curl module in Chapel is dependent on libcurl being installed and available
to Chapel programs.

Using Curl Support in Chapel
----------------------------

Simple uses of Curl work through the generic :mod:`URL` module. This module
allows a URL to be opened as a :record:`IO.channel`.

.. code-block:: chapel

  use URL;
  var urlreader = openUrlReader("http://example.com");
  var str:bytes;
  // Output each line read from the URL to stdout
  while(urlreader.readline(str)) {
    write(str);
  }

The Curl module includes ``require`` statements to include the Curl header and
library, so the above example can be compiled simply with:

.. code-block:: sh

  chpl curl-example.chpl


More complex uses of Curl can make use of the extern types provided in this
module. In that event, please see the libcurl documentation for how to use
these functions. Note that it is possible to use :proc:`setopt` to adjust the
settings for a the result of :proc:`URL.openUrlReader` or
:proc:`URL.openUrlWriter` before starting the connection.

Many times when we are connecting to a URL (FTP, IMAP, SMTP, HTTP) we have to
give extra information to the Curl handle. This is done via the setopt()
interface.  Documentation on the various options, as well as the functions
that are referenced below can be found `here
<https://curl.haxx.se/libcurl/c/libcurl-easy.html>`_

Due to the large number of constants in the Curl interface, and the fact that
these constants vary for different versions of Curl, this module does not
provide extern declarations for all constants. However these are trivial to add
to your own programs. For example, the below example declares
``CURLOPT_VERBOSE`` as a ``CURLoption`` like this:

.. code-block:: chapel

  extern const CURLOPT_VERBOSE:CURLoption;

Here is a full program enabling verbose output from Curl while downloading:

.. code-block:: chapel

  // This example changes the Curl options before connecting
  use URL;
  use Curl;
  var reader = openUrlReader("https://example.com");
  var str:bytes;
  // Set verbose output from curl
  extern const CURLOPT_VERBOSE:CURLoption;
  Curl.setopt(reader, CURLOPT_VERBOSE, true);

  // now read into the bytes
  reader.readbytes(str);
  writeln(str);
  reader.close();


Curl Support Types and Functions
--------------------------------

 */
module Curl {
  public use IO, SysCTypes;
  use Sys;

  require "curl/curl.h";
  require "-lcurl";


  /* Returns the ``CURL`` handle connected to a channel opened with
     :proc:`URL.openUrlReader` or :proc:`URL.openUrlWriter`.
   */
  proc getCurlHandle(ch:channel):c_ptr(CURL) throws {
    use CurlQioIntegration;

    if ch.home != here {
      throw SystemError.fromSyserr(EINVAL, "getCurlHandle only functions with local channels");
    }

    var plugin = ch.channelPlugin():CurlChannel?;
    if plugin == nil then
      throw SystemError.fromSyserr(EINVAL, "getCurlHandle called on a non-curl channel");

    var curl = plugin!.curl;
    return curl;
  }

  /* This function is the equivalent to the
     `curl_easy_setopt <https://curl.haxx.se/libcurl/c/curl_easy_setopt.html>`_
     function in libcurl. It sets information on the curl file handle
     attached to a channel that can change libcurl's behavior.

     :arg ch: a curl channel created with openUrlReader or openUrlWriter
     :arg opt: the curl option to set.
     :arg arg: the value to set the curl option specified by opt.
     :type arg: `int`, `string`, `bool`, or `slist`
  */
  proc setopt(ch:channel, opt:c_int, arg):bool throws {
    use CurlQioIntegration;

    var err:syserr = ENOERR;

    if (arg.type == slist) && (slist.home != ch.home) {
      throw SystemError.fromSyserr(EINVAL, "in channel.setopt(): slist, and curl handle do not reside on the same locale");
    }

    // Invalid argument type for option if the below conditionals
    // don't handle it.
    err = EINVAL;

    on ch.home {
      var plugin = ch.channelPlugin():CurlChannel?;
      if plugin == nil then
        throw SystemError.fromSyserr(EINVAL, "in channel.setopt(): not a curl channel");

      var curl = plugin!.curl;

      // This reasoning is pulled from the libcurl source
      if (opt < CURLOPTTYPE_OBJECTPOINT) {
        if isIntegralType(arg.type) || isBoolType(arg.type) {
          var tmp:c_long = arg:c_long;
          err = qio_int_to_err(curl_easy_setopt_long(curl, opt:CURLoption, tmp));
        }
      } else if (opt < CURLOPTTYPE_OFF_T) {
        if isAnyCPtr(arg.type) {
          var tmp:c_void_ptr = arg:c_void_ptr;
          err = qio_int_to_err(curl_easy_setopt_ptr(curl, opt:CURLoption, tmp));
        } else if arg.type == slist {
          var tmp:c_void_ptr = arg.list:c_void_ptr;
          err = qio_int_to_err(curl_easy_setopt_ptr(curl, opt:CURLoption, tmp));
        }
      } else {
        if isIntegralType(arg.type) {
          var tmp:curl_off_t = arg:curl_off_t;
          err = qio_int_to_err(curl_easy_setopt_offset(curl, opt:CURLoption, tmp));
        }
      }
    }

    if err then try ioerror(err, "in file.setopt(opt:c_int, arg)");
    return true;
  }

  /* Set curl options on a curl file attached to a channel.

     For example, you might do:

     .. code-block:: chapel

       extern const CURLOPT_USERNAME:CURLoption;
       extern const CURLOPT_PASSWORD:CURLoption;

       curlfile.setopt((CURLOPT_USERNAME, username),
                       (CURLOPT_PASSWORD, password));

     :arg args: any number of tuples of the form (curl_option, value).
                This function will call ``setopt`` on each pair in turn.
   */
  proc setopt(ch:channel, args ...?k) throws {
    for param i in 0..k-1 {
      setopt(ch, args(i)(0), args(i)(1));
    }
  }

  /*
     A linked list of strings used in many curl setopt calls. This type
     corresponds to the libcurl type curl_slist.

  .. note::

     Memory in the list is not automatically managed. It is necessary
     to call :proc:`slist.free` to free the slist when it is no longer used.

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
     this slist when it is freed.
   */
  proc slist.free() {
    on this.home {
      curl_slist_free_all(this.list);
    }
  }

  // These are meant to be used with the file.setopt() function. This way, a user
  // has access to the easy interface.


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

  // Curl Constants
  /* Successful result for CURL easy API calls */
  extern const CURLE_OK: c_int;
  /* Successful result for CURL multi API calls */
  extern const CURLM_OK: c_int;

  // These constants are used for type checking curl_easy_setopt 
  private extern const CURLOPTTYPE_OBJECTPOINT: c_int;
  private extern const CURLOPTTYPE_OFF_T: c_int;

  // These constants are used within the implementation of Curl-QIO integration.
  // Users wishing to use other CURL setopt options should write their
  // own extern declarations.
  private extern const CURLOPT_URL: CURLoption;
  private extern const CURLOPT_HEADERDATA: CURLoption;
  private extern const CURLOPT_WRITEFUNCTION: CURLoption;
  private extern const CURLOPT_WRITEDATA: CURLoption;
  private extern const CURLOPT_RESUME_FROM_LARGE: CURLoption;
  private extern const CURLOPT_NOBODY: CURLoption;
  private extern const CURLOPT_UPLOAD: CURLoption;
  private extern const CURLOPT_READFUNCTION: CURLoption;
  private extern const CURLOPT_READDATA: CURLoption;

  // Other Curl constants
  private extern const CURLINFO_CONTENT_LENGTH_DOWNLOAD: c_int;

  private extern const CURL_READFUNC_PAUSE:size_t;
  private extern const CURL_READFUNC_ABORT:size_t;
  private extern const CURL_WRITEFUNC_PAUSE:size_t;
  private extern const CURLPAUSE_ALL: c_int;
  private extern const CURLPAUSE_CONT: c_int;

  // extern Curl types

  /* A CURL easy handle. Most CURL functions accept a ``c_ptr(CURL)``. */
  extern type CURL;
  /* A CURL multi handle. */
  extern type CURLM;
  /* A CURL string list */
  extern "struct curl_slist" record curl_slist { }

  /* CURLoption identifies options for ``curl_easy_setopt``.
   */
  extern type CURLoption=c_int;
  /* The return type of CURL easy API functions */ 
  extern type CURLcode=c_int;
  /* The return type of CURL multi API functions */ 
  extern type CURLMcode=c_int;
  /* CURLINFO identifies info to get with ``curl_easy_getinfo`` */
  extern type CURLINFO=c_int;
  /* curl_off_t is a file offset used by the CURL library */
  extern type curl_off_t=int(64);

  // extern curl functions

  /* See https://curl.haxx.se/libcurl/c/curl_easy_init.html */ 
  extern proc curl_easy_init():c_ptr(CURL);

  // setopt and getinfo are actual varargs functions in C that
  // can accept any type.

  /* See https://curl.haxx.se/libcurl/c/curl_easy_getinfo.html */
  extern proc curl_easy_getinfo(handle:c_ptr(CURL), info:CURLINFO, arg):CURLcode;
  /* See https://curl.haxx.se/libcurl/c/curl_easy_setopt.html */
  extern proc curl_easy_setopt(handle:c_ptr(CURL), option:CURLoption, arg):CURLcode;

  // setopt helpers for specific types
  /* Helper function for ``curl_easy_setopt`` when passing a numeric argument */
  proc curl_easy_setopt_long(curl:c_ptr(CURL), option:CURLoption, arg:c_long):CURLcode {
    return curl_easy_setopt(curl, option, arg);
  }

  /* Helper function for ``curl_easy_setopt`` when passing a pointer argument */
  proc curl_easy_setopt_ptr(curl:c_ptr(CURL), option:CURLoption,
      arg:c_void_ptr):CURLcode {
    return curl_easy_setopt(curl, option, arg);
  }

  /* Helper function for ``curl_easy_setopt`` when passing an offset argument */
  proc curl_easy_setopt_offset(curl:c_ptr(CURL), option:CURLoption, offset:int(64)):CURLcode {
    var tmp:curl_off_t = offset;
    return curl_easy_setopt(curl, option, tmp);
  }

  /* Helper function for ``curl_easy_getinfo`` when passing a pointer argument.
     Generally this is a pointer to the value to be set. */
  proc curl_easy_getinfo_ptr(curl:c_ptr(CURL), info:CURLINFO, arg:c_void_ptr):CURLcode {
    return curl_easy_getinfo(curl, info, arg);
  }

  /* See https://curl.haxx.se/libcurl/c/curl_easy_perform.html */
  extern proc curl_easy_perform(curl:c_ptr(CURL)):CURLcode;
  /* See https://curl.haxx.se/libcurl/c/curl_easy_cleanup.html */
  extern proc curl_easy_cleanup(curl:c_ptr(CURL)):void;
  /* See https://curl.haxx.se/libcurl/c/curl_easy_pause.html */
  extern proc curl_easy_pause(curl:c_ptr(CURL), bitmask:c_int):CURLcode;

  /* See https://curl.haxx.se/libcurl/c/curl_multi_init.html */
  extern proc curl_multi_init():c_ptr(CURLM);
  /* See https://curl.haxx.se/libcurl/c/curl_multi_add_handle.html */
  extern proc curl_multi_add_handle(curlm:c_ptr(CURLM), curl:c_ptr(CURL)):CURLMcode;
  /* See https://curl.haxx.se/libcurl/c/curl_multi_timeout.html */
  extern proc curl_multi_timeout(curlm:c_ptr(CURLM), ref timeout:c_long):CURLMcode;
  /* See https://curl.haxx.se/libcurl/c/curl_multi_fdset.html */
  extern proc curl_multi_fdset(curlm:c_ptr(CURLM), read_fd_set:c_ptr(fd_set), write_fd_set:c_ptr(fd_set), exc_fd_set:c_ptr(fd_set), ref max_fd:c_int):CURLMcode;
  /* See https://curl.haxx.se/libcurl/c/curl_multi_perform.html */
  extern proc curl_multi_perform(curlm:c_ptr(CURLM), ref running_handles):CURLMcode;
  /* See https://curl.haxx.se/libcurl/c/curl_multi_remove_handle.html */
  extern proc curl_multi_remove_handle(curlm:c_ptr(CURLM), curl:c_ptr(CURL)):CURLMcode;
  /* See https://curl.haxx.se/libcurl/c/curl_multi_cleanup.html */
  extern proc curl_multi_cleanup(curlm:c_ptr(CURLM)):CURLcode;

  pragma "no doc"
  module CurlQioIntegration {

    import Time;
    use IO;
    use Curl;
    use Sys;

    class CurlFile : QioPluginFile {

      var url_c: c_string;     // Path/URL
      var length: ssize_t;    // length of what we are reading, -1 if we can't get

      var seekable: bool;

      override proc setupChannel(out pluginChannel:unmanaged QioPluginChannel?,
                          start:int(64),
                          end:int(64),
                          qioChannelPtr:qio_channel_ptr_t):syserr {
        var curlch = new unmanaged CurlChannel();
        curlch.curlf = this:unmanaged;
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
      override proc getpath(out path:c_string, out len:int(64)):syserr {
        path = qio_strdup(this.url_c);
        len = url_c.size;
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
      var curlf: unmanaged CurlFile?;
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

      return strncmp(haystack, needle, needle.size:size_t) == 0;
    }

    private proc curl_write_string(contents: c_void_ptr, size:size_t, nmemb:size_t, userp: c_void_ptr) {
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
        // it is larger than this, we'll take care of it in curl_write_string.

        buf.mem = c_calloc(uint(8), 800);
        buf.len = 0;
        buf.alloced = 800;

        var curl:c_ptr(CURL);

        curl = curl_easy_init();
        curl_easy_setopt_ptr(curl, CURLOPT_URL, fl.url_c:c_void_ptr);
        curl_easy_setopt_ptr(curl, CURLOPT_WRITEFUNCTION, c_ptrTo(curl_write_string):c_void_ptr);
        curl_easy_setopt_ptr(curl, CURLOPT_HEADERDATA, c_ptrTo(buf));
        curl_easy_setopt_long(curl, CURLOPT_NOBODY, 1);

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
        curl_easy_getinfo_ptr(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, c_ptrTo(lengthDouble));
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
      curl_easy_setopt_ptr(curl, CURLOPT_URL, cc.curlf!.url_c:c_void_ptr);

      var writer = qio_channel_writable(cc.qio_ch);

      if writer {
        //writeln("Setting up upload");
        // Set the function to get the data to send
        err = curl_easy_setopt_long(curl, CURLOPT_UPLOAD, 1);
        if err then return EINVAL;
        err =curl_easy_setopt_ptr(curl, CURLOPT_READFUNCTION, c_ptrTo(curl_read_buffered):c_void_ptr);
        if err then return EINVAL;
        err = curl_easy_setopt_ptr(curl, CURLOPT_READDATA, cc:c_void_ptr);
        if err then return EINVAL;

        // TODO -- is this necessary?
        //err = curl_easy_setopt_offset(curl, CURLOPT_INFILESIZE_LARGE, 14);
        //if err then return EINVAL;
      } else {
        //writeln("Setting up download");
        // Set the function to process the received data
        err = curl_easy_setopt_ptr(curl, CURLOPT_WRITEFUNCTION, c_ptrTo(curl_write_received):c_void_ptr);
        if err then return EINVAL;
        err = curl_easy_setopt_ptr(curl, CURLOPT_WRITEDATA, cc:c_void_ptr);
        if err then return EINVAL;
      }
      // If it's seekable, start at the right offset
      if cc.curlf!.seekable {  // we can request byteranges
        err = curl_easy_setopt_offset(curl, CURLOPT_RESUME_FROM_LARGE, start);
        if err then return EINVAL;
      } else {
        if start != 0 then
          return EINVAL;
      }

      // Note, this does not start the operation right away in order
      // to give the user to modify the operation with setopt(channel) calls

      //writeln("finished start_channel");

      return ENOERR;
    }

    private proc curl_write_received(contents: c_void_ptr, size:size_t, nmemb:size_t, userp: c_void_ptr):size_t {
      var realsize:size_t = size * nmemb;
      var cc = userp:unmanaged CurlChannel?;
      var err:syserr = ENOERR;

      // lock the channel if it's not already locked
      assert(cc!.have_channel_lock);

      var amt = realsize.safeCast(ssize_t);

      //writeln("curl_write_received offset=", qio_channel_offset_unlocked(cc.qio_ch), " len=", amt);

      // make sure the channel has room in the buffer for the data
      // copy the data to the channel's buffer
      err = qio_channel_copy_to_available_unlocked(cc!.qio_ch, contents, amt);

      // unlock the channel if we locked it

      if err != ENOERR {
        cc!.saved_error = err;
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

        // Compute the timeout curl recommends
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
    private proc curl_read_buffered(contents: c_void_ptr, size:size_t, nmemb:size_t, userp: c_void_ptr):size_t {
      var realsize:size_t = size * nmemb;
      var cc = userp:unmanaged CurlChannel?;
      var err:syserr = ENOERR;

      // lock the channel if it's not already locked
      assert(cc!.have_channel_lock);

      var amt = realsize.safeCast(ssize_t);

      // Write from the buffer's start position up until the start
      // of the user-visible data.

      var gotamt: ssize_t = 0;
      // copy the data from the channel's buffer
      err = qio_channel_copy_from_buffered_unlocked(cc!.qio_ch, contents, amt, gotamt);

      // unlock the channel if we locked it

      // If there was an error from the channel, abort the connection
      if err != ENOERR {
        cc!.saved_error = err;
        return CURL_READFUNC_ABORT;
      }
      // If the channel is not closed, but we would
      // otherwise return 0, pause the connection, so that
      // the connection is not closed until the channel is.
      if gotamt == 0 && ! qio_channel_isclosed(0, cc!.qio_ch) {
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

        // Compute the timeout curl recommends
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

    proc openCurlFile(url:string,
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
      var url_c = c_calloc(uint(8), url.size+1);
      c_memcpy(url_c:c_void_ptr, url.localize().c_str():c_void_ptr, url.size);

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
  } // end of module CurlQioIntegration

} /* end of module */
