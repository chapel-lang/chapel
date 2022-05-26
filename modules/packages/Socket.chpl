/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
  Supports inter-process communication through IP sockets.

  The Socket module focuses on connecting, accepting sockets and providing interface for
  communication between them. Also provided are some constant values representing
  common idioms in socket programming, such as standard Addresses, Families and Flags.
  For more constants that can used alongside refer to :mod:`Sys`.

  To those familiar with the Unix socket API, the method names will feel familiar,
  though their usage will be somewhat simpler than the raw Unix socket API.

  Records
  -----------------
  :type:`ipAddr`
  :type:`tcpConn`
  :type:`tcpListener`
  :type:`udpSocket`

  Enum
  -----------------
  :type:`IPFamily`

  Procedures
  -----------------
  :proc:`bind`
  :proc:`connect`
  :proc:`getPeerName`
  :proc:`getSockOpt`
  :proc:`getSockName`
  :proc:`listen`
  :proc:`setSockOpt`

  Records, Types and Function Definitions
  ---------------------------------------
*/
module Socket {

// TODO: replace with mason config
// Socket module uses -levent for creating a event loop
// and -levent_pthreads to allow for multi-threaded event loop
// access. Each function that has to wait for I/O adds its event
// onto the event loop which then waits for the event to occur
// as soon as the event occurs, a callback function is called
// which writes back to a sync variable passed on to it by the
// caller. The caller then reads the sync variable to determine
// event type and processes things further.
require "event2/event.h";
require "event2/thread.h";
require "-levent";
require "-levent_pthreads";

public use Sys;
public use SysError;
public use CTypes;
use Time;
use OS, OS.POSIX;
use IO;
import SysBasic.{syserr, ENOERR, fd_t, qio_err_t};

/*
  Available values for different Internet
  Protocol Families to be used when creating Sockets.
*/
enum IPFamily {
  /* IPv4 */
  IPv4 = 2,
  /* IPv6 */
  IPv6 = 10,
  /* IP Unspecified */
  IPUnspec = 0
}

/* Type of Standard IPv4 Address */
type ipv4Addr = sys_in_addr_t;
/* Standard IPv4 Addresses of type :type:`ipv4Addr` */
const IPv4Localhost:ipv4Addr = INADDR_LOOPBACK;
const IPv4Any:ipv4Addr = INADDR_ANY;
const IPv4Broadcast:ipv4Addr = INADDR_BROADCAST;

/* Type of Standard IPv6 Address */
type ipv6Addr = sys_in6_addr_t;
/* Standard IPv6 Addresses of type :type:`ipv6Addr` */
const IPv6Localhost:ipv6Addr = in6addr_loopback;
const IPv6Any:ipv6Addr = in6addr_any;

pragma "no doc"
proc sys_sockaddr_t.init(in other: sys_sockaddr_t) {
  this.init();
  try! {
    var host = other.numericHost();
    var port = other.port();
    var family = other.family;
    this.set(host.c_str(), port, family:c_int);
  }
}

/*
  Abstract supertype for network addresses. Contains data
  about :type:`IPFamily`, `host` and `port`. It supports both
  IPv4 and IPv6 addresses. ipAddr can be compared using
  `==` and `!=` operators.
*/
record ipAddr {
  pragma "no doc"
  var _addressStorage:sys_sockaddr_t;

  pragma "no doc"
  proc init(in address: sys_sockaddr_t) {
    this._addressStorage = new sys_sockaddr_t(address);
  }

  pragma "no doc"
  proc init() {
    try! this._addressStorage = new sys_sockaddr_t(ipAddr.create()._addressStorage);
  }

  /*
    Returns the family type of address.
    :return: family type of address
    :rtype: :type:`IPFamily`
  */
  proc const ref family {
    return try! _addressStorage.family:IPFamily;
  }

  /*
    Returns the host address.
    :return: host address
    :rtype: `string`
  */
  proc const ref host {
    return try! _addressStorage.numericHost();
  }

  /*
    Returns the `port` stored in record.
    :return: Returns numeric port.
    :rtype: `uint(16)`
  */
  proc const ref port {
    return try! _addressStorage.port();
  }

