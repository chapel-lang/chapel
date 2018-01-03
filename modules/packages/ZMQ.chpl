/*
 * Copyright 2004-2018 Cray Inc.
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

/*

Lightweight messaging with ZeroMQ (or ØMQ)

This module provides high-level Chapel bindings to the
`ZeroMQ messaging library <http://zeromq.org/>`_.

Dependencies
------------

The ZMQ module in Chapel is dependent on ZeroMQ.  For information on how to
install ZeroMQ, see the
`ZeroMQ installation instructions <http://zeromq.org/intro:get-the-software>`_.

.. note::

   Chapel's support for ZeroMQ is a work in progress and may not (yet) support
   the full functionality of the ZeroMQ C API.

.. note::

   Chapel's ZMQ module was developed for compatibility with ZeroMQ v4.x.

Using ZMQ in Chapel
-------------------

.. _using-contexts:

Contexts
++++++++

In ZeroMQ proper, a `context` is an opaque, thread-safe handle to manage
ØMQ's resources within a process.  Typically, a process will allocate only one
context, although more than one context per process is allowable
`[ref] <http://zguide.zeromq.org/page:all#Getting-the-Context-Right>`__.

In Chapel, a :record:`Context` is a reference-counted wrapper around the
underlying ØMQ context. To create a context, it is sufficient to write:

.. code-block:: chapel

   var context: Context;

.. _using-sockets:

Sockets
+++++++

In ZeroMQ, a `socket` is an opaque handle to an asynchronous, message-based
communication channel that is "typed" to provide one of a series of common
communication patterns (i.e., the socket type).

In Chapel, as with a :record:`Context`, a :record:`Socket` is a
reference-counted wrapper around the underlying ØMQ socket.  Sockets are
created via :proc:`Context.socket()` and maintain a reference to the parent
context, so that the parent context may go out of scope and the context will
not be reclaimed while any sockets are still in use.

.. note::

   As with ØMQ's C API, a :record:`Socket` object is not thread safe.
   That is, a :record:`Socket` object should not be accessed concurrently by
   multiple Chapel tasks.  (This may change in a future ZMQ module.)

A :record:`Socket` may be one of the socket types in the following list of
compatible pairs of socket types
`[ref] <http://zguide.zeromq.org/page:all#Messaging-Patterns>`__:

* :const:`PUB`  and :const:`SUB`
* :const:`REQ`  and :const:`REP`
* :const:`PUSH` and :const:`PULL`

.. code-block:: chapel

   // create a PUB socket
   var context: Context;
   var socket = context.socket(ZMQ.PUB);

Sending and Receiving Messages
++++++++++++++++++++++++++++++

Sending a message is as simple as passing the object to send as the argument
to :proc:`Socket.send()`.  Receiving a message requires that the type to be
received be passed as the argument to :proc:`Socket.recv()`.
In either case, if the object sent or type to be received cannot be serialized
by ZMQ, the following error shall be produced at compile time.

.. code-block:: chapel

   // send or receive an int
   var val = 42;
   socket.send(val);
   val = socket.recv(int);

   // error: Type "Foo" is not serializable by ZMQ
   class Foo { var val: int; }
   socket.recv(Foo);

Multilocale Support
+++++++++++++++++++

Chapel's ZMQ module supports multilocale execution of ZeroMQ programs.  The
locale on which the :type:`Context` object is created sets the "home" locale
for all sockets created from this context and all operations performed on the
socket.  For example, a send call on a socket from a locale other than the
home locale will migrate a task to the home locale that will remotely access
the data to send it over the socket.

Examples
--------

Example: "Hello, World"
+++++++++++++++++++++++++

This "Hello, World" example demonstrates a :const:`PUSH`-:const:`PULL` socket
pair in two Chapel programs that exchange a single string message.

.. code-block:: chapel

   // pusher.chpl
   use ZMQ;
   config const to: string = "world!";
   var context: Context;
   var socket = context.socket(ZMQ.PUSH);
   socket.bind("tcp://*:5555");
   socket.send(to);

.. (comment) the above started a nested comment, so here we end it */

