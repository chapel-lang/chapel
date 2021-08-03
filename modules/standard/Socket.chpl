/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
  Socket Module library, specifically related to IP Sockets.

  The Socket module focuses on connecting, accepting sockets and providing interface for
  communication between the sockets. Also provided are some constant values representing
  common idioms in socket programming, such as standard Addresses, Families and Flags.
  For more constants that can used alongside refer :mod:`Sys`.

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
  :proc:`delayAck`
  :proc:`getpeername`
  :proc:`getSocketOpt`
  :proc:`getsockname`
  :proc:`listen`
  :proc:`naggle`
  :proc:`setSockOpt`
  :proc:`socket`

  Records, Types and Function Definitions
  ---------------------------------
*/
module Socket {

  public use Sys;
  public use SysError;
  use Time;
  use SysCTypes;
  use SysBasic;
  use CPtr;
  use IO;

  /*
    Available values for different Internet
    Protocol Families to be used when creating Sockets.
  */
  enum IPFamily {
    // IPv4
    IPv4 = 2,
    // IPv6
    IPv6 = 10,
    // IP Unspecified
    IPUnspec = 0
  }

  // Type of Standard IPv4 Address
  type ipv4Addr = sys_in_addr_t;
  // Standard IPv4 Addresses of type :type:`ipv4Addr`
  const IPv4Localhost:ipv4Addr = INADDR_LOOPBACK;
  const IPv4Any:ipv4Addr = INADDR_ANY;
  const IPv4Broadcast:ipv4Addr = INADDR_BROADCAST;

  // Type of Standard IPv6 Address
  type ipv6Addr = sys_in6_addr_t;
  // Standard IPv6 Addresses of type :type:`ipv6Addr`
  const IPv6Localhost:ipv6Addr = in6addr_loopback;
  const IPv6Any:ipv6Addr = in6addr_any;

  /*
    Abstract supertype for network addresses. Contains data
    about :type:`IPfamily`, `host` and `port`.
  */
  record ipAddr {
    pragma "no doc"
    var _addressStorage:sys_sockaddr_t;
  }

  pragma "no doc"
  proc ipAddr.init(in address:sys_sockaddr_t) {
    this._addressStorage = new sys_sockaddr_t();
    // this won't halt as sys_sockaddr_t was set without any errors
    try! this._addressStorage.set(address.numericHost().c_str(), address.port(), address.family);
  }

  /*
    Returns a new record of type :type:`ipAddr` prvoided :arg:`host`, :arg:`port`
    and :arg:`family`. this function is equivalent to the following code:
    .. code-block:: Chapel
      ipAddr.create("127.0.0.1", 8111, IPFamily.IPv4)

    :arg host: address string in dot-dash or colon notation depending on family.
    :type host: `string`
    :arg port: network address's port.
    :type port: `uint(16)`
    :arg family: value of IP Family
    :type family: `IPFamily`
    :return: address instance.
    :rtype: `ipAddr`
    :throws SystemError: Upon incompatible `host`, `port` or `family`
  */
  proc type ipAddr.create(host:string = "127.0.0.1", port:uint(16) = 8000, family:IPFamily = IPFamily.IPv4) throws {
    var addressStorage = new sys_sockaddr_t();
    addressStorage.set(host.c_str(), port, family:c_int);
    return new ipAddr(addressStorage);
  }

  /*
    Returns a new record of type `ipAddr` prvoided :arg:`host` and :arg:`port`.
    The family type is assumed based on :arg:`host` which is a standard address.
    this function is equivalent to the following code:
    .. code-block:: Chapel
      ipAddr.ipv4(IPv4LocalHost, 8111)

    :arg host: standard ipv4 address.
    :type host: `ipv4Addr`
    :arg port: network address's port.
    :type port: `uint(16)`
    :return: address instance.
    :rtype: `ipAddr`
    :throws SystemError: Upon incompatible `host`, `port` or `family`
  */
  proc type ipAddr.ipv4(host:ipv4Addr, port:uint(16) = 8000) throws {
    var addressStorage = new sys_sockaddr_t();
    addressStorage.set(host, port);
    return new ipAddr(addressStorage);
  }