  operator =(in other: ipAddr) {
    this._addressStorage = new sys_sockaddr_t(other._addressStorage);
  }
}


/*
  Returns a new record of type :type:`ipAddr` provided `host`, `port`
  and `family`. this function is equivalent to the following code:

  :arg host: address string in dot-dash or colon notation depending on family.
  :type host: `string`
  :arg port: network address's port.
  :type port: `uint(16)`
  :arg family: value of IP Family
  :type family: :type:`IPFamily`
  :return: address instance.
  :rtype: `ipAddr`
  :throws SystemError: Upon incompatible `host`, `port` or `family`
*/
proc type ipAddr.create(host: string = "127.0.0.1", port: uint(16) = 8000,
          family: IPFamily = IPFamily.IPv4): ipAddr throws {
  // We will use type methods for now but expect to add initializers (and possibly deprecate these ones) once [#8692](https://github.com/chapel-lang/chapel/issues/8692) is resolved
  var addressStorage = new sys_sockaddr_t();
  addressStorage.set(host.c_str(), port, family:c_int);
  return new ipAddr(addressStorage);
}

/*
  Returns a new record of type `ipAddr` provided `host` and `port`.
  The family type is assumed based on `host` which is a standard address.
  this function is equivalent to the following code:

  :arg host: standard ipv4 address.
  :type host: `ipv4Addr`
  :arg port: network address's port.
  :type port: `uint(16)`
  :return: address instance.
  :rtype: `ipAddr`
  :throws SystemError: Upon incompatible `host`, `port` or `family`
*/
proc type ipAddr.ipv4(host: ipv4Addr, port: uint(16) = 8000): ipAddr throws {
  var addressStorage = new sys_sockaddr_t();
  addressStorage.set(host, port);
  return new ipAddr(addressStorage);
}

/*
  Returns a new record of type `ipAddr` provided `host` and `port`.
  The family type is assumed based on `host` which is a standard address.
  this function is equivalent to the following code:

  :arg host: standard ipv6 address.
  :type host: `ipv6Addr`
  :arg port: network address's port.
  :type port: `uint(16)`
  :return: address instance.
  :rtype: `ipAddr`
  :throws SystemError: Upon incompatible `host`, `port` or `family`
*/
proc type ipAddr.ipv6(host: ipv6Addr, port: uint(16) = 8000): ipAddr throws {
  var addressStorage = new sys_sockaddr_t();
  addressStorage.set(host, port);
  return new ipAddr(addressStorage);
}

pragma "no doc"
inline operator !=(const ref lhs: ipAddr, const ref rhs: ipAddr) {
  return try! lhs.family != rhs.family || lhs.host != rhs.host || lhs.port != rhs.port;
}

pragma "no doc"
inline operator ==(const ref lhs: ipAddr, const ref rhs: ipAddr) {
  return !(lhs != rhs);
}

pragma "no doc"
proc ipAddr.writeThis(f) throws {
  f.write("(","family:",this.family,",host:",this.host,",port:",this.port,")");
}

pragma "no doc"
operator timeval.=(other: real) {
  this.tv_sec = other:c_long;
  this.tv_usec = (other - this.tv_sec) * 1000000;
}

pragma "no doc"
private extern proc qio_get_fd(fl:qio_file_ptr_t, ref fd:c_int):syserr;

/* The type returned from :proc:`connect` */
type tcpConn = file;

/*
  Returns the file descriptor associated with socket
  :return: file descriptor
  :rtype: `int(32)`
*/
proc tcpConn.socketFd throws {
  var tempfd:c_int;
  var err:syserr = ENOERR;
  on this.home {
    var localtempfd = tempfd;
    err = qio_get_fd(this._file_internal, localtempfd);
    if err then try ioerror(err, "in tcpConn.socketFd");
    tempfd = localtempfd;
  }
  return tempfd;
}

/*
  Returns the address of remote socket connection
  :return: Returns remote address
  :rtype: `ipAddr`
*/
proc tcpConn.addr throws {
  var address:ipAddr;
  on this.home {
    address = getPeerName(this.socketFd);
  }
  return address;
}

/*
  Enables or disables Nagle's algorithm on a given TCP Connection.

  :arg enable: whether to enable or disable Nagle's algorithm
  :type enable: `bool`

  :throws SystemError: if not able to set `TCP_NODELAY` flag properly
*/
proc ref tcpConn.setNagle(enable:bool) throws {
  var socketFd = this.socketFd;
  nagle(socketFd, enable);
}

/*
  Enables or disables Delayed Ack optimization on a given TCP Connection.

  :arg enable: whether to enable or disable Nagle's algorithm
  :type enable: `bool`

  :throws SystemError: if not able to set `TCP_QUICKACK` flag properly
*/
proc tcpConn.setDelayAck(enable:bool) throws {
  var socketFd = this.socketFd;
  delayAck(socketFd, enable);
}

pragma "no doc"
inline operator !=(const ref lhs: tcpConn,const ref rhs: tcpConn) {
  return lhs.socketFd != rhs.socketFd;
}

pragma "no doc"
inline operator ==(const ref lhs: tcpConn,const ref rhs: tcpConn) {
  return lhs.socketFd == rhs.socketFd;
}

pragma "no doc"
proc tcpConn.writeThis(f) throws {
  f.write("(","addr:",this.addr,",fd:",this.socketFd,")");
}

pragma "no doc"
private extern proc sizeof(e): c_size_t;

pragma "no doc"
extern "struct event_base" record event_base {};
pragma "no doc"
extern "struct event" record event {};
pragma "no doc"
extern type pthread_t;
pragma "no doc"
extern type pthread_attr_t;

private extern const EV_TIMEOUT:c_short;
private extern const EV_READ:c_short;
private extern const EV_WRITE:c_short;
private extern const EV_SIGNAL:c_short;
private extern const EV_PERSIST:c_short;
private extern const EV_ET:c_short;
private extern const EV_FINALIZE:c_short;
private extern const EV_CLOSED:c_short;
private extern const EVLOOP_NO_EXIT_ON_EMPTY:c_int;

private extern proc event_base_new():c_ptr(event_base);
private extern proc event_base_dispatch(base: c_ptr(event_base)):c_int;
private extern proc event_base_loop(base: c_ptr(event_base), flags: c_int):c_int;
private extern proc event_base_free(base: c_ptr(event_base));
private extern proc event_base_got_break(base: c_ptr(event_base)):c_int;
private extern proc event_base_loopbreak(base: c_ptr(event_base)):c_int;
private extern proc event_new(base: c_ptr(event_base), fd: c_int, events: c_short,
                              callback: c_fn_ptr, callback_arg: c_void_ptr): c_ptr(event);
private extern proc event_add(ev: c_ptr(event), timeout: c_ptr(timeval)):c_int;
private extern proc event_del(ev: c_ptr(event)):c_int;
private extern proc event_free(ev: c_ptr(event)):c_int;
private extern proc event_remove_timer(ev: c_ptr(event)):c_int;
private extern proc evutil_make_socket_nonblocking(fd: c_int):c_int;
private extern proc libevent_global_shutdown();
private extern proc evthread_use_pthreads();
private extern proc pthread_create(thread: c_ptr(pthread_t), const attr: c_ptr(pthread_attr_t),
                                   start_routine: c_fn_ptr, arg: c_void_ptr): c_int;
private extern proc pthread_join(thread: pthread_t, retval: c_ptr(c_void_ptr)): c_int;

pragma "no doc"
var event_loop_base:c_ptr(event_base);

/*
  A record holding reference to a tcp socket
  bound and listening for connections.
*/
record tcpListener {
  /*
    File Descriptor Associated with instance
  */
  var socketFd: int(32) = -1;

