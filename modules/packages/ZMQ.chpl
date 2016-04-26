/*
 * Copyright 2004-2016 Cray Inc.
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

// ZMQ.version
// libzmq version 4.0.*

/*
  Module ZMQ: Chapel bindings for the ZeroMQ Messaging Library
*/
module ZMQ {

  require "zmq.h", "-lzmq";

  use Reflection;

  private extern var errno: c_int;

  // Types
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
  private extern proc zmq_msg_send(ref msg: zmq_msg_t, sock: c_void_ptr,
                                   flags: c_int): c_int;
  private extern proc zmq_msg_recv(ref msg: zmq_msg_t, sock: c_void_ptr,
                                   flags: c_int): c_int;
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
  const PUB  = ZMQ_PUB;
  const SUB  = ZMQ_SUB;
  const REQ  = ZMQ_REQ;
  const REP  = ZMQ_REP;
  const PUSH = ZMQ_PUSH;
  const PULL = ZMQ_PULL;

  // -- Socket Options
  private extern const ZMQ_AFFINITY: c_int;
  private extern const ZMQ_IDENTITY: c_int;
  private extern const ZMQ_SUBSCRIBE: c_int;
  const SUBSCRIBE = ZMQ_SUBSCRIBE;
  private extern const ZMQ_UNSUBSCRIBE: c_int;
  private extern const ZMQ_RATE: c_int;
  private extern const ZMQ_RECOVERY_IVL: c_int;
  private extern const ZMQ_SNDBUF: c_int;
  private extern const ZMQ_RCVBUF: c_int;
  private extern const ZMQ_RCVMORE: c_int;
  private extern const ZMQ_FD: c_int;
  private extern const ZMQ_EVENTS: c_int;
  private extern const ZMQ_TYPE: c_int;
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
  private extern const ZMQ_DONTWAIT: c_int;
  private extern const ZMQ_SNDMORE: c_int;

  // -- Security Options
  private extern const ZMQ_NULL: c_int;
  private extern const ZMQ_PLAIN: c_int;
  private extern const ZMQ_CURVE: c_int;

  /*
    Query the ZMQ library version.

    :returns: An :type:`(int,int,int)` tuple of the major, minor, and patch
        version of the ZMQ library.
   */
  proc version: (int,int,int) {
    var major, minor, patch: c_int;
    zmq_version(c_ptrTo(major), c_ptrTo(minor), c_ptrTo(patch));
    return (major:int, minor:int, patch:int);
  }

  /*
    ZMQ.Context

    The ZMQ Context class maintains a vector of child sockets.  This allows
    users to delete the Context object without managing the closure and
    deletion of Sockets created from this Context.

    In ZMQ's C API, context "objects" are thread safe. The lock internal to
    this Chapel class is only used to serialize accesses to the vector-style
    array of child sockets.
  */
  class Context {

    // Member Elements
    var ctx: c_void_ptr;
    var socks: [1..0] Socket;
    var lock: sync bool;

    // Constructor
    proc Context() {
      this.ctx = zmq_ctx_new();
      if this.ctx == nil {
        var errmsg = zmq_strerror(errno):string;
        writef("Error in Context(): %s\n", errmsg);
      }
    }

    // Destructor
    pragma "no doc"
    proc ~Context() {
      // Close any open sockets
      for sock in socks do
        delete sock;
      // Terminate the context
      var ret = zmq_ctx_term(this.ctx):int;
      if ret == -1 {
        var errmsg = zmq_strerror(errno):string;
        writef("Error in ~Context(): %s\n", errmsg);
      }
    }

    // socket()
    proc socket(socktype: int, parSafe: bool = true,
                linger: int = -1): Socket{
      var sock = new Socket(this, socktype, parSafe, linger);
      this.lock = true;
      this.socks.push_back(sock);
      this.lock;
      return sock;
    }

  } // class Context

  /*
    ZMQ.Socket

    Design Notes:

    - Do not delete Socket objects; they are deleted when the parent
      Context object is deleted, however, they can be closed early.

    Questions:

    - Should the parSafe lock be a *counting* lock, so that Socket member
      functions can call their own locking methods?

  */
  class Socket {

    // Member Elements
    const parSafe: bool = true;
    var lock: sync bool;
    var socket: c_void_ptr;
    var context: Context;

    // Constructor
    proc Socket(ctx: Context, socktype: int, parSafe: bool,
                linger: int = -1) {
      this.context = ctx;
      this.socket = zmq_socket(ctx.ctx, socktype:c_int);
      if this.socket == nil {
        var errmsg = zmq_strerror(errno):string;
        writef("Error in Socket(): %s\n", errmsg);
      }
      // Not calling Socket.setsockopt() due to non-recursive binary locking
      var val = linger:c_int;
      var ret = zmq_setsockopt(this.socket, ZMQ_LINGER,
                               c_ptrTo(val):c_void_ptr,
                               numBytes(c_int):size_t):int;
      if ret == -1 {
        var errmsg = zmq_strerror(errno):string;
        writef("Error in Socket() setting ZMQ_LINGER: %s\n", errmsg);
      }
    }

    // Destructor
    pragma "no doc"
    proc ~Socket() {
      if this.socket != nil then
        this.close();
    }