  /*
    Returns a new record of type `ipAddr` prvoided :arg:`host` and :arg:`port`.
    The family type is assumed based on :arg:`host` which is a standard address.
    this function is equivalent to the following code:
    .. code-block:: Chapel
      ipAddr.ipv6(IPv6LocalHost, 8111)

    :arg host: standard ipv6 address.
    :type host: `ipv6Addr`
    :arg port: network address's port.
    :type port: `uint(16)`
    :return: address instance.
    :rtype: `ipAddr`
    :throws SystemError: Upon incompatible `host`, `port` or `family`
  */
  proc type ipAddr.ipv6(host:ipv6Addr, port:uint(16) = 8000) throws {
    var addressStorage = new sys_sockaddr_t();
    addressStorage.set(host, port);
    return new ipAddr(addressStorage);
  }

  /*
    Returns the family type of address.
    :return: family type of address
    :rtype: `IPFamily`
  */
  proc ipAddr.family {
    return try! _addressStorage.family:IPFamily;
  }

  /*
    Returns the host address.
    :return: host address
    :rtype: `string`
  */
  proc ipAddr.host {
    return try! _addressStorage.numericHost();
  }

  /*
    Returns the `port` stored in record.
    :return: Returns numeric port.
    :rtype: `uint(16)`
  */
  proc ipAddr.port {
    return try! _addressStorage.port();
  }

  // compare ipAddr
  inline operator !=(in lhs: ipAddr,in rhs: ipAddr) {
    return lhs.family != rhs.family || lhs.host != rhs.host || lhs.port != rhs.port;
  }

  // write ipAddr
  proc ipAddr.writeThis(f) throws {
    f.write("(","family:",this.family,",host:",this.host,",port:",this.port,")");
  }

  private extern proc qio_get_fd(fl:qio_file_ptr_t, ref fd:c_int):syserr;

  // Type TCPConn returned from :proc:`connect`
  type tcpConn = file;

  /*
    Returns the file descriptor associated with socket
    :return: Returns file descriptor.
    :rtype: `int(32)`
  */
  proc tcpConn.socketFd throws {
    var tempfd:c_int;
    var err:syserr = ENOERR;
    on this.home {
      err = qio_get_fd(this._file_internal, tempfd);
    }
    if err then try ioerror(err, "in tcpConn.socketFd");
    return tempfd;
  }

  /*
    Returns the address of remote socket connection
    :return: Returns remote address.
    :rtype: `ipAddr`
  */
  proc tcpConn.addr {
    return try! getpeername(this.socketFd);
  }

  inline operator !=(in lhs: tcpConn,in rhs: tcpConn) {
    return lhs.socketFd != rhs.socketFd;
  }

  proc tcpConn.writeThis(f) throws {
    f.write("(","addr:",this.addr,",fd:",this.socketFd);
  }

  pragma "no doc"
  private extern proc sizeof(e): size_t;

  /*
    A record holding reference to a tcp socket
    bound and listening for connections.
  */
  record tcpListener {
    // File Descriptor Associated with instance
    var socketFd:int(32);

    pragma "no doc"
    proc init(socketFd:c_int) {
      this.socketFd = socketFd;
    }
  }