  pragma "no doc"
  proc init() {
    try! this.socketFd = socket();
    try! bind(this.socketFd, ipAddr.ipv4(port = 0), true);
  }

  pragma "no doc"
  proc init(socketFd: c_int) {
    this.socketFd = socketFd;
    try! setBlocking(this.socketFd, false);
  }

  proc deinit() {
    if this.socketFd != -1 {
      sys_close(this.socketFd);
    }
  }
}

/*
  Waits for a new connection based on `timeout` and
  returns a new :type:`tcpConn` if successful. Default
  time to wait for a new connection is indefinite.

  .. code-block:: Chapel

    const server = listen(ipAddr.create());
    const client = server.accept()

  :arg timeout: time to wait for new connection.
  :type timeval: :type:`~Sys.timeval`
  :return: accepted connection
  :rtype: `tcpConn`
  :throws Error: Upon timeout completion without
                  any new connection
*/
proc tcpListener.accept(in timeout: timeval = new timeval(-1,0)):tcpConn throws {
  var client_addr:sys_sockaddr_t = new sys_sockaddr_t();
  var fdOut:fd_t;
  var err_out:qio_err_t = 0;
  // try accept
  err_out = sys_accept(socketFd, client_addr, fdOut);
  // if error is not about blocking, throw error
  if err_out != 0 && err_out != EAGAIN && err_out != EWOULDBLOCK {
    throw SystemError.fromSyserr(err_out, "accept() failed");
  }
  // successfully return file
  if err_out == 0 {
    return openfd(fdOut):tcpConn;
  }
  var localSync$: sync c_short;
  // create event pending state
  var internalEvent = event_new(event_loop_base, this.socketFd, EV_READ | EV_TIMEOUT, c_ptrTo(syncRWTCallback), c_ptrTo(localSync$):c_void_ptr);
  defer {
    // cleanup
    event_free(internalEvent);
  }

  while err_out != 0 {
    var t: Timer;
    t.start();
    // make event active
    err_out = event_add(internalEvent, if timeout.tv_sec == -1 then nil else c_ptrTo(timeout));
    if err_out != 0 {
      throw new Error("accept() failed");
    }
    // return value
    var retval = localSync$.readFE();
    // stop timer
    t.stop();
    // if error was timeout throw error
    if retval & EV_TIMEOUT != 0 {
      throw SystemError.fromSyserr(ETIMEDOUT, "accept() timed out");
    }
    var elapsedTime = t.elapsed(TimeUnits.microseconds):c_long;
    // try accept again
    err_out = sys_accept(socketFd, client_addr, fdOut);
    if err_out != 0 {
      // error was not about blocking wait so throw it
      if err_out != EAGAIN && err_out != EWOULDBLOCK {
        throw SystemError.fromSyserr(err_out, "accept() failed");
      }
      // no indefinitely blocking wait
      if timeout.tv_sec != -1 {
        var totalTimeout = timeout.tv_sec*1000000 + timeout.tv_usec;
        // timer didn't elapsed
        if totalTimeout > t.elapsed(TimeUnits.microseconds) {
          const remainingMicroSeconds = ((totalTimeout - elapsedTime)%1000000);
          const remainingSeconds = ((totalTimeout - elapsedTime)/1000000);
          timeout.tv_sec = remainingSeconds;
          timeout.tv_usec = remainingMicroSeconds;
        }
        throw SystemError.fromSyserr(ETIMEDOUT, "accept() timed out");
      }
    }
  }
  return openfd(fdOut):tcpConn;
}

proc tcpListener.accept(timeout: real): tcpConn throws {
  return this.accept(timeout:timeval);
}

/*
  Close the file descriptor
*/
proc ref tcpListener.close() throws {
  var err_out = sys_close(this.socketFd);
  if err_out != 0 {
    throw SystemError.fromSyserr(err_out, "Failed to close tcpListener");
  }
  this.socketFd = -1;
}

/*
  Returns the address on which socket is
  listening on and bound to.

  :return: bound address
  :rtype: `ipAddr`
*/
proc tcpListener.addr throws {
  return getSockName(this.socketFd);
}

/*
  Enables or disables Nagle's algorithm on a given TCP Listener.

  :arg enable: whether to enable or disable Nagle's algorithm
  :type enable: `bool`

  :throws SystemError: if not able to set `TCP_NODELAY` option properly
*/
proc ref tcpListener.setNagle(enable:bool) throws {
  var socketFd = this.socketFd;
  nagle(socketFd, enable);
}

/*
  Enables or disables Delayed Ack optimization on a given TCP Listener.

  :arg enable: whether to enable or disable Nagle's algorithm
  :type enable: `bool`

  :throws SystemError: if not able to set `TCP_QUICKACK` flag properly
*/
proc ref tcpListener.setDelayAck(enable:bool) throws {
  var socketFd = this.socketFd;
  delayAck(socketFd, enable);
}

pragma "no doc"
inline operator !=(const ref lhs: tcpListener,const ref rhs: tcpListener) {
  return lhs.socketFd != rhs.socketFd;
}

pragma "no doc"
inline operator ==(const ref lhs: tcpListener,const ref rhs: tcpListener) {
  return !(lhs.socketFd != rhs.socketFd);
}

pragma "no doc"
proc tcpListener.writeThis(f) throws {
  f.write("(","addr:",this.addr,",fd:",this.socketFd);
}

pragma "no doc"
extern const SOMAXCONN: int;
/*
  Default `backlog` value used in :proc:`listen`
  It is calculated as min(`SOMAXCONN`, 128) where `SOMAXCONN` is
  the maximum number of allowed pending connections in the system.
*/
const backlogDefault:uint(16) = (if SOMAXCONN <= 128 then SOMAXCONN else 128):uint(16);

/*
  Convenience procedure which creates a new :type:`tcpListener` bound
  to and listening on `address` for new connections. `backlog`
  determines how many connections can be pending (not having called
  accept) before the socket will begin to reject them. The default
  value of backlog is `backlogDefault`.

  .. code-block:: Chapel

    const address = ipAddr.create("127.0.0.1", 8000, IPFamily.IPv4);
    const server = listen(address);

  :arg address: address to connect to
  :type address: :type:`ipAddr`
  :arg timeout: standard ipv6 address.
  :type timeval: :type:`~Sys.timeval`
  :return: connected socket
  :rtype: `tcpConn`
  :throws SystemError: On failure to bind or listen on `address`
*/
proc listen(in address: ipAddr, reuseAddr: bool = true,
            backlog: uint(16) = backlogDefault): tcpListener throws {
  var family = address.family;
  var socketFd = socket(family, SOCK_STREAM);
  bind(socketFd, address, reuseAddr);
  var err_out = sys_listen(socketFd, backlog:c_int);
  if err_out != 0 {
    throw SystemError.fromSyserr(err_out, "Failed to listen on socket");
  }
  const listener = new tcpListener(socketFd);
  return listener;
}

/*
  Convenience procedure which creates a :type:`tcpConn` connected to
  `address`.`timeout` determines how much time to wait for
  connection to be established. The default value for `timeout` is
  indefinite.

  .. code-block:: Chapel

    const address = ipAddr.create("127.0.0.1", 8000, IPFamily.IPv4);
    const timeout = new timeval(4,0);
    const connectedClient = connect(address, timeout);

  :arg address: address to connect to
  :type address: :type:`ipAddr`
  :arg timeout: time to wait for connection establishment.
  :type timeval: :type:`~Sys.timeval`
  :return: connected socket
  :rtype: `tcpConn`
  :throws SystemError: Upon failure to connect
*/
proc connect(const ref address: ipAddr, in timeout = new timeval(-1,0)): tcpConn throws {
  var family = address.family;
  var socketFd = socket(family, SOCK_STREAM);
  setBlocking(socketFd, false);
  var err_out = sys_connect(socketFd, address._addressStorage);
  if err_out != 0 && err_out != EINPROGRESS {
    sys_close(socketFd);
    throw SystemError.fromSyserr(err_out,"connect() failed");
  }
  if err_out == 0 {
    setBlocking(socketFd, true);
    return openfd(socketFd):tcpConn;
  }
  var localSync$: sync int = 0;
  localSync$.readFE();
  var writerEvent = event_new(event_loop_base, socketFd, EV_WRITE | EV_TIMEOUT, c_ptrTo(syncRWTCallback), c_ptrTo(localSync$):c_void_ptr);
  defer {
    event_del(writerEvent);
    event_free(writerEvent);
  }
  err_out = event_add(writerEvent, if timeout.tv_sec == -1 then nil else c_ptrTo(timeout));
  if err_out != 0 {
    throw new Error("connect() failed");
  }
  var retval = localSync$.readFE();
  if retval & EV_TIMEOUT != 0 {
    throw SystemError.fromSyserr(ETIMEDOUT, "connect() timed out");
  }
  err_out = sys_connect(socketFd, address._addressStorage);
  if err_out != 0 {
    sys_close(socketFd);
    throw SystemError.fromSyserr(err_out,"connect() failed");
  }
  setBlocking(socketFd, true);
  return openfd(socketFd):tcpConn;
}

proc connect(const ref address: ipAddr, in timeout:real): tcpConn throws {
  return connect(address, timeout:timeval);
}

/*
  This overload of `connect` not only returns a :type:`tcpConn`
  but also does DNS resolution for the provided `host`.
  The `timeout` is tried for all resolved addresses and the first
  successful one is returned back.

  .. code-block:: Chapel

    const timeout = new timeval(4,0);
    const connectedClient = connect("google.com", "http", IPFamily.IPv4, timeout);

  :arg host: host to connect to or resolve if not in standard ip notation
  :type host: `string`
  :arg service: service to connect to on resolved `host`
  :type service: `string`
  :arg family: type of socket family to connect to
  :type family: :type:`IPFamily`
  :arg timeout: time to wait for each possible connection.
  :type timeval: :type:`~Sys.timeval`
  :return: connected socket
  :rtype: `tcpConn`
  :throws SystemError: Upon failure to resolve address or connect
                        to any of the resolved address in given `timeout`.
*/
proc connect(in host: string, in service: string, family: IPFamily = IPFamily.IPUnspec,
             in timeout = new timeval(-1,0)): tcpConn throws {
  var result:sys_addrinfo_ptr_t;
  var hints = new sys_addrinfo_t();
  hints.ai_family = family:c_int;
  hints.ai_socktype = SOCK_STREAM;
  var err = getaddrinfo(host.c_str(), service.c_str(), hints, result);
  if err != 0 {
    throw new Error("Can't resolve address");
  }
  var tempPointer = result;
  var conn:tcpConn;
  while tempPointer != nil {
    var address = new ipAddr(tempPointer.addr);
    try {
      conn = connect(address, timeout);
      break;
    }
    catch {
      tempPointer = tempPointer.next;
      continue;
    }
  }
  sys_freeaddrinfo(result);
  if tempPointer == nil {
    throw new Error("Can't resolve address");
  }
  return conn;
}

proc connect(in host: string, in service: string, family: IPFamily = IPFamily.IPUnspec,
             timeout:real): tcpConn throws {
  return connect(host, service, family, timeout:timeval);
}

/*
  This overload of `connect` not only returns a :type:`tcpConn`
  but also does DNS resolution for the provided `host`.
  The `timeout` is tired for all resolved addresses and the first
  successful one is returned back.

  .. code-block:: Chapel

    const timeout = new timeval(4,0);
    const connectedClient = connect("google.com", 80, IPFamily.IPv4, timeout);

  :arg host: address of host to connect or resolve if not in ip notation
  :type host: `string`
  :arg port: port to connect to on `host`
  :type port: `uint(16)`
  :arg family: type of socket family to connect to
  :type family: :type:`IPFamily`
  :arg timeout: time to wait for each possible connection.
  :type timeval: :type:`~Sys.timeval`
  :return: connected socket
  :rtype: `tcpConn`
  :throws SystemError: Upon failure to resolve address or connect
                    to any of the resolved address in given `timeout`.
*/
proc connect(in host: string, in port: uint(16), family: IPFamily = IPFamily.IPUnspec,
             timeout = new timeval(-1,0)): tcpConn throws {
  return connect(host, port:string, family, timeout);
}

proc connect(in host: string, in port: uint(16), family: IPFamily = IPFamily.IPUnspec,
             timeout:real): tcpConn throws {
  return connect(host, port, family, timeout:timeval);
}

/*
  A record holding reference to a udp socket
  bound to any available port.
*/
record udpSocket {
  /*
    File Descriptor Associated with instance
  */
  var socketFd: int(32);
  /* Create a UDP socket of provided Family. */
  proc init(family: IPFamily = IPFamily.IPv4) {
    this.socketFd = -1;
    try! this.socketFd = socket(family, SOCK_DGRAM);
    try! setBlocking(this.socketFd, false);
  }