.. code-block:: chapel

   // puller.chpl
   use ZMQ;
   var context: Context;
   var socket = context.socket(ZMQ.PULL);
   socket.connect("tcp://localhost:5555");
   writeln("Hello, ", socket.recv(string));

Implementation Notes
--------------------

As noted previously, the ZMQ module is a work in progress.  The implementation
notes below describe some of how the ZMQ module is implemented and how future
versions may expose more features native to ZeroMQ.

Serialization
+++++++++++++

In Chapel, sending or receiving messages is supported for a variety of types.
Primitive numeric types and strings are supported as the foundation.
In addition, user-defined :type:`record` types may be serialized automatically
as `multipart messages <http://zguide.zeromq.org/page:all#Multipart-Messages>`_
by internal use of the :chpl:mod:`Reflection` module.
Currently, the ZMQ module can serialize records of primitive numeric types,
strings, and other serializable records.

.. note::

   The serialization protocol for strings changed in Chapel 1.16 in order to
   support inter-language messaging through ZeroMQ. (See
   :ref:`notes on interoperability <interop>` below.)
   As a result, programs using ZMQ that were compiled by Chapel 1.15 or
   earlier cannot communicate using strings with programs compiled by
   Chapel 1.16 or later, although such communication can be used between
   programs compiled by the same version without issue.

   Prior to Chapel 1.16, ZMQ would send a string as two multipart messages:
   the first sent the length as `int`; the second sent the character array
   (in bytes).  It was identified that this scheme was incompatible with how
   other language bindings for ZeroMQ serialize and send strings.

   As of Chapel 1.16, the ZMQ module uses the C-level ``zmq_msg_send()`` and
   ``zmq_msg_recv()`` API for :proc:`Socket.send()` and :proc:`Socket.recv()`,
   respectively, when transmitting strings.  Further, ZMQ sends the string as
   a single message of only the byte stream of the string's character array.
   (Recall that Chapel's :type:`string` type currently only supports ASCII
   strings, not full Unicode strings.)

.. _interop:

Interoperability
++++++++++++++++

The ZeroMQ messaging library has robust support in many programming languages
and Chapel's ZMQ module intends on providing interfaces and serialization
protocols suitable for exchanging data between Chapel and non-Chapel programs.

As an example (and test) of Chapel-Python interoperability over ZeroMQ, the
following sources demonstrate a :const:`PUSH`-:const:`PULL` socket pair between
a Chapel server and a Python client using the
`PyZMQ Python bindings for ZeroMQ <https://pyzmq.readthedocs.io/en/latest/>`_.

.. literalinclude:: ../../../../test/library/packages/ZMQ/interop-py/server.chpl
   :language: chapel
   :lines: 10-

.. literalinclude:: ../../../../test/library/packages/ZMQ/interop-py/client.py
   :language: python

Tasking-Layer Interaction
+++++++++++++++++++++++++

As noted previously, a :record:`Socket` object is not thread safe.  As the
mapping of Chapel tasks to OS threads is dependent on the Chapel tasking layer
and may be cooperatively scheduled, a :record:`Socket` object should not be
accessed concurrently by multiple Chapel tasks.

The ZMQ module is designed to "play nicely" with the Chapel tasking layer.
While the C-level call ``zmq_send()`` may be a blocking call (depending on
the socket type and flag arguments), it is desirable that a
semantically-blocking call to :proc:`Socket.send()` allow other Chapel tasks
to be scheduled on the OS thread as supported by the tasking layer.
Internally, the ZMQ module uses non-blocking calls to ``zmq_send()`` and
``zmq_recv()`` to transfer data, and yields to the tasking layer via
`chpl_task_yield()` when the call would otherwise block.

Limitations and Future Work
+++++++++++++++++++++++++++