  /*
    Waits for a new connection based on :arg:`timeout` and
    returns a new :type:`tcpConn` if successful. Default
    time to wait for a new connection is indefinite.

    :arg timeout: time to wait for new connection.
    :type timeval: :type:`~Sys.timeval`
    :return: accepted connection.
    :rtype: `tcpConn`
    :throws Error: Upon timeout completion without
                   any new connection
  */
  proc tcpListener.accept(in timeout:timeval = new timeval(-1,0)) throws {
    var client_addr:sys_sockaddr_t = new sys_sockaddr_t();
    var fdOut:fd_t;
    var rset, allset: fd_set;
    sys_fd_zero(allset);
    sys_fd_set(this.socketFd, allset);
    rset = allset;
    var nready:c_int;
    var err_out:err_t;
    if timeout.tv_sec == -1 {
      err_out = sys_select(socketFd+1, c_ptrTo(rset), nil, nil, nil, nready);
    }
    else {
      err_out = sys_select(socketFd+1, c_ptrTo(rset), nil, nil, c_ptrTo(timeout), nready);
    }

    if nready == 0 {
      throw SystemError.fromSyserr(ETIMEDOUT, "accept() timed out");
    }
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "accept() failed");
    }
    if(sys_fd_isset(socketFd, rset)){
      err_out = sys_accept(socketFd, client_addr, fdOut);
    }
    else{
      throw new Error("connection disconnected");
    }
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "accept() failed");
    }
    var sockFile:tcpConn = openfd(fdOut);
    return sockFile;
  }

  // Close the file descriptor
  proc tcpListener.close() {
    sys_close(this.socketFd);
  }

  /*
    Returns the address on which socket is
    listening on and bound to.

    :return: bound address
    :rtype: `ipAddr`
  */
  proc tcpListener.addr {
    return try! getsockname(this.socketFd);
  }

  inline operator !=(in lhs: tcpListener,in rhs: tcpListener) {
    return lhs.socketFd != rhs.socketFd;
  }

  proc tcpListener.writeThis(f) throws {
    f.write("(","addr:",this.addr,",fd:",this.socketFd);
  }

  pragma "no doc"
  extern const SOMAXCONN:int;
  // Default `backlog` value used in :proc:`listen`
  const BACKLOG_DEFAULT = if SOMAXCONN <= 128 then SOMAXCONN else 128;

  /*
    Convenience procedure which creates a new :type:`tcpListener` bound
    to and listening on :arg:`address` for new connections. :arg:`backlog`
    determines how many connections can be pending (not having called
    accept) before the socket will begin to reject them. The default
    value of backlog is `BACKLOG_DEFAULT`.

    :arg address: address to connect to
    :type address: :type:`ipAddr`
    :arg timeout: standard ipv6 address.
    :type timeval: :type:`~Sys.timeval`
    :return: connected socket.
    :rtype: `tcpConn`
    :throws SystemError: On failure to bind or listen on :arg:`address`
  */
  proc listen(in address:ipAddr, reuseAddr = true, backlog:integral = BACKLOG_DEFAULT) throws {
    var family = address.family;
    var socketFd = socket(family, SOCK_STREAM | SOCK_NONBLOCK);
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
    :arg:`address`.:arg:`timeout` determines how much time to wait for
    connection to be established. The default value for :arg:`timeout` is
    indefinite.

    :arg address: address to connect to
    :type address: :type:`ipAddr`
    :arg timeout: time to wait for connection establishment.
    :type timeval: :type:`~Sys.timeval`
    :return: connected socket.
    :rtype: `tcpConn`
    :throws SystemError: Upon failure to connect.
  */
  proc connect(in address:ipAddr, in timeout = new timeval(-1,0)) throws {
    var family = address.family;
    var socketFd = socket(family, SOCK_STREAM | SOCK_NONBLOCK);
    var err_out = sys_connect(socketFd, address._addressStorage);
    if(err_out != 0 && err_out != EINPROGRESS) {
      sys_close(socketFd);
      throw SystemError.fromSyserr(err_out,"Connect not Possible");
    }
    if(err_out == 0) {
      var sockFile:tcpConn = openfd(socketFd);
      return sockFile;
    }
    var rset, wset: fd_set;
    sys_fd_zero(wset);
    sys_fd_set(socketFd, wset);
    rset = wset;
    var nready:int(32);
    if timeout.tv_sec == -1 {
      err_out = sys_select(socketFd + 1, c_ptrTo(rset), c_ptrTo(wset), nil, nil, nready);
    }
    else {
      err_out = sys_select(socketFd + 1, c_ptrTo(rset), c_ptrTo(wset), nil, c_ptrTo(timeout), nready);
    }
    if nready == 0 {
      sys_close(socketFd);
      throw SystemError.fromSyserr(ETIMEDOUT, "Connect timed out");
    }
    if err_out != 0 {
      sys_close(socketFd);
      throw SystemError.fromSyserr(err_out, "Connect failed while I/O waiting");
    }
    if(sys_fd_isset(socketFd, rset) != 0 || sys_fd_isset(socketFd, wset) != 0){
      var tempAddress = new sys_sockaddr_t();
      err_out = sys_getpeername(socketFd, address._addressStorage);
      if(err_out != 0) {
        var berkleyError:err_t;
        var ptrberkleyError = c_ptrTo(berkleyError);
        var voidPtrberkleyError:c_void_ptr = ptrberkleyError;
        var berkleySize:socklen_t = sizeof(berkleyError):socklen_t;
        err_out = sys_getsockopt(socketFd, SOL_SOCKET, SO_ERROR, voidPtrberkleyError, berkleySize);
        defer sys_close(socketFd);
        if(err_out != 0){
          throw SystemError.fromSyserr(err_out, "Connect Unsuccessful");
        }
        else if(berkleyError != 0){
          throw SystemError.fromSyserr(berkleyError, "Berkley Error");
        }
      }
    }
    else {
      sys_close(socketFd);
      throw new Error("connect() failed");
    }
    var sockFile:tcpConn = openfd(socketFd);
    return sockFile;
  }

  /*
    This overload of `connect` not only returns a :type:`tcpConn`
    but also does DNS resolution for the provided :arg:`host`.
    The :arg:`timeout` is tired for all resolved addresses and the first
    successful one is returned back.

    :arg host: host to connect to or resolve if not in standard ip notation
    :type host: `string`
    :arg service: service to connect to on resolved :arg:`host`
    :type service: `string`
    :arg family: type of socket family to connect to
    :type family: :type:`IPFamily`
    :arg timeout: time to wait for each possible connection.
    :type timeval: :type:`~Sys.timeval`
    :return: connected socket.
    :rtype: `tcpConn`
    :throws SystemError: Upon failure to resolve address or connect
                         to any of the resolved address in given :arg:`timeout`.
  */
  proc connect(in host:string, in service:string, family:IPFamily = IPFamily.IPUnspec, in timeout = new timeval(-1,0)):tcpConn throws {
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

  /*
    This overload of `connect` not only returns a :type:`tcpConn`
    but also does DNS resolution for the provided :arg:`host`.
    The :arg:`timeout` is tired for all resolved addresses and the first
    successful one is returned back.

    :arg host: address of host to connect or resolve if not in ip notation
    :type host: `string`
    :arg port: port to connect to on :arg:`host`
    :type port: `uint(16)`
    :arg family: type of socket family to connect to
    :type family: :type:`IPFamily`
    :arg timeout: time to wait for each possible connection.
    :type timeval: :type:`~Sys.timeval`
    :return: connected socket.
    :rtype: `tcpConn`
    :throws SystemError: Upon failure to resolve address or connect
                      to any of the resolved address in given :arg:`timeout`.
  */
  proc connect(in host:string, in port:uint(16), family:IPFamily = IPFamily.IPUnspec, in timeout = new timeval(-1,0)) throws {
    return connect(host, port:string, family, timeout);
  }

  /*
    A record holding reference to a udp socket
    bound to any available port.
  */
  record udpSocket {
    var socketFd:int(32);
    // Create a UDP socket of provided Family.
    proc init(family:IPFamily = IPFamily.IPv4) {
      this.socketFd = -1;
      try! {
        var sockFd = socket(family, SOCK_DGRAM | SOCK_NONBLOCK);
        this.socketFd = sockFd;
      }
    }
  }

  // Get :type:`ipAddr` associated with udp socket
  proc udpSocket.addr {
    return try! getsockname(this.socketFd);
  }

  pragma "no doc"
  extern proc sys_recvfrom(sockfd:fd_t, buff:c_void_ptr, len:size_t, flags:c_int, ref src_addr_out:sys_sockaddr_t, ref num_recvd_out:ssize_t):err_t;

  /*
    Reads incoming :arg:`bufferLen` number of bytes on socket, and
    return a tuple of (data, address), where address will be a
    :type:`ipAddr` pointing to address of the socket from where data was received.

    :arg bufferLen: number of bytes to read
    :type bufferLen: `int`
    :arg timeout: time to wait for data to arrive.
    :type timeval: :type:`~Sys.timeval`
    :return: tuple of (data, address)
    :rtype: :type:`tuple(bytes, ipAddr)`
    :throws SystemError: Upon failure to receive any data
                      within given :arg:`timeout`.
  */
  proc udpSocket.recvfrom(bufferLen:int, in timeout = new timeval(-1,0), flags:c_int = 0) throws {
    var rset: fd_set;
    sys_fd_zero(rset);
    sys_fd_set(this.socketFd, rset);
    var nready:c_int;
    var err_out:err_t = 0;
    if timeout.tv_sec == -1 {
      err_out = sys_select(socketFd + 1, c_ptrTo(rset), nil, nil, nil, nready);
    }
    else {
      err_out = sys_select(socketFd + 1, c_ptrTo(rset), nil, nil, c_ptrTo(timeout), nready);
    }
    if nready == 0 {
      throw SystemError.fromSyserr(ETIMEDOUT, "recv timed out");
    }
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "recv failed");
    }
    var buffer = c_calloc(c_uchar, bufferLen);
    var length:ssize_t;
    var addressStorage = new sys_sockaddr_t();
    err_out = sys_recvfrom(this.socketFd, buffer, bufferLen:size_t, 0, addressStorage, length);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "recv failed");
    }
    return (createBytesWithOwnedBuffer(buffer, length, bufferLen), new ipAddr(addressStorage));
  }

  /*
    Reads incoming :arg:`bufferLen` number of bytes on socket, and
    return a tuple of read bytes, which can have size smaller than asked and if
    the size is more they will be truncated.

    :arg bufferLen: number of bytes to read
    :type bufferLen: `int`
    :arg timeout: time to wait for data to arrive.
    :type timeval: :type:`~Sys.timeval`
    :return: data
    :rtype: `bytes`
    :throws SystemError: Upon failure to receive any data
                         within given :arg:`timeout`.
  */
  proc udpSocket.recv(bufferLen: int, in timeout = new timeval(-1,0)) throws {
    var (data, _) = this.recvfrom(bufferLen, timeout);
    return data;
  }

  pragma "no doc"
  extern proc sys_sendto(sockfd:fd_t, buff:c_void_ptr, len:c_long, flags:c_int, ref address:sys_sockaddr_t,  ref num_sent_out:ssize_t):err_t;

  /*
    Send :arg:`data` over socket to the provided address and
    return number of bytes sent if successful.

    :arg data: data to send to address
    :type data: `bytes`
    :arg address: socket address for sending data
    :type address: :type:`ipAddr`
    :arg timeout: time to wait for data to arrive.
    :type timeval: :type:`~Sys.timeval`
    :return: data
    :rtype: `bytes`
    :throws SystemError: Upon failure to send any data
                         within given :arg:`timeout`.
  */
  proc udpSocket.send(data: bytes, in address: ipAddr, in timeout = new timeval(-1,0)) throws {
    var wset: fd_set;
    sys_fd_zero(wset);
    sys_fd_set(this.socketFd, wset);
    var nready:c_int;
    var err_out:err_t = 0;
    if timeout.tv_sec == -1 {
      err_out = sys_select(socketFd + 1, nil, c_ptrTo(wset), nil, nil, nready);
    }
    else {
      err_out = sys_select(socketFd + 1, nil, c_ptrTo(wset), nil, c_ptrTo(timeout), nready);
    }
    if nready == 0 {
      throw SystemError.fromSyserr(ETIMEDOUT, "send timed out");
    }
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "send failed I/O");
    }
    var length:ssize_t;
    err_out = sys_sendto(this.socketFd, data.c_str():c_void_ptr, data.size:c_long, 0, addr._addressStorage, length);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "send failed");
    }
    return length;
  }

  inline operator !=(in lhs: udpSocket,in rhs: udpSocket) {
    return lhs.socketFd != rhs.socketFd;
  }

  proc udpSocket.writeThis(f) throws {
    f.write("(","addr:",this.addr,",fd:",this.socketFd);
  }

  pragma "no doc"
  proc setSockOpt(socketFd:fd_t, level:c_int, optname:c_int, value:c_int) throws {
    var optlen = sizeof(value);
    var ptroptval = c_ptrTo(value);
    var err_out = sys_setsockopt(socketFd, level, optname, ptroptval:c_void_ptr, optlen);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed to set socket option");
    }
  }