  proc deinit() {
    if this.socketFd != -1 {
      sys_close(this.socketFd);
    }
  }
}

/* Get :type:`ipAddr` associated with udp socket */
proc udpSocket.addr throws {
  return getSockName(this.socketFd);
}

proc udpSocket.close throws {
  var err_out = sys_close(this.socketFd);
  if err_out != 0 {
    throw SystemError.fromSyserr(err_out, "Failed to close udpSocket");
  }
}

pragma "no doc"
private extern proc sys_recvfrom(sockfd:fd_t, buff:c_void_ptr, len:c_size_t, flags:c_int, ref src_addr_out:sys_sockaddr_t, ref num_recvd_out:c_ssize_t):qio_err_t;

/*
  Reads upto `bufferLen` bytes from the socket, and
  return a tuple of (data, address), where address will be a
  :type:`ipAddr` pointing to address of the socket from where data was received.

  .. code-block:: Chapel

    const timeout = new timeval(4,0);
    const socket = new udpSocket();
    const (data, sender) = socket.recvFrom(40, timeout);

  :arg bufferLen: number of bytes to read
  :type bufferLen: `int`
  :arg timeout: time to wait for data to arrive.
  :type timeval: :type:`~Sys.timeval`
  :return: tuple of (data, address)
  :rtype: (:type:`~Bytes.bytes`, :type:`ipAddr`)
  :throws SystemError: Upon failure to receive any data
                    within given `timeout`.
*/
proc udpSocket.recvfrom(bufferLen: int, in timeout = new timeval(-1,0),
                        flags:c_int = 0):(bytes, ipAddr) throws {
  var err_out:qio_err_t = 0;
  var buffer = c_calloc(c_uchar, bufferLen);
  var length:c_ssize_t;
  var addressStorage = new sys_sockaddr_t();
  err_out = sys_recvfrom(this.socketFd, buffer, bufferLen:c_size_t, 0, addressStorage, length);
  if err_out == 0 {
    return (createBytesWithOwnedBuffer(buffer, length, bufferLen), new ipAddr(addressStorage));
  }
  if err_out != 0 && err_out != EAGAIN && err_out != EWOULDBLOCK {
    c_free(buffer);
    throw SystemError.fromSyserr(err_out,"recv failed");
  }
  var localSync$: sync c_short;
  var internalEvent = event_new(event_loop_base, this.socketFd, EV_READ | EV_TIMEOUT, c_ptrTo(syncRWTCallback), c_ptrTo(localSync$):c_void_ptr);
  defer {
    event_free(internalEvent);
  }

  while err_out != 0 {
    var t: Timer;
    t.start();
    err_out = event_add(internalEvent, if timeout.tv_sec == -1 then nil else c_ptrTo(timeout));
    if err_out != 0 {
      c_free(buffer);
      throw new Error("recv failed");
    }
    var retval = localSync$.readFE();
    t.stop();
    if retval & EV_TIMEOUT != 0 {
      c_free(buffer);
      throw SystemError.fromSyserr(ETIMEDOUT, "recv timed out");
    }
    var elapsedTime = t.elapsed(TimeUnits.microseconds):c_long;
    err_out = sys_recvfrom(this.socketFd, buffer, bufferLen:c_size_t, 0, addressStorage, length);
    if err_out != 0 {
      if err_out != EAGAIN && err_out != EWOULDBLOCK {
        c_free(buffer);
        throw SystemError.fromSyserr(err_out,"recv failed");
      }
      if timeout.tv_sec == -1 {
        var totalTimeout = timeout.tv_sec*1000000 + timeout.tv_usec;
        if totalTimeout >= t.elapsed(TimeUnits.microseconds) {
          const remainingMicroSeconds = ((totalTimeout - elapsedTime)%1000000);
          const remainingSeconds = ((totalTimeout - elapsedTime)/1000000);
          timeout.tv_sec = remainingSeconds;
          timeout.tv_usec = remainingMicroSeconds;
        }
        c_free(buffer);
        throw SystemError.fromSyserr(ETIMEDOUT, "recv timed out");
      }
    }
  }
  return (createBytesWithOwnedBuffer(buffer, length, bufferLen), new ipAddr(addressStorage));
}

proc udpSocket.recvfrom(bufferLen: int, timeout: real, flags:c_int = 0):(bytes, ipAddr) throws {
  return this.recvfrom(bufferLen, timeout:timeval, flags);
}

/*
  Reads incoming `bufferLen` number of bytes on socket, and
  return a tuple of read bytes, which can have size smaller than asked and if
  the size is more they will be truncated.

  .. code-block:: Chapel

    const timeout = new timeval(4,0);
    const socket = new udpSocket();
    const data = socket.recv(40, timeout);

  :arg bufferLen: number of bytes to read
  :type bufferLen: `int`
  :arg timeout: time to wait for data to arrive.
  :type timeval: :type:`~Sys.timeval`
  :return: data
  :rtype: :type:`~Bytes.bytes`
  :throws SystemError: Upon failure to receive any data
                        within given `timeout`.
*/
proc udpSocket.recv(bufferLen: int, in timeout = new timeval(-1,0)) throws {
  var (data, _) = this.recvfrom(bufferLen, timeout);
  return data;
}

proc udpSocket.recv(bufferLen: int, timeout: real) throws {
  return this.recv(bufferLen, timeout:timeval);
}

pragma "no doc"
private extern proc sys_sendto(sockfd:fd_t, buff:c_void_ptr, len:c_long, flags:c_int, const ref address:sys_sockaddr_t,  ref num_sent_out:c_ssize_t):qio_err_t;

/*
  Send `data` over socket to the provided address and
  return number of bytes sent if successful.

  .. code-block:: Chapel

    const timeout = new timeval(4,0);
    const socket = new udpSocket();
    const sentBytes = socket.send("hello world!":bytes, timeout);

  :arg data: data to send to address
  :type data: :type:`~Bytes.bytes`
  :arg address: socket address for sending data
  :type address: :type:`ipAddr`
  :arg timeout: time to wait for data to arrive.
  :type timeval: :type:`~Sys.timeval`
  :return: sentBytes
  :rtype: `c_ssize_t`
  :throws SystemError: Upon failure to send any data
                        within given `timeout`.
*/
proc udpSocket.send(data: bytes, in address: ipAddr,
                    in timeout = new timeval(-1,0)):c_ssize_t throws {
  var err_out:qio_err_t = 0;
  var length:c_ssize_t;
  err_out = sys_sendto(this.socketFd, data.c_str():c_void_ptr, data.size:c_long, 0, address._addressStorage, length);
  if err_out == 0 {
    return length;
  }
  if err_out != 0 && err_out != EAGAIN && err_out != EWOULDBLOCK {
    throw SystemError.fromSyserr(err_out, "send failed");
  }
  var localSync$: sync c_short;
  var internalEvent = event_new(event_loop_base, this.socketFd, EV_WRITE | EV_TIMEOUT, c_ptrTo(syncRWTCallback), c_ptrTo(localSync$):c_void_ptr);
  defer {
    event_free(internalEvent);
  }
  while err_out != 0 {
    var t: Timer;
    t.start();
    err_out = event_add(internalEvent, if timeout.tv_sec == -1 then nil else c_ptrTo(timeout));
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "send failed");
    }
    var retval = localSync$.readFE();
    t.stop();
    if retval & EV_TIMEOUT != 0 {
      throw SystemError.fromSyserr(ETIMEDOUT, "send timed out");
    }
    var elapsedTime = t.elapsed(TimeUnits.microseconds):c_long;
    err_out = sys_sendto(this.socketFd, data.c_str():c_void_ptr, data.size:c_long, 0, address._addressStorage, length);
    if err_out != 0 {
      if err_out != EAGAIN && err_out != EWOULDBLOCK {
        throw SystemError.fromSyserr(err_out, "send failed");
      }
      if timeout.tv_sec == -1 {
        var totalTimeout = timeout.tv_sec*1000000 + timeout.tv_usec;
        if totalTimeout >= t.elapsed(TimeUnits.microseconds) {
          const remainingMicroSeconds = ((totalTimeout - elapsedTime)%1000000);
          const remainingSeconds = ((totalTimeout - elapsedTime)/1000000);
          timeout.tv_sec = remainingSeconds;
          timeout.tv_usec = remainingMicroSeconds;
        }
        throw SystemError.fromSyserr(ETIMEDOUT, "send timed out");
      }
    }
  }
  return length;
}