Currently, the ZMQ module does not provide interfaces for working with
`ZeroMQ message objects <http://zguide.zeromq.org/page:all#Working-with-Messages>`_,
`handling errors <http://zguide.zeromq.org/page:all#Handling-Errors-and-ETERM>`_,
or making explicitly non-blocking send/receive calls.  These are
active-but-incomplete areas of work that are intended to be supported in
future Chapel releases.

One interaction of these features is worth noting explicitly: because multipart
messages are used to automatically serialize non-primitive data types (e.g.,
strings and records) and a partially-sent multi-part message cannot be
canceled (except by closing the socket), an explicitly non-blocking send call
that encountered an error in the ZeroMQ library during serialization would not
be in a recoverable state, nor would there be a matching "partial receive".

References
----------

* `ZeroMQ C API Reference <http://api.zeromq.org/>`_
* `ZeroMQ, The Guide <http://zguide.zeromq.org/page:all>`_

*/
module ZMQ {

  // TODOs:
  //  use error handling instead of halting on an error

  require "zmq.h", "-lzmq";

  use Reflection;
  use ExplicitRefCount;

  private extern proc chpl_macro_int_errno():c_int;
  private inline proc errno return chpl_macro_int_errno():c_int;

  // Types
  pragma "no doc"
  extern type zmq_msg_t;

  // C API
  private extern proc zmq_bind(sock: c_void_ptr, endpoint: c_string): c_int;
  private extern proc zmq_close(ctx: c_void_ptr): c_int;
  private extern proc zmq_connect(sock: c_void_ptr, endpoint: c_string): c_int;
  private extern proc zmq_ctx_new(): c_void_ptr;
  private extern proc zmq_ctx_term(ctx: c_void_ptr): c_int;
  private extern proc zmq_errno(): c_int;
  private extern proc zmq_msg_init(ref msg: zmq_msg_t): c_int;
  private extern proc zmq_msg_init_size(ref msg: zmq_msg_t,
                                        size: size_t): c_int;
  private extern proc zmq_msg_init_data(ref msg: zmq_msg_t,
                                        data: c_void_ptr,
                                        size: size_t,
                                        ffn: c_fn_ptr,
                                        hint: c_void_ptr): c_int;
  private extern proc zmq_msg_data(ref msg: zmq_msg_t): c_void_ptr;
  private extern proc zmq_msg_size(ref msg: zmq_msg_t): size_t;
  private extern proc zmq_msg_send(ref msg: zmq_msg_t, sock: c_void_ptr,
                                   flags: c_int): c_int;
  private extern proc zmq_msg_recv(ref msg: zmq_msg_t, sock: c_void_ptr,
                                   flags: c_int): c_int;
  private extern proc zmq_msg_close(ref msg: zmq_msg_t): c_int;
  private extern proc zmq_recv(sock: c_void_ptr, buf: c_void_ptr,
                               len: size_t, flags: c_int): c_int;
  private extern proc zmq_send(sock: c_void_ptr, buf: c_void_ptr,
                               len: size_t, flags: c_int): c_int;
  private extern proc zmq_setsockopt (sock: c_void_ptr, option_name: int,
                                      const option_value: c_void_ptr,
                                      option_len: size_t): c_int;
  private extern proc zmq_socket(ctx: c_void_ptr, socktype: c_int): c_void_ptr;
  private extern proc zmq_strerror(errnum: c_int): c_string;
  private extern proc zmq_version(major: c_ptr(c_int),
                                  minor: c_ptr(c_int),
                                  patch: c_ptr(c_int));

  // Constants
  // -- Socket Types
  private extern const ZMQ_PAIR: c_int;
  private extern const ZMQ_PUB: c_int;
  private extern const ZMQ_SUB: c_int;
  private extern const ZMQ_REQ: c_int;
  private extern const ZMQ_REP: c_int;
  private extern const ZMQ_DEALER: c_int;
  private extern const ZMQ_ROUTER: c_int;
  private extern const ZMQ_PULL: c_int;
  private extern const ZMQ_PUSH: c_int;
  private extern const ZMQ_XPUB: c_int;
  private extern const ZMQ_XSUB: c_int;
  private extern const ZMQ_STREAM: c_int;

