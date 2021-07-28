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
module Socket {

  public use Sys;
  public use SysError;
  use Time;
  use SysCTypes;
  use SysBasic;
  use CPtr;
  use IO;

  enum IPFamily {
    IPv4 = 2,
    IPv6 = 10,
    IPUnspec = 0
  }

  const IPv4Localhost = INADDR_LOOPBACK;
  const IPv4Any = INADDR_ANY;
  const IPv4Broadcast = INADDR_BROADCAST;
  const IPv6Localhost = in6addr_loopback;
  const IPv6Any = in6addr_any;


  record ipAddr {
    var _addressStorage:sys_sockaddr_t;
  }

  proc ipAddr.init(inout host:string = "localhost", port:uint(16) = 8000, family:IPFamily = IPFamily.IPv4) {
    if(host == "localhost"){
      host = if family == IPFamily.IPv4 then "127.0.0.1" else "::1";
    }

    _addressStorage = new sys_sockaddr_t();

    try! {
      _addressStorage.set(host.c_str(), port, family:c_int);
    }
  }

  proc ipAddr.init(host:sys_in_addr_t, port:uint(16) = 8000) {
    _addressStorage = new sys_sockaddr_t();

    try! {
      _addressStorage.set(host, port);
    }
  }

  proc ipAddr.init(host:sys_in6_addr_t, port:uint(16) = 8000) {
    _addressStorage = new sys_sockaddr_t();

    try! {
      _addressStorage.set(host, port);
    }
  }

  pragma "no doc"
  proc ipAddr.init(in address:sys_sockaddr_t) {
    this._addressStorage = new sys_sockaddr_t();
    try! {
      _addressStorage.set(address.numericHost().c_str(), address.port(), address.family);
    }
  }

  proc ipAddr.family {
    return _addressStorage.family;
  }

  proc ipAddr.host {
    return try! _addressStorage.numericHost();
  }

  proc ipAddr.port {
    return try! _addressStorage.port();
  }

  inline operator !=(in lhs: ipAddr,in rhs: ipAddr) {
    return lhs.family != rhs.family || lhs.host != rhs.host || lhs.port != rhs.port;
  }

  proc ipAddr.writeThis(f) throws {
    f.write("(","family:",this.family,",host:",this.host,",port:",this.port,")");
  }

  private extern proc qio_get_fd(fl:qio_file_ptr_t, ref fd:c_int):syserr;

  type tcpConn = file;

  proc tcpConn.socketFd throws {
    var tempfd:c_int;
    var err:syserr = ENOERR;
    on this.home {
      err = qio_get_fd(this._file_internal, tempfd);
    }
    if err then try ioerror(err, "in tcpConn.socketFd");
    return tempfd;
  }

  proc tcpConn.addr {
    return try! remoteAddress(this.socketFd);
  }

  inline operator !=(in lhs: tcpConn,in rhs: tcpConn) {
    return lhs.socketFd != rhs.socketFd;
  }

  proc tcpConn.writeThis(f) throws {
    f.write("(","addr:",this.addr,",fd:",this.socketFd);
  }

  private extern proc sizeof(e): size_t;

  record tcpListener {
    var socketFd:int(32);

    pragma "no doc"
    proc init(socketFd:c_int) {
      this.socketFd = socketFd;
    }
  }

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

  proc tcpListener.close() {
    sys_close(this.socketFd);
  }

  proc tcpListener.addr {
    return try! localAddress(this.socketFd);
  }

  inline operator !=(in lhs: tcpListener,in rhs: tcpListener) {
    return lhs.socketFd != rhs.socketFd;
  }

  proc tcpListener.writeThis(f) throws {
    f.write("(","addr:",this.addr,",fd:",this.socketFd);
  }

  proc listen(in address:ipAddr, reuseAddr = true, backlog = 5) throws {
    var family = if address.family == AF_INET6 then IPFamily.IPv6 else IPFamily.IPv4;
    var socketFd = socket(family, SOCK_STREAM | SOCK_NONBLOCK);

    bind(socketFd, address, reuseAddr);

    var err_out = sys_listen(socketFd, backlog:c_int);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed to listen on socket");
    }