proc udpSocket.send(data: bytes, in address: ipAddr, timeout: real) throws {
  return this.send(data, address, timeout:timeval);
}

pragma "no doc"
inline operator !=(const ref lhs: udpSocket,const ref rhs: udpSocket) {
  return lhs.socketFd != rhs.socketFd;
}

pragma "no doc"
inline operator ==(const ref lhs: udpSocket,const ref rhs: udpSocket) {
  return lhs.socketFd == rhs.socketFd;
}

pragma "no doc"
proc udpSocket.writeThis(f) throws {
  f.write("(","addr:",this.addr,",fd:",this.socketFd);
}

pragma "no doc"
proc setSockOpt(socketFd: fd_t, level: c_int, optname: c_int, ref value: c_int) throws {
  var optlen = sizeof(value):socklen_t;
  var ptroptval = c_ptrTo(value);
  var err_out = sys_setsockopt(socketFd, level, optname, ptroptval:c_void_ptr, optlen);
  if err_out != 0 {
    throw SystemError.fromSyserr(err_out, "Failed to set socket option");
  }
}

/*
  Set the value of the given socket option (see `setsockopt(2) </https://pubs.opengroup.org/onlinepubs/9699919799/functions/setsockopt.html#>`_)
  on provided :type:`tcpConn`. The needed symbolic constants (SO_* etc.)
  are defined in the :mod:`Sys`.

  .. code-block:: Chapel

    setSockOpt(socket, IPPROTO_TCP, TCP_QUICKACK, 1:c_int);

  :arg socket: socket to set option on
  :type socket: `tcpConn` or `udpSocket` or `tcpListener`
  :arg level: protocol level
  :type level: `int(32)`
  :arg optname: option to set.
  :type optname: `int(32)`
  :arg value: value to set on option
  :type value: `int(32)`
  :throws SystemError: Upon incompatible arguments
                        and socket.
*/
proc setSockOpt(ref socket: ?t, level: c_int, optname: c_int, value: c_int)
                throws where t == udpSocket || t == tcpConn || t == tcpListener {
  var socketFd = socket.socketFd;
  setSockOpt(socketFd, level, optname, value);
}