  /*
    The publisher socket type for a publish-subscribe messaging pattern.
   */
  const PUB  = ZMQ_PUB;

  /*
    The subscriber socket type for a publish-subscribe messaging pattern.
   */
  const SUB  = ZMQ_SUB;

  /*
    The requester socket type for a paired request-reply messaging pattern.
   */
  const REQ  = ZMQ_REQ;

  /*
    The replier socket type for a paired request-reply messaging pattern.
   */
  const REP  = ZMQ_REP;

  /*
    The pusher socket type for a pipeline messaging pattern.
   */
  const PUSH = ZMQ_PUSH;

  /*
    The puller socket type for a pipeline messaging pattern.
   */
  const PULL = ZMQ_PULL;

  // -- Socket Options
  private extern const ZMQ_AFFINITY: c_int;
  private extern const ZMQ_IDENTITY: c_int;

  /*
    The :proc:`Socket.setsockopt()` option value to specify the message filter
    for a :const:`SUB`-type :record:`Socket`.
   */
  const SUBSCRIBE = ZMQ_SUBSCRIBE;
  private extern const ZMQ_SUBSCRIBE: c_int;

  /*
    The :proc:`Socket.setsockopt()` option value to remote an existing message
    filter for a :const:`SUB`-type :record:`Socket`.
   */
  const UNSUBSCRIBE = ZMQ_UNSUBSCRIBE;
  private extern const ZMQ_UNSUBSCRIBE: c_int;

  private extern const ZMQ_RATE: c_int;
  private extern const ZMQ_RECOVERY_IVL: c_int;
  private extern const ZMQ_SNDBUF: c_int;
  private extern const ZMQ_RCVBUF: c_int;
  private extern const ZMQ_RCVMORE: c_int;
  private extern const ZMQ_FD: c_int;
  private extern const ZMQ_EVENTS: c_int;
  private extern const ZMQ_TYPE: c_int;

  /*
    The :proc:`Socket.setsockopt()` option value to specify the linger period
    for the associated :record:`Socket` object.
   */
  const LINGER = ZMQ_LINGER;
  private extern const ZMQ_LINGER: c_int;

  private extern const ZMQ_RECONNECT_IVL: c_int;
  private extern const ZMQ_BACKLOG: c_int;
  private extern const ZMQ_RECONNECT_IVL_MAX: c_int;
  private extern const ZMQ_MAXMSGSIZE: c_int;
  private extern const ZMQ_SNDHWM: c_int;
  private extern const ZMQ_RCVHWM: c_int;
  private extern const ZMQ_MULTICAST_HOPS: c_int;
  private extern const ZMQ_RCVTIMEO: c_int;
  private extern const ZMQ_SNDTIMEO: c_int;
  private extern const ZMQ_LAST_ENDPOINT: c_int;
  private extern const ZMQ_ROUTER_MANDATORY: c_int;
  private extern const ZMQ_TCP_KEEPALIVE: c_int;
  private extern const ZMQ_TCP_KEEPALIVE_CNT: c_int;
  private extern const ZMQ_TCP_KEEPALIVE_IDLE: c_int;
  private extern const ZMQ_TCP_KEEPALIVE_INTVL: c_int;
  private extern const ZMQ_TCP_ACCEPT_FILTER: c_int;
  private extern const ZMQ_IMMEDIATE: c_int;
  private extern const ZMQ_XPUB_VERBOSE: c_int;
  private extern const ZMQ_ROUTER_RAW: c_int;
  private extern const ZMQ_IPV6: c_int;
  private extern const ZMQ_MECHANISM: c_int;
  private extern const ZMQ_PLAIN_SERVER: c_int;
  private extern const ZMQ_PLAIN_USERNAME: c_int;
  private extern const ZMQ_PLAIN_PASSWORD: c_int;
  private extern const ZMQ_CURVE_SERVER: c_int;
  private extern const ZMQ_CURVE_PUBLICKEY: c_int;
  private extern const ZMQ_CURVE_SECRETKEY: c_int;
  private extern const ZMQ_CURVE_SERVERKEY: c_int;
  private extern const ZMQ_PROBE_ROUTER: c_int;
  private extern const ZMQ_REQ_CORRELATE: c_int;
  private extern const ZMQ_REQ_RELAXED: c_int;
  private extern const ZMQ_CONFLATE: c_int;
  private extern const ZMQ_ZAP_DOMAIN: c_int;

