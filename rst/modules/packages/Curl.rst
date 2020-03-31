.. default-domain:: chpl

.. module:: Curl
   :synopsis: Low-level support for many network protocols with libcurl

Curl
====
**Usage**

.. code-block:: chapel

   use Curl;



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

 

.. function:: proc getCurlHandle(ch: channel): c_ptr(CURL) throws

   Returns the ``CURL`` handle connected to a channel opened with
   :proc:`URL.openUrlReader` or :proc:`URL.openUrlWriter`.
   

.. function:: proc setopt(ch: channel, opt: c_int, arg): bool throws

   This function is the equivalent to the
   `curl_easy_setopt <https://curl.haxx.se/libcurl/c/curl_easy_setopt.html>`_
   function in libcurl. It sets information on the curl file handle
   attached to a channel that can change libcurl's behavior.
   
   :arg ch: a curl channel created with openUrlReader or openUrlWriter
   :arg opt: the curl option to set.
   :arg arg: the value to set the curl option specified by opt.
   :type arg: `int`, `string`, `bool`, or `slist`
   

.. function:: proc setopt(ch: channel, args ...?k) throws

   Set curl options on a curl file attached to a channel.
   
   For example, you might do:
   
   .. code-block:: chapel
   
     extern const CURLOPT_USERNAME:CURLoption;
     extern const CURLOPT_PASSWORD:CURLoption;
   
     curlfile.setopt((CURLOPT_USERNAME, username),
                     (CURLOPT_PASSWORD, password));
   
   :arg args: any number of tuples of the form (curl_option, value).
              This function will call ``setopt`` on each pair in turn.
   

.. record:: slist

   
      A linked list of strings used in many curl setopt calls. This type
      corresponds to the libcurl type curl_slist.
   
   .. note::
   
      Memory in the list is not automatically managed. It is necessary
      to call :proc:`slist.free` to free the slist when it is no longer used.
   
   


.. method:: proc slist.append(str: string) throws

   Append the string argument to an slist. This function is the same
   as calling
   `curl_slist_append <https://curl.haxx.se/libcurl/c/curl_slist_append.html>`_
   
   This function halts if an error is encountered. Future versions will
   support returning an error code instead of halting.
   
   :arg str: a string argument to append
   

.. method:: proc slist.free()

   Free an slist. Chapel programs must call this function after using an slist.
   Programs must ensure that there are no ongoing connections using
   this slist when it is freed.
   

.. data:: const CURLE_OK: c_int

   Successful result for CURL easy API calls 

.. data:: const CURLM_OK: c_int

   Successful result for CURL multi API calls 

.. type:: type CURL

   A CURL easy handle. Most CURL functions accept a ``c_ptr(CURL)``. 

.. type:: type CURLM

   A CURL multi handle. 

.. record:: curl_slist

   A CURL string list 


.. type:: type CURLoption = c_int

   CURLoption identifies options for ``curl_easy_setopt``.
   

.. type:: type CURLcode = c_int

   The return type of CURL easy API functions 

.. type:: type CURLMcode = c_int

   The return type of CURL multi API functions 

.. type:: type CURLINFO = c_int

   CURLINFO identifies info to get with ``curl_easy_getinfo`` 

.. type:: type curl_off_t = int(64)

   curl_off_t is a file offset used by the CURL library 

.. function:: proc curl_easy_init(): c_ptr(CURL)

   See https://curl.haxx.se/libcurl/c/curl_easy_init.html 

.. function:: proc curl_easy_getinfo(handle: c_ptr(CURL), info: CURLINFO, arg): CURLcode

   See https://curl.haxx.se/libcurl/c/curl_easy_getinfo.html 

.. function:: proc curl_easy_setopt(handle: c_ptr(CURL), option: CURLoption, arg): CURLcode

   See https://curl.haxx.se/libcurl/c/curl_easy_setopt.html 

.. function:: proc curl_easy_setopt_long(curl: c_ptr(CURL), option: CURLoption, arg: c_long): CURLcode

   Helper function for ``curl_easy_setopt`` when passing a numeric argument 

.. function:: proc curl_easy_setopt_ptr(curl: c_ptr(CURL), option: CURLoption, arg: c_void_ptr): CURLcode

   Helper function for ``curl_easy_setopt`` when passing a pointer argument 

.. function:: proc curl_easy_setopt_offset(curl: c_ptr(CURL), option: CURLoption, offset: int(64)): CURLcode

   Helper function for ``curl_easy_setopt`` when passing an offset argument 

.. function:: proc curl_easy_getinfo_ptr(curl: c_ptr(CURL), info: CURLINFO, arg: c_void_ptr): CURLcode

   Helper function for ``curl_easy_getinfo`` when passing a pointer argument.
   Generally this is a pointer to the value to be set. 

.. function:: proc curl_easy_perform(curl: c_ptr(CURL)): CURLcode

   See https://curl.haxx.se/libcurl/c/curl_easy_perform.html 

.. function:: proc curl_easy_cleanup(curl: c_ptr(CURL)): void

   See https://curl.haxx.se/libcurl/c/curl_easy_cleanup.html 

.. function:: proc curl_easy_pause(curl: c_ptr(CURL), bitmask: c_int): CURLcode

   See https://curl.haxx.se/libcurl/c/curl_easy_pause.html 

.. function:: proc curl_multi_init(): c_ptr(CURLM)

   See https://curl.haxx.se/libcurl/c/curl_multi_init.html 

.. function:: proc curl_multi_add_handle(curlm: c_ptr(CURLM), curl: c_ptr(CURL)): CURLMcode

   See https://curl.haxx.se/libcurl/c/curl_multi_add_handle.html 

.. function:: proc curl_multi_timeout(curlm: c_ptr(CURLM), ref timeout: c_long): CURLMcode

   See https://curl.haxx.se/libcurl/c/curl_multi_timeout.html 

.. function:: proc curl_multi_fdset(curlm: c_ptr(CURLM), read_fd_set: c_ptr(fd_set), write_fd_set: c_ptr(fd_set), exc_fd_set: c_ptr(fd_set), ref max_fd: c_int): CURLMcode

   See https://curl.haxx.se/libcurl/c/curl_multi_fdset.html 

.. function:: proc curl_multi_perform(curlm: c_ptr(CURLM), ref running_handles): CURLMcode

   See https://curl.haxx.se/libcurl/c/curl_multi_perform.html 

.. function:: proc curl_multi_remove_handle(curlm: c_ptr(CURLM), curl: c_ptr(CURL)): CURLMcode

   See https://curl.haxx.se/libcurl/c/curl_multi_remove_handle.html 

.. function:: proc curl_multi_cleanup(curlm: c_ptr(CURLM)): CURLcode

   See https://curl.haxx.se/libcurl/c/curl_multi_cleanup.html 