pragma "no doc"
proc setSockOpt(socketFd:fd_t, level: c_int, optname: c_int, ref value: bytes) throws {
  var optlen = value.size:socklen_t;
  var ptroptval = value.c_str();
  var err_out = sys_setsockopt(socketFd, level, optname, ptroptval, optlen);
  if err_out != 0 {
    throw SystemError.fromSyserr(err_out, "Failed to set socket option");
  }
}

/*
  Overload for :proc:`setSockOpt` that allows setting a :type:`~Bytes.bytes` value
  on socket option. This is useful for `setsockopt` calls that work with a C struct,
  including `SO_LINGER`, `SO_RCVTIMEO`, and `SO_SNDTIMEO`. It is up to the caller to
  ensure that the `value` which is a :type:`~Bytes.bytes` parameter contains
  the proper bits.

  :arg socket: socket to set option on
  :type socket: `tcpConn` or `tcpListener` or `udpSocket`
  :arg level: protocol level
  :type level: `int(32)`
  :arg optname: option to set.
  :type optname: `int(32)`
  :arg value: value to set on option
  :type value: :type:`~Bytes.bytes`
  :throws SystemError: Upon incompatible arguments
                        and socket.
*/
proc setSockOpt(ref socket: ?t, level: c_int, optname: c_int, value: bytes)
                throws where t == udpSocket || t == tcpConn || t == tcpListener {
  var socketFd = socket.socketFd;
  setSockOpt(socketFd, level, optname, value);
}