  // -- Message Options
  private extern const ZMQ_MORE: c_int;

  // -- Send/Recv Options

  /*
    The flag value for :proc:`Socket.send()` and :proc:`Socket.recv()` to
    indicate that the associated send or receive operation should be performed
    as a non-blocking operation.
   */
  private extern const ZMQ_DONTWAIT: c_int;

  /*
    The flag value for :proc:`Socket.send()` to indicate that the associated
    send operation is a multi-part message and that more message parts will
    subsequently be issued.
   */
  private extern const ZMQ_SNDMORE: c_int;

  // -- Security Options
  private extern const ZMQ_NULL: c_int;
  private extern const ZMQ_PLAIN: c_int;
  private extern const ZMQ_CURVE: c_int;

  pragma "no doc"
  const unset = -42;

  pragma "no doc"
  proc free_helper(data: c_void_ptr, hint: c_void_ptr) {
    chpl_here_free(data);
  }

  /*
    Query the ZMQ library version.

    :returns: An `(int,int,int)` tuple of the major, minor, and patch
        version of the ZMQ library.
   */
  proc version: (int,int,int) {
    var major, minor, patch: c_int;
    zmq_version(c_ptrTo(major), c_ptrTo(minor), c_ptrTo(patch));
    return (major:int, minor:int, patch:int);
  }

  pragma "no doc"
  class ContextClass: RefCountBase {
    var ctx: c_void_ptr;
    var home: locale;

    proc ContextClass() {
      this.home = here;
      this.ctx = zmq_ctx_new();
      if this.ctx == nil {
        var errmsg = zmq_strerror(errno):string;
        halt("Error in ContextClass(): %s\n", errmsg);
      }
    }

    proc deinit() {
      on this.home {
        var ret = zmq_ctx_term(this.ctx):int;
        if ret == -1 {
          var errmsg = zmq_strerror(errno):string;
          halt("Error in ~ContextClass(): %s\n", errmsg);
        }
      }
    }

  } // class ContextClass

  /*
    A ZeroMQ context. See :ref:`more on using Contexts <using-contexts>`.
    Note that this record contains private fields not listed below.
   */
  record Context {
    pragma "no doc"
    var classRef: ContextClass;

    /*
      Create a ZMQ context.
     */
    proc Context() {
      acquire(new ContextClass());
    }

    pragma "no doc"
    proc deinit() {
      release();
    }

    pragma "no doc"
    proc acquire(newRef: ContextClass) {
      classRef = newRef;
      classRef.incRefCount();
    }

    pragma "no doc"
    proc acquire() {
      classRef.incRefCount();
    }

    pragma "no doc"
    proc release() {
      var rc = classRef.decRefCount();
      if rc == 1 {
        delete classRef;
        classRef = nil;
      }
    }

    /*
      Create a :record:`Socket` of type `sockType` from this context.

      :arg sockType: The ZMQ socket type to be created;
          e.g., :const:`PUB`, :const:`PUSH`, etc.
      :type sockType: `int`
     */
    proc socket(sockType: int): Socket {
      var sock = new Socket(this, sockType);
      return sock;
    }

  } // record Context

  pragma "no doc"
  pragma "init copy fn"
  proc chpl__initCopy(x: Context) {
    x.acquire();
    return x;
  }