    // close
    proc close() {
      if this.parSafe then this.lock = true;
      var ret = zmq_close(this.socket):int;
      if ret == -1 {
        var errmsg = zmq_strerror(errno):string;
        writef("Error in Socket.close(): %s\n", errmsg);
      }
      this.socket = c_nil;
      if this.parSafe then this.lock;
    }

    // connect
    proc connect(endpoint: string) {
      if this.parSafe then this.lock = true;
      var ret = zmq_connect(this.socket, endpoint.c_str());
      if this.parSafe then this.lock;
      if ret == -1 {
        var errmsg = zmq_strerror(errno):string;
        writef("Error in Socket.connect(): %s\n", errmsg);
      }
    }

    // bind
    proc bind(endpoint: string) {
      if this.parSafe then this.lock = true;
      var ret = zmq_bind(this.socket, endpoint.c_str());
      if this.parSafe then this.lock;
      if ret == -1 {
        var errmsg = zmq_strerror(errno):string;
        writef("Error in Socket.bind(): %s\n", errmsg);
      }
    }

    // setsockopt
    proc setsockopt(option: int, value: ?T) where isPODType(T) {
      if this.parSafe then this.lock = true;
      var copy: T = value;
      var ret = zmq_setsockopt(this.socket, option:c_int,
                               c_ptrTo(copy):c_void_ptr,
                               numBytes(T)): int;
      if ret == -1 {
        var errmsg = zmq_strerror(errno):string;
        writef("Error in Socket.setsockopt(): %s\n", errmsg);
      }
      if this.parSafe then this.lock;
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

      :arg flags: An optional argument of the OR-able flags :const:`DONTWAIT`
          and :const:`SNDMORE`.
      :type flags: `int`
     */
    proc send(data: ?T, flags: int = 0) where !isZMQSerializable(T) {
      compilerError("Type \"", T:string, "\" is not serializable by ZMQ");
    }

    // send, strings
    pragma "no doc"
    proc send(data: string, flags: int = 0) {
      // message part 1, length
      send(data.length:uint, ZMQ_SNDMORE | flags);
      // message part 2, string
      while (-1 == zmq_send(this.socket, data.c_str():c_void_ptr,
                            data.length:size_t,
                            (ZMQ_DONTWAIT | flags):c_int)) {
        if errno == EAGAIN then
          chpl_task_yield();
        else {
          var errmsg = zmq_strerror(errno):string;
          halt("Error in Socket.send(%s): %s\n".format(string:string, errmsg));
        }
      }
    }

    // send, numeric types
    pragma "no doc"
    proc send(data: ?T, flags: int = 0) where isNumericType(T) {
      var temp = data;
      while (-1 == zmq_send(this.socket, c_ptrTo(temp):c_void_ptr,
                            numBytes(T):size_t,
                            (ZMQ_DONTWAIT | flags):c_int)) {
        if errno == EAGAIN then
          chpl_task_yield();
        else {
          var errmsg = zmq_strerror(errno):string;
          halt("Error in Socket.send(%s): %s\n".format(T:string, errmsg));
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
      param N = numFields(T);
      for param i in 1..(N-1) do
        send(getField(data,i), ZMQ_SNDMORE | flags);
      send(getField(data,N), flags);
    }

    /*
      Receive an object of type :type:`T` from a socket.

      :arg T: The type of the object to be received. If :type:`T` is not
          serializable by the ZMQ module, a compile-time error will be raised.

      :arg flags: An optional argument of the flag :const:`SNDMORE`.
      :type flags: `int`

      :returns: An object of type :type:`T`
     */
    proc recv(type T, flags: int = 0): T where !isZMQSerializable(T) {
      compilerError("Type \"", T:string, "\" is not serializable by ZMQ");
    }

    // recv, strings
    pragma "no doc"
    proc recv(type T, flags: int = 0) where isString(T) {
      var len = recv(uint, flags):int;
      var buf = c_calloc(uint(8), (len+1):size_t);
      var str = new string(buff=buf, length=len, size=len+1,
                           owned=true, needToCopy=false);
      while (-1 == zmq_recv(this.socket, buf:c_void_ptr, len:size_t,
                            (ZMQ_DONTWAIT | flags):c_int)) {
        if errno == EAGAIN then
          chpl_task_yield();
        else {
          var errmsg = zmq_strerror(errno):string;
          halt("Error in Socket.recv(%s): %s\n".format(T:string, errmsg));
        }
      }
      return str;
    }

    // recv, numeric types
    pragma "no doc"
    proc recv(type T, flags: int = 0) where isNumericType(T) {
      var data: T;
      while (-1 == zmq_recv(this.socket, c_ptrTo(data):c_void_ptr,
                            numBytes(T):size_t,
                            (ZMQ_DONTWAIT | flags):c_int)) {
        if errno == EAGAIN then
          chpl_task_yield();
        else {
          var errmsg = zmq_strerror(errno):string;
          halt("Error in Socket.recv(%s): %s\n".format(T:string, errmsg));
        }
      }
      return data;
    }

    // recv, enumerated types
    pragma "no doc"
    proc recv(type T, flags: int = 0) where isEnumType(T) {
      return recv(int, flags):T;
    }

    // recv, records (of other supported things)
    pragma "no doc"
    proc recv(type T, flags: int = 0) where (isRecordType(T) && (!isString(T))) {
      var data: T;
      for param i in 1..numFields(T) do
        getFieldRef(data,i) = recv(getField(data,i).type);
      return data;
    }

  } // class Socket

} // module ZMQ