    const listener = new tcpListener(socketFd);
    return listener;
  }

  proc connect(in address:ipAddr, in timeout = new timeval(-1,0)) throws {
    var family = if address.family == AF_INET6 then IPFamily.IPv6 else IPFamily.IPv4;
    var socketFd = socket(family, SOCK_STREAM | SOCK_NONBLOCK);

    var err_out = sys_connect(socketFd, address._addressStorage);
    if(err_out != 0 && err_out != EINPROGRESS) {
      sys_close(socketFd);
      throw SystemError.fromSyserr(err_out,"Failed to connect");
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
      throw SystemError.fromSyserr(ETIMEDOUT, "connect timed out");
    }

    if err_out != 0 {
      sys_close(socketFd);
      throw SystemError.fromSyserr(err_out, "connect() failed");
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
          throw SystemError.fromSyserr(err_out, "Failed to connect");
        }
        else if(berkleyError != 0){
          throw SystemError.fromSyserr(berkleyError, "Failed to connect");
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

  proc connect(in host:string, in port:uint(16), family:IPFamily = IPFamily.IPUnspec, in timeout = new timeval(-1,0)) throws {
    return connect(host, port:string, family, timeout);
  }

  record udpSocket {
    var socketFd:int(32);

    proc init(family:IPFamily = IPFamily.IPv4) {
      this.socketFd = -1;
      try! {
        var sockFd = socket(family, SOCK_DGRAM | SOCK_NONBLOCK);
        this.socketFd = sockFd;
      }
    }
  }

  proc udpSocket.addr {
    return try! localAddress(this.socketFd);
  }

  extern proc sys_recvfrom(sockfd:fd_t, buffer:c_void_ptr, len:size_t, flags:c_int, ref address:sys_sockaddr_t, ref recvd_out:ssize_t):err_t;

  proc udpSocket.recvfrom(buffer_len:int, in timeout = new timeval(-1,0), flags:c_int = 0) throws {
    var rset: fd_set;
    sys_fd_zero(rset);
    sys_fd_set(this.socketFd, rset);
    var nready:c_int;
    var err_out:err_t;

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

    var buffer = c_calloc(c_uchar, buffer_len);
    var length:ssize_t;
    var addressStorage = new sys_sockaddr_t();
    err_out = sys_recvfrom(this.socketFd, buffer, buffer_len:size_t, 0, addressStorage, length);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "recv failed");
    }

    return (createBytesWithOwnedBuffer(buffer, length, buffer_len), new ipAddr(addressStorage));
  }

  proc udpSocket.recv(buffer_len: int, in timeout = new timeval(-1,0)) throws {
    var (data, _) = this.recvfrom(buffer_len, timeout);
    return data;
  }

  extern proc sys_sendto(sockfd:fd_t, buffer:c_void_ptr, len:c_long, flags:c_int, ref address:sys_sockaddr_t,  ref recvd_out:ssize_t):err_t;

  proc udpSocket.send(data: bytes, in addr: ipAddr, in timeout = new timeval(-1,0)) throws {
    var wset: fd_set;
    sys_fd_zero(wset);
    sys_fd_set(this.socketFd, wset);
    var nready:c_int;
    var err_out:err_t;

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
      throw SystemError.fromSyserr(err_out, "send failed");
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

  proc setSocketOpt(socketFd:fd_t, level:c_int, optname:c_int, value:c_int) throws {
    var optlen = sizeof(value);
    var ptroptval = c_ptrTo(value);

    var err_out = sys_setsockopt(socketFd, level, optname, ptroptval:c_void_ptr, optlen);

    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed to set socket option");
    }
  }

  proc setSocketOpt(ref socket:tcpConn, level:c_int, optname:c_int, value:c_int) throws {
    var socketFd = socket.socketFd;

    setSocketOpt(socketFd, level, optname, value);
  }

  proc setSocketOpt(ref socket:tcpListener, level:c_int, optname:c_int, value:c_int) throws {
    var socketFd = socket.socketFd;

    setSocketOpt(socketFd, level, optname, value);
  }

  proc setSocketOpt(ref socket:udpSocket, level:c_int, optname:c_int, value:c_int) throws {
    var socketFd = socket.socketFd;

    setSocketOpt(socketFd, level, optname, value);
  }

  proc setSocketOpt(socketFd:fd_t, level:c_int, optname:c_int, value:bytes) throws {
    var optlen = value.size;
    var ptroptval = value.c_str();

    var err_out = sys_setsockopt(socketFd, level, optname, ptroptval, optlen);

    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed to set socket option");
    }
  }

  proc setSocketOpt(ref socket:tcpConn, level:c_int, optname:c_int, value:bytes) throws {
    var socketFd = socket.socketFd;

    setSocketOpt(socketFd, level, optname, value);
  }

  proc setSocketOpt(ref socket:tcpListener, level:c_int, optname:c_int, value:bytes) throws {
    var socketFd = socket.socketFd;

    setSocketOpt(socketFd, level, optname, value);
  }

  proc setSocketOpt(ref socket:udpSocket, level:c_int, optname:c_int, value:bytes) throws {
    var socketFd = socket.socketFd;

    setSocketOpt(socketFd, level, optname, value);
  }

  proc setSocketOpt(socketFd:fd_t, level:c_int, optname:c_int, value:nothing, optlen:socklen_t) throws {
    var err_out = sys_setsockopt(socketFd, level, optname, nil, optlen);

    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed to set socket option");
    }
  }

  proc setSocketOpt(ref socket:tcpConn, level:c_int, optname:c_int, value:nothing, optlen:socklen_t) throws {
    var socketFd = socket.socketFd;

    setSocketOpt(socketFd, level, optname, value, optlen);
  }

  proc setSocketOpt(ref socket:tcpListener, level:c_int, optname:c_int, value:nothing, optlen:socklen_t) throws {
    var socketFd = socket.socketFd;

    setSocketOpt(socketFd, level, optname, value, optlen);
  }

  proc setSocketOpt(ref socket:udpSocket, level:c_int, optname:c_int, value:nothing, optlen:socklen_t) throws {
    var socketFd = socket.socketFd;

    setSocketOpt(socketFd, level, optname, value, optlen);
  }

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

  proc getSocketOpt(ref socket:tcpConn, level:c_int, optname:c_int) throws {
    var socketFd = socket.socketFd;

    return getSocketOpt(socketFd, level, optname) ;
  }

  proc getSocketOpt(ref socket:tcpListener, level:c_int, optname:c_int) throws {
    var socketFd = socket.socketFd;

    return getSocketOpt(socketFd, level, optname) ;
  }

  proc getSocketOpt(ref socket:udpSocket, level:c_int, optname:c_int) throws {
    var socketFd = socket.socketFd;

    return getSocketOpt(socketFd, level, optname) ;
  }

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

  proc getSocketOpt(ref socket:tcpConn, level:c_int, optname:c_int, buflen = 0) throws {
    var socketFd = socket.socketFd;

    return getSocketOpt(socketFd, level, optname, buflen) ;
  }

  proc getSocketOpt(ref socket:tcpListener, level:c_int, optname:c_int, buflen = 0) throws {
    var socketFd = socket.socketFd;

    return getSocketOpt(socketFd, level, optname, buflen) ;
  }

  proc getSocketOpt(ref socket:udpSocket, level:c_int, optname:c_int, buflen = 0) throws {
    var socketFd = socket.socketFd;

    return getSocketOpt(socketFd, level, optname, buflen) ;
  }

  proc remoteAddress(socketFD: fd_t) throws {
    var addressStorage = new sys_sockaddr_t();
    var err = sys_getpeername(socketFD, addressStorage);
    if err != 0 {
      throw SystemError.fromSyserr(err, "Failed to get address");
    }

    return new ipAddr(addressStorage);
  }

  proc remoteAddress(ref socket: tcpConn) throws {
    var socketFd = socket.socketFd;

    return remoteAddress(socketFd);
  }

  proc localAddress(socketFD: fd_t) throws {
    var addressStorage = new sys_sockaddr_t();
    var err = sys_getsockname(socketFD, addressStorage);
    if err != 0 {
      throw SystemError.fromSyserr(err, "Failed to get address");
    }

    return new ipAddr(addressStorage);
  }

  proc localAddress(ref socket: tcpConn) throws {
    var socketFd = socket.socketFd;

    return localAddress(socketFd);
  }

  proc localAddress(ref socket: tcpListener) throws {
    var socketFd = socket.socketFd;

    return localAddress(socketFd);
  }

  proc localAddress(ref socket: udpSocket) throws {
    var socketFd = socket.socketFd;

    return localAddress(socketFd);
  }

  proc socket(family:IPFamily = IPFamily.IPv4, sockType:c_int = SOCK_STREAM, protocol = 0) throws {
    var socketFd: c_int;
    var err = sys_socket(family:c_int, sockType | SOCK_CLOEXEC, protocol:c_int, socketFd);
    if err != 0 {
      throw SystemError.fromSyserr(err, "Failed to create socket");
    }

    return socketFd;
  }

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

  proc bind(ref socket:udpSocket, ref address: ipAddr, reuseAddr = true) throws {
    var socketFd = socket.socketFd;

    bind(socketFd, address, reuseAddr);
  }

  proc bind(ref socket:tcpListener, ref address: ipAddr, reuseAddr = true) throws {
    var socketFd = socket.socketFd;

    bind(socketFd, address, reuseAddr);
  }

  proc bind(ref socket:tcpConn, ref address: ipAddr, reuseAddr = true) throws {
    var socketFd = socket.socketFd;

    bind(socketFd, address, reuseAddr);
  }

  proc naggle(socketFd:fd_t, enable:bool = true) throws {
    var c_enable = if enable then 0 else 1;
    var ptrEnable = c_ptrTo(c_enable);
    var err_out = sys_setsockopt(socketFd, IPPROTO_TCP, TCP_NODELAY, ptrEnable:c_void_ptr, sizeof(c_enable):c_int);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed naggle");
    }
  }

  proc naggle(ref socket:tcpListener, enable:bool = true) throws {
    var socketFd = socket.socketFd;

    naggle(socketFd, enable);
  }

  proc naggle(ref socket:tcpConn, enable:bool = true) throws {
    var socketFd = socket.socketFd;

    naggle(socketFd, enable);
  }

  proc delayAck(socketFd:fd_t, enable:bool = true) throws {
    var c_enable = if enable then 0 else 1;
    var ptrEnable = c_ptrTo(c_enable);
    var err_out = sys_setsockopt(socketFd, IPPROTO_TCP, TCP_QUICKACK, ptrEnable:c_void_ptr, sizeof(c_enable):c_int);
    if err_out != 0 {
      throw SystemError.fromSyserr(err_out, "Failed delayAck");
    }
  }

  proc delayAck(ref socket:tcpListener, enable:bool = true) throws {
    var socketFd = socket.socketFd;

    delayAck(socketFd, enable);
  }

  proc delayAck(ref socket:tcpConn, enable:bool = true) throws {
    var socketFd = socket.socketFd;

    delayAck(socketFd, enable);
  }
}