  pragma "no doc"
  pragma "auto copy fn"
  proc chpl__autoCopy(x: Context) {
    x.acquire();
    return x;
  }

  pragma "no doc"
  proc =(ref lhs: Context, rhs: Context) {
    if lhs.classRef != nil then
      lhs.release();
    lhs.acquire(rhs.classRef);
  }

  pragma "no doc"
  class SocketClass: RefCountBase {
    var socket: c_void_ptr;
    var home: locale;

    proc SocketClass(ctx: Context, sockType: int) {
      this.home = here;
      this.socket = zmq_socket(ctx.classRef.ctx, sockType:c_int);
      if this.socket == nil {
        var errmsg = zmq_strerror(errno):string;
        halt("Error in SocketClass(): %s\n", errmsg);
      }
    }

    proc deinit() {
      on this.home {
        var ret = zmq_close(socket):int;
        if ret == -1 {
          var errmsg = zmq_strerror(errno):string;
          halt("Error in ~SocketClass(): %s\n", errmsg);
        }
        socket = c_nil;
      }
    }
  }

  /*
    A ZeroMQ socket. See :ref:`more on using Sockets <using-sockets>`.
    Note that this record contains private fields not listed below.
   */
  record Socket {
    pragma "no doc"
    var classRef: SocketClass;

    pragma "no doc"
    var context: Context;

    pragma "no doc"
    proc Socket() {
      compilerError("Cannot create Socket directly; try Context.socket()");
    }

    pragma "no doc"
    proc Socket(ctx: Context, sockType: int) {
      context = ctx;
      on ctx.classRef.home do
        acquire(new SocketClass(ctx, sockType));
    }

    pragma "no doc"
    proc deinit() {
      release();
    }

    pragma "no doc"
    proc acquire(newRef: SocketClass) {
      classRef = newRef;
      classRef.incRefCount();
    }

    pragma "no doc"
    proc acquire() {
      classRef.incRefCount();
    }

    pragma "no doc"
    proc release() {
      var rc = classRef.decRefCount();
      if rc == 1 {
        delete classRef;
        classRef = nil;
      }
    }

    /*
      Close the socket.

      :arg linger: Optional argument to specify the linger period for the
          socket prior to closing.  If -1, then the linger period is infinite;
          if non-negative, then the linger period shall be set to the specified
          value (in milliseconds).
      :type linger: `int`
     */
    proc close(linger: int = unset) {
      on classRef.home {
        if linger != unset then
          setsockopt(LINGER, linger:c_int);
        var ret = zmq_close(classRef.socket):int;
        if ret == -1 {
          var errmsg = zmq_strerror(errno):string;
          writef("Error in Socket.close(): %s\n", errmsg);
        }
        classRef.socket = c_nil;
      }
    }

    /*
      Bind the socket to the specified local `endpoint` and accept incoming
      connections.
     */
    proc bind(endpoint: string) {
      on classRef.home {
        var tmp = endpoint;
        var ret = zmq_bind(classRef.socket, tmp.c_str());
        if ret == -1 {
          var errmsg = zmq_strerror(errno):string;
          halt("Error in Socket.bind(): ", errmsg);
        }
      }
    }

    /*
      Connect the socket to the specified `endpoint`.
     */
    proc connect(endpoint: string) {
      on classRef.home {
        var tmp = endpoint;
        var ret = zmq_connect(classRef.socket, tmp.c_str());
        if ret == -1 {
          var errmsg = zmq_strerror(errno):string;
          writef("Error in Socket.connect(): %s\n", errmsg);
        }
      }
    }