// The :arg:`value`
//     can be an integer, None or a bytes-like object representing a buffer. In the later case it is up to the caller to ensure that the bytestring contains the proper bits (see the optional built-in module struct for a way to encode C structures as bytestrings). When value is set to None, optlen argument is required. It’s equivalent to call setsockopt() C function with optval=NULL and optlen=optlen

  /*
    Set the value of the given socket option (see `setsockopt(2) </https://pubs.opengroup.org/onlinepubs/9699919799/functions/setsockopt.html#>`_)
    on provided :type:`tcpConn`. The needed symbolic constants (SO_* etc.)
    are defined in the :mod:`Sys`.

    :arg socket: socket to set option on
    :type socket: `tcpConn`
    :arg level: protocol level
    :type level: `int(32)`
    :arg optname: option to set.
    :type optname: `int(32)`
    :arg value: value to set on option
    :type value: `int(32)`
    :throws SystemError: Upon incompatible arguments
                         and socket.
  */
  proc setSockOpt(ref socket:tcpConn, level:c_int, optname:c_int, value:c_int) throws {
    var socketFd = socket.socketFd;
    setSockOpt(socketFd, level, optname, value);
  }

  // See above
  proc setSockOpt(ref socket:tcpListener, level:c_int, optname:c_int, value:c_int) throws {
    var socketFd = socket.socketFd;
    setSockOpt(socketFd, level, optname, value);
  }

  // See above
  proc setSockOpt(ref socket:udpSocket, level:c_int, optname:c_int, value:c_int) throws {
    var socketFd = socket.socketFd;
    setSockOpt(socketFd, level, optname, value);
  }

  pragma "no doc"
  proc setSockOpt(socketFd:fd_t, level:c_int, optname:c_int, value:bytes) throws {
    var optlen = value.size;
    var ptroptval = value.c_str();
    var err_out = sys_setsockopt(socketFd, level, optname, ptroptval, optlen);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed to set socket option");
    }
  }

  /*
    Overload for :proc:`setSockOpt` that allows setting a `bytes` value
    on socket option. It is up to the caller to ensure that the
    :arg:`value` which is a :type:`bytes` parameter contains the proper bits.

    :arg socket: socket to set option on
    :type socket: `tcpConn`
    :arg level: protocol level
    :type level: `int(32)`
    :arg optname: option to set.
    :type optname: `int(32)`
    :arg value: value to set on option
    :type value: `bytes`
    :throws SystemError: Upon incompatible arguments
                         and socket.
  */
  proc setSockOpt(ref socket:tcpConn, level:c_int, optname:c_int, value:bytes) throws {
    var socketFd = socket.socketFd;
    setSockOpt(socketFd, level, optname, value);
  }

  // See above
  proc setSockOpt(ref socket:tcpListener, level:c_int, optname:c_int, value:bytes) throws {
    var socketFd = socket.socketFd;
    setSockOpt(socketFd, level, optname, value);
  }

  // See above
  proc setSockOpt(ref socket:udpSocket, level:c_int, optname:c_int, value:bytes) throws {
    var socketFd = socket.socketFd;
    setSockOpt(socketFd, level, optname, value);
  }

  pragma "no doc"
  proc setSockOpt(socketFd:fd_t, level:c_int, optname:c_int, value:nothing, optlen:socklen_t) throws {
    var err_out = sys_setsockopt(socketFd, level, optname, nil, optlen);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed to set socket option");
    }
  }

  /*
    Overload for :proc:`setSockOpt` which is equivalent to calling setsockopt() C
    function with `optval=NULL` and `optlen=optlen`.

    :arg socket: socket to set option on
    :type socket: `tcpConn`
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
  proc setSockOpt(ref socket:tcpConn, level:c_int, optname:c_int, value:nothing, optlen:socklen_t) throws {
    var socketFd = socket.socketFd;
    setSockOpt(socketFd, level, optname, value, optlen);
  }

  // See above
  proc setSockOpt(ref socket:tcpListener, level:c_int, optname:c_int, value:nothing, optlen:socklen_t) throws {
    var socketFd = socket.socketFd;
    setSockOpt(socketFd, level, optname, value, optlen);
  }

  // See above
  proc setSockOpt(ref socket:udpSocket, level:c_int, optname:c_int, value:nothing, optlen:socklen_t) throws {
    var socketFd = socket.socketFd;
    setSockOpt(socketFd, level, optname, value, optlen);
  }

  pragma "no doc"
  proc getSocketOpt(socketFd:fd_t, level:c_int, optname:c_int) throws {
    var optval:c_int;
    var ptroptval = c_ptrTo(optval);
    var optlen = sizeof(optval);
    var err_out = sys_getsockopt(socketFd, level, ptroptval:c_void_ptr, optlen);
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
    :type socket: `tcpConn`
    :arg level: protocol level
    :type level: `int(32)`
    :arg optname: option to set.
    :type optname: `int(32)`
    :return: value of socket option
    :rtype: `int(32)`
    :throws SystemError: Upon incompatible arguments
                         and socket.
  */
  proc getSocketOpt(ref socket:tcpConn, level:c_int, optname:c_int) throws {
    var socketFd = socket.socketFd;
    return getSocketOpt(socketFd, level, optname) ;
  }

  // See above
  proc getSocketOpt(ref socket:tcpListener, level:c_int, optname:c_int) throws {
    var socketFd = socket.socketFd;
    return getSocketOpt(socketFd, level, optname) ;
  }

  // See above
  proc getSocketOpt(ref socket:udpSocket, level:c_int, optname:c_int) throws {
    var socketFd = socket.socketFd;
    return getSocketOpt(socketFd, level, optname) ;
  }

  pragma "no doc"
  proc getSocketOpt(socketFd:fd_t, level:c_int, optname:c_int, buflen = 0) throws {
    if buflen < 0 || buflen > 1024 {
      throw new Error("getSocketOpt buflen out of range");
    }
    else {
      var len:socklen_t = buflen;
      var buffer = c_calloc(c_uchar, buflen);
      var err_out = sys_getsockopt(socketFd, level, buffer:c_void_ptr, len:socklen_t);
      if err_out != 0 {
        throw SystemError.fromSyserr(err_out, "Failed to get socket option");
      }
      return createBytesWithOwnedBuffer(buffer, len, buflen);
    }
  }

  /*
    Returns the value of the given socket option which is expected to be of type
    `bytes` on provided :type:`tcpConn`. The needed symbolic constants (SO_* etc.)
    are defined in :mod:`Sys` module.

    :arg socket: socket to set option on
    :type socket: `tcpConn`
    :arg level: protocol level
    :type level: `int(32)`
    :arg optname: option to set.
    :type optname: `int(32)`
    :return: value of socket option
    :rtype: `bytes`
    :throws SystemError: Upon incompatible arguments
                         and socket.
  */
  proc getSocketOpt(ref socket:tcpConn, level:c_int, optname:c_int, buflen = 0) throws {
    var socketFd = socket.socketFd;
    return getSocketOpt(socketFd, level, optname, buflen) ;
  }

  // See Above
  proc getSocketOpt(ref socket:tcpListener, level:c_int, optname:c_int, buflen = 0) throws {
    var socketFd = socket.socketFd;
    return getSocketOpt(socketFd, level, optname, buflen) ;
  }

  // See Above
  proc getSocketOpt(ref socket:udpSocket, level:c_int, optname:c_int, buflen = 0) throws {
    var socketFd = socket.socketFd;
    return getSocketOpt(socketFd, level, optname, buflen) ;
  }

  pragma "no doc"
  proc getpeername(socketFD: fd_t) throws {
    var addressStorage = new sys_sockaddr_t();
    var err = sys_getpeername(socketFD, addressStorage);
    if err != 0 {
      throw SystemError.fromSyserr(err, "Failed to get remote address");
    }
    return new ipAddr(addressStorage);
  }

  /*
    Returns the remote addressto which socket is connected. This is
    useful to find out the port number of a remote IPv4/v6 socket, for instance.

    :arg socket: socket to set option on
    :type socket: `tcpConn`

    :return: remote address
    :rtype: `ipAddr`
    :throws SystemError: If socket is not connected
  */
  proc getpeername(ref socket: tcpConn) throws {
    var socketFd = socket.socketFd;
    return getpeername(socketFd);
  }

  pragma "no doc"
  proc getsockname(socketFD: fd_t) throws {
    var addressStorage = new sys_sockaddr_t();
    var err = sys_getsockname(socketFD, addressStorage);
    if err != 0 {
      throw SystemError.fromSyserr(err, "Failed to get local address");
    }
    return new ipAddr(addressStorage);
  }

  /*
    Returns the socket's own address. This is useful to find out the port
    number of a remote IPv4/v6 socket, for instance.

    :arg socket: socket to set option on
    :type socket: `tcpConn`

    :return: remote address
    :rtype: `ipAddr`
    :throws SystemError: If socket is closed.
  */
  proc getsockname(ref socket: tcpConn) throws {
    var socketFd = socket.socketFd;
    return getsockname(socketFd);
  }

  // See Above
  proc getsockname(ref socket: tcpListener) throws {
    var socketFd = socket.socketFd;
    return getsockname(socketFd);
  }

  // See Above
  proc getsockname(ref socket: udpSocket) throws {
    var socketFd = socket.socketFd;
    return getsockname(socketFd);
  }

  pragma "no doc"
  proc socket(family:IPFamily = IPFamily.IPv4, sockType:c_int = SOCK_STREAM, protocol = 0) throws {
    var socketFd: c_int;
    var err = sys_socket(family:c_int, sockType | SOCK_CLOEXEC, protocol:c_int, socketFd);
    if err != 0 {
      throw SystemError.fromSyserr(err, "Failed to create socket");
    }
    return socketFd;
  }

  pragma "no doc"
  proc bind(socketFd:fd_t, ref address: ipAddr, reuseAddr = true) throws {
    var enable = if reuseAddr then 1 else 0;
    if reuseAddr {
      var ptrEnable = c_ptrTo(enable);
      var err_out = sys_setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, ptrEnable:c_void_ptr, sizeof(enable):c_int);
      if err_out != 0 {
        throw SystemError.fromSyserr(err_out, "Failed to reuse address");
      }
    }
    var err = sys_bind(socketFd, address._addressStorage);
    if err != 0 {
      throw SystemError.fromSyserr(err, "Failed to bind Socket");
    }
  }


  /*
    Bind the socket to address. The socket must not already be bound.

    :arg socket: socket to set option on
    :type socket: `tcpConn`
    :arg address: address to bind to
    :type address: `ipAddr`
    :arg reuseAddr: whether to reuse address if already in use
    :type address: `boolean`

    :throws SystemError: If socket is closed or already bound.
  */
  proc bind(ref socket:udpSocket, ref address: ipAddr, reuseAddr = true) throws {
    var socketFd = socket.socketFd;
    bind(socketFd, address, reuseAddr);
  }

  // See Above
  proc bind(ref socket:tcpListener, ref address: ipAddr, reuseAddr = true) throws {
    var socketFd = socket.socketFd;
    bind(socketFd, address, reuseAddr);
  }

  // See Above
  proc bind(ref socket:tcpConn, ref address: ipAddr, reuseAddr = true) throws {
    var socketFd = socket.socketFd;
    bind(socketFd, address, reuseAddr);
  }

  pragma "no doc"
  proc naggle(socketFd:fd_t, enable:bool = true) throws {
    var c_enable = if enable then 0 else 1;
    var ptrEnable = c_ptrTo(c_enable);
    var err_out = sys_setsockopt(socketFd, IPPROTO_TCP, TCP_NODELAY, ptrEnable:c_void_ptr, sizeof(c_enable):c_int);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed naggle");
    }
  }

  // Enables or disables Nagle's algorithm on a given TCP Listener.
  proc naggle(ref socket:tcpListener, enable:bool = true) throws {
    var socketFd = socket.socketFd;
    naggle(socketFd, enable);
  }

  // Enables or disables Nagle's algorithm on a given TCP Connection.
  proc naggle(ref socket:tcpConn, enable:bool = true) throws {
    var socketFd = socket.socketFd;
    naggle(socketFd, enable);
  }

  pragma "no doc"
  proc delayAck(socketFd:fd_t, enable:bool = true) throws {
    var c_enable = if enable then 0 else 1;
    var ptrEnable = c_ptrTo(c_enable);
    var err_out = sys_setsockopt(socketFd, IPPROTO_TCP, TCP_QUICKACK, ptrEnable:c_void_ptr, sizeof(c_enable):c_int);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed delayAck");
    }
  }

  // Enables or disables Delayed Ack optimization on a given TCP Listener.
  proc delayAck(ref socket:tcpListener, enable:bool = true) throws {
    var socketFd = socket.socketFd;
    delayAck(socketFd, enable);
  }

  // Enables or disables Delayed Ack optimization on a given TCP Connection.
  proc delayAck(ref socket:tcpConn, enable:bool = true) throws {
    var socketFd = socket.socketFd;
    delayAck(socketFd, enable);
  }
}