pragma "no doc"
proc setSockOpt(socketFd:fd_t, level: c_int, optname: c_int, value:nothing, optlen:socklen_t) throws {
  var err_out = sys_setsockopt(socketFd, level, optname, nil, optlen);
  if err_out != 0 {
    throw SystemError.fromSyserr(err_out, "Failed to set socket option");
  }
}

/*
  Overload for :proc:`setSockOpt` which is equivalent to calling setsockopt() C
  function with `optval=NULL` and `optlen=optlen`.

  :arg socket: socket to set option on
  :type socket: `tcpConn` or `tcpListener` or `udpSocket`
  :arg level: protocol level
  :type level: `int(32)`
  :arg optname: option to set.
  :type optname: `int(32)`
  :arg value: None
  :type value: `nothing`
  :arg optlen: size of option
  :type optlen: `int(32)`
  :throws SystemError: Upon incompatible arguments
                        and socket.
*/
proc setSockOpt(ref socket: ?t, level: c_int, optname: c_int, value: nothing, optlen: socklen_t)
                throws where t == udpSocket || t == tcpConn || t == tcpListener {
  var socketFd = socket.socketFd;
  setSockOpt(socketFd, level, optname, value, optlen);
}

pragma "no doc"
proc getSockOpt(socketFd:fd_t, level: c_int, optname: c_int) throws {
  var optval:c_int;
  var ptroptval = c_ptrTo(optval);
  var optlen = sizeof(optval):socklen_t;
  var err_out = sys_getsockopt(socketFd, level, optname, ptroptval:c_void_ptr, optlen);
  if err_out != 0 {
    throw SystemError.fromSyserr(err_out, "Failed to get socket option");
  }
  return optval;
}

/*
  Returns the value of the given socket option (see `getsockopt </https://pubs.opengroup.org/onlinepubs/9699919799/functions/getsockopt.html>`_)
  on provided :type:`tcpConn`. The needed symbolic constants (SO_* etc.)
  are defined in :mod:`Sys` module.

  :arg socket: socket to set option on
  :type socket: `tcpConn` or `udpSocket` or `tcpListener`
  :arg level: protocol level
  :type level: `int(32)`
  :arg optname: option to set.
  :type optname: `int(32)`
  :return: value of socket option
  :rtype: `int(32)`
  :throws SystemError: Upon incompatible arguments
                        and socket.
*/
proc getSockOpt(ref socket: ?t, level: c_int, optname: c_int): int(32)
                throws where t == udpSocket || t == tcpConn || t == tcpListener {
  var socketFd = socket.socketFd;
  return getSockOpt(socketFd, level, optname) ;
}

pragma "no doc"
proc getSockOpt(socketFd:fd_t, level: c_int, optname: c_int, buflen: uint(16)) throws {
  if buflen < 0 || buflen > 1024 {
    throw new Error("getSockOpt buflen out of range");
  }
  else {
    var len:socklen_t = buflen;
    var buffer = c_calloc(c_uchar, buflen);
    var err_out = sys_getsockopt(socketFd, level, optname, buffer:c_void_ptr, len);
    if err_out != 0 {
      c_free(buffer);
      throw SystemError.fromSyserr(err_out, "Failed to get socket option");
    }
    return createBytesWithOwnedBuffer(buffer, len, buflen);
  }
}

/*
  Returns the value of the given socket option which is expected to be of type
  :type:`~Bytes.bytes` on provided :type:`tcpConn`. The needed symbolic constants (SO_* etc.)
  are defined in :mod:`Sys` module.

  :arg socket: socket to set option on
  :type socket: `tcpConn` or `udpSocket` or `tcpListener`
  :arg level: protocol level
  :type level: `int(32)`
  :arg optname: option to set.
  :type optname: `int(32)`
  :return: value of socket option
  :rtype: :type:`~Bytes.bytes`
  :throws SystemError: Upon incompatible arguments
                        and socket.
*/
proc getSockOpt(ref socket: ?t, level: c_int, optname: c_int, buflen: uint(16)):
                bytes throws where t == udpSocket || t == tcpConn || t == tcpListener  {
  var socketFd = socket.socketFd;
  return getSockOpt(socketFd, level, optname, buflen) ;
}

pragma "no doc"
proc getPeerName(socketFD: fd_t) throws {
  var addressStorage = new sys_sockaddr_t();
  var err = sys_getpeername(socketFD, addressStorage);
  if err != 0 {
    throw SystemError.fromSyserr(err, "Failed to get remote address");
  }
  return new ipAddr(addressStorage);
}