    /*
      Set socket options;
      see `zmq_setsockopt <http://api.zeromq.org/4-0:zmq-setsockopt>`_

      :arg option: a socket option;
          e.g., :const:`LINGER`, :const:`SUBSCRIBE`, :const:`UNSUBSCRIBE`
      :type option: `int`

      :arg value: the socket option value
     */
    proc setsockopt(option: int, value: ?T) where isPODType(T) {
      on classRef.home {
        var copy: T = value;
        var ret = zmq_setsockopt(classRef.socket, option:c_int,
                                 c_ptrTo(copy):c_void_ptr,
                                 numBytes(T)): int;
        if ret == -1 {
          var errmsg = zmq_strerror(errno):string;
          halt("Error in Socket.setsockopt(): ", errmsg);
        }
      }
    }

    pragma "no doc"
    proc setsockopt(option: int, value: string) {
      on classRef.home {
        var ret = zmq_setsockopt(classRef.socket, option:c_int,
                                 value.c_str():c_void_ptr,
                                 value.length:size_t): int;
        if ret == -1 {
          var errmsg = zmq_strerror(errno):string;
          halt("Error in Socket.setsockopt(): ", errmsg);
        }
      }
    }

    // ZMQ serialization checker
    pragma "no doc"
    inline proc isZMQSerializable(type T) param: bool {
      return isNumericType(T) || isEnumType(T) ||
        isString(T) || isRecordType(T);
    }

    /*
      Send an object `data` on a socket.

      :arg data: The object to be sent. If `data` is an object whose type
          is not serializable by the ZMQ module, a compile-time error will be
          raised.

      :type flags: `int`
     */
    proc send(data: ?T) where !isZMQSerializable(T) {
      compilerError("Type \"", T:string, "\" is not serializable by ZMQ");
    }

    pragma "no doc"
    proc send(data: ?T, flags: int) where !isZMQSerializable(T) {
      compilerError("Type \"", T:string, "\" is not serializable by ZMQ");
    }

    // send, strings
    pragma "no doc"
    proc send(data: string, flags: int = 0) {
      on classRef.home {
        // Deep-copy the string to the current locale and release ownership
        // because the ZeroMQ library will take ownership of the underlying
        // buffer and free it when it is no longer needed.
        //
        // TODO: If *not crossing locales*, check for ownership and
        // conditionally have ZeroMQ free the memory.
        var copy = new string(s=data, owned=true);
        copy.owned = false;

        // Create the ZeroMQ message from the string buffer
        var msg: zmq_msg_t;
        if (0 != zmq_msg_init_data(msg, copy.c_str():c_void_ptr,
                                   copy.length:size_t, c_ptrTo(free_helper),
                                   c_nil)) {
          var errmsg = zmq_strerror(errno):string;
          try! halt("Error in Socket.send(%s): %s\n".format(string:string, errmsg));
        }

        // Send the message
        while(-1 == zmq_msg_send(msg, classRef.socket,
                                 (ZMQ_DONTWAIT | flags):c_int)) {
          if errno == EAGAIN then
            chpl_task_yield();
          else {
            var errmsg = zmq_strerror(errno):string;
            try! halt("Error in Socket.send(%s): %s\n".format(string:string, errmsg));
          }
        }
      }
    }

    // send, numeric types
    pragma "no doc"
    proc send(data: ?T, flags: int = 0) where isNumericType(T) {
      on classRef.home {
        var copy = data;
        while (-1 == zmq_send(classRef.socket, c_ptrTo(copy):c_void_ptr,
                              numBytes(T):size_t,
                              (ZMQ_DONTWAIT | flags):c_int)) {
          if errno == EAGAIN then
            chpl_task_yield();
          else {
            var errmsg = zmq_strerror(errno):string;
            try! halt("Error in Socket.send(%s): %s\n".format(T:string, errmsg));
          }
        }
      }
    }

    // send, enumerated types
    pragma "no doc"
    proc send(data: ?T, flags: int = 0) where isEnumType(T) {
      send(data:int, flags);
    }

    // send, records (of other supported things)
    pragma "no doc"
    proc send(data: ?T, flags: int = 0) where (isRecordType(T) &&
                                               (!isString(T))) {
      on classRef.home {
        var copy = data;
        param N = numFields(T);
        for param i in 1..(N-1) do
          send(getField(copy,i), ZMQ_SNDMORE | flags);
        send(getField(copy,N), flags);
      }
    }

    /*
      Receive an object of type `T` from a socket.

      :arg T: The type of the object to be received. If `T` is not
          serializable by the ZMQ module, a compile-time error will be raised.

      :returns: An object of type `T`
     */
    proc recv(type T): T where !isZMQSerializable(T) {
      compilerError("Type \"", T:string, "\" is not serializable by ZMQ");
    }

    pragma "no doc"
    proc recv(type T, flags: int): T where !isZMQSerializable(T) {
      compilerError("Type \"", T:string, "\" is not serializable by ZMQ");
    }

    // recv, strings
    pragma "no doc"
    proc recv(type T, flags: int = 0) where isString(T) {
      var ret: T;
      on classRef.home {
        // Initialize an empty ZeroMQ message
        var msg: zmq_msg_t;
        if (0 != zmq_msg_init(msg)) {
          var errmsg = zmq_strerror(errno):string;
          try! halt("Error in Socket.recv(%s): %s\n".format(string:string, errmsg));
        }

        // Receive the message
        while (-1 == zmq_msg_recv(msg, classRef.socket,
                                  (ZMQ_DONTWAIT | flags):c_int)) {
          if errno == EAGAIN then
            chpl_task_yield();
          else {
            var errmsg = zmq_strerror(errno):string;
            try! halt("Error in Socket.recv(%s): %s\n".format(T:string, errmsg));
          }
        }

        // Construct the string on the current locale, copying the data buffer
        // from the message object; then, release the message object
        var len = zmq_msg_size(msg):int;
        var str = new string(buff=zmq_msg_data(msg):c_ptr(uint(8)),
                             length=len, size=len+1,
                             owned=true, needToCopy=true);
        if (0 != zmq_msg_close(msg)) {
          var errmsg = zmq_strerror(errno):string;
          try! halt("Error in Socket.recv(%s): %s\n".format(string:string, errmsg));
        }

        // Return the string to the calling locale
        ret = str;
      }
      return ret;
    }

    // recv, numeric types
    pragma "no doc"
    proc recv(type T, flags: int = 0) where isNumericType(T) {
      var ret: T;
      on classRef.home {
        var data: T;
        while (-1 == zmq_recv(classRef.socket, c_ptrTo(data):c_void_ptr,
                              numBytes(T):size_t,
                              (ZMQ_DONTWAIT | flags):c_int)) {
          if errno == EAGAIN then
            chpl_task_yield();
          else {
            var errmsg = zmq_strerror(errno):string;
            try! halt("Error in Socket.recv(%s): %s\n".format(T:string, errmsg));
          }
        }
        ret = data;
      }
      return ret;
    }

    // recv, enumerated types
    pragma "no doc"
    proc recv(type T, flags: int = 0) where isEnumType(T) {
      return recv(int, flags):T;
    }

    // recv, records (of other supported things)
    pragma "no doc"
    proc recv(type T, flags: int = 0) where (isRecordType(T) && (!isString(T))) {
      var ret: T;
      on classRef.home {
        var data: T;
        for param i in 1..numFields(T) do
          getFieldRef(data,i) = recv(getField(data,i).type);
        ret = data;
      }
      return ret;
    }

  } // record Socket

  pragma "no doc"
  pragma "init copy fn"
  proc chpl__initCopy(x: Socket) {
    x.acquire();
    return x;
  }

  pragma "no doc"
  pragma "auto copy fn"
  proc chpl__autoCopy(x: Socket) {
    x.acquire();
    return x;
  }

  pragma "no doc"
  proc =(ref lhs: Socket, rhs: Socket) {
    if lhs.classRef == rhs.classRef then return;
    if lhs.classRef != nil then
      lhs.release();
    lhs.acquire(rhs.classRef);
  }

} // module ZMQ