/*
  Returns the remote address to which socket is connected. This is
  useful to find out the address and port number of a
  remote IPv4/v6 socket, for instance.

  :arg socket: socket to set option on
  :type socket: `tcpConn`

  :return: remote address
  :rtype: `ipAddr`
  :throws SystemError: If socket is not connected
*/
proc getPeerName(ref socket: tcpConn): ipAddr throws {
  var socketFd = socket.socketFd;
  return getPeerName(socketFd);
}

pragma "no doc"
proc getSockName(socketFD: fd_t) throws {
  var addressStorage = new sys_sockaddr_t();
  var err = sys_getsockname(socketFD, addressStorage);
  if err != 0 {
    throw SystemError.fromSyserr(err, "Failed to get local address");
  }
  return new ipAddr(addressStorage);
}

/*
  Returns the socket's own address. This is useful to find out the port
  number of a IPv4/v6 socket, for instance.

  :arg socket: socket to set option on
  :type socket: `tcpConn` or `udpSocket` or `tcpListener`

  :return: remote address
  :rtype: `ipAddr`
  :throws SystemError: If socket is closed
*/
proc getSockName(ref socket: ?t): ipAddr throws
                 where t == udpSocket || t == tcpConn || t == tcpListener  {
  var socketFd = socket.socketFd;
  return getSockName(socketFd);
}

pragma "no doc"
proc socket(family:IPFamily = IPFamily.IPv4, sockType:c_int = SOCK_STREAM, protocol = 0) throws {
  var socketFd: c_int;
  var err = sys_socket(family:c_int, sockType, protocol:c_int, socketFd);
  if err != 0 {
    throw SystemError.fromSyserr(err, "Failed to create socket");
  }
  return socketFd;
}

pragma "no doc"
proc setBlocking(socketFd: fd_t, blocking: bool) throws {
  var flags:c_int;
  var err = sys_fcntl(socketFd, F_GETFL, flags);
  if err != 0 {
    throw SystemError.fromSyserr(err, "Failed to get socket flags");
  }
  if blocking {
    flags &= ~OS.POSIX.O_NONBLOCK;
  }
  else {
    flags |= OS.POSIX.O_NONBLOCK;
  }
  err = sys_fcntl_long(socketFd, F_SETFL, flags, flags);
  if err != 0 {
    throw SystemError.fromSyserr(err, "Failed to make socket non blocking");
  }
}

pragma "no doc"
proc bind(socketFd:fd_t, ref address: ipAddr, reuseAddr = true) throws {
  var enable = (if reuseAddr then 1 else 0):c_int;
  setSockOpt(socketFd, SOL_SOCKET, SO_REUSEADDR, enable);
  var err = sys_bind(socketFd, address._addressStorage);
  if err != 0 {
    throw SystemError.fromSyserr(err, "Failed to bind Socket");
  }
}


/*
  Bind the socket to address. The socket must not already
  be bound to any address prior to calling this procedure.

  .. code-block:: Chapel

    var socket =  new udpSocket();
    var address = ipAddr.create("127.0.0.1", 8111);
    bind(socket, address);

  :arg socket: socket to set option on
  :type socket: `tcpConn` or `udpSocket` or `tcpListener`
  :arg address: address to bind to
  :type address: `ipAddr`
  :arg reuseAddr: whether to reuse address if already in use
  :type address: `boolean`

  :throws SystemError: If socket is closed or already bound
*/
proc bind(ref socket: ?t, ref address: ipAddr, reuseAddr = true)
         throws where t == udpSocket || t == tcpConn || t == tcpListener {
  var socketFd = socket.socketFd;
  bind(socketFd, address, reuseAddr);
}

pragma "no doc"
proc nagle(socketFd:fd_t, enable:bool) throws {
  var c_enable = (if enable then 0 else 1):c_int;
  setSockOpt(socketFd, IPPROTO_TCP, TCP_NODELAY, c_enable);
}

pragma "no doc"
proc nagle(socketFd:fd_t):bool throws {
  return if getSockOpt(socketFd, IPPROTO_TCP, TCP_NODELAY) == 0 then true else false;
}

pragma "no doc"
proc delayAck(socketFd:fd_t, enable:bool) throws {
  var c_enable = (if enable then 0 else 1):c_int;
  setSockOpt(socketFd, IPPROTO_TCP, TCP_QUICKACK, c_enable);
}

pragma "no doc"
proc delayAck(socketFd:fd_t):bool throws {
  return if getSockOpt(socketFd, IPPROTO_TCP, TCP_QUICKACK) == 0 then true else false;
}

evthread_use_pthreads();
event_loop_base = event_base_new();
pragma "no doc"
var event_loop_thread:pthread_t;

pragma "no doc"
proc dispatchLoop():c_void_ptr throws {
  if event_loop_base == nil {
    throw new Error("event loop wasn't initialized");
  }
  if event_base_got_break(event_loop_base) == 1 {
    return nil;
  }
  var x = event_base_loop(event_loop_base, EVLOOP_NO_EXIT_ON_EMPTY);
  if x != 0 {
    throw new Error("event loop wasn't initialized");
  }
  return nil;
}

pthread_create(c_ptrTo(event_loop_thread), nil:c_ptr(pthread_attr_t), c_ptrTo(dispatchLoop), nil);

pragma "no doc"
proc syncRWTCallback(fd: c_int, event: c_short, arg: c_void_ptr) {
  var syncVariablePtr = arg: c_ptr(sync int);
  syncVariablePtr.deref().writeEF(event);
}

pragma "no doc"
proc deinit() {
  event_base_loopbreak(event_loop_base);
  pthread_join(event_loop_thread, nil:c_ptr(c_void_ptr));
  event_base_free(event_loop_base);
  libevent_global_shutdown();
}
}
