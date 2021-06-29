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
  use Time;
  use SysCTypes;
  use SysError;
  use CPtr;
  use IO;

  enum IPFamily {
    IPv4 = 2,
    IPv6 = 10
  }

  const IPv4Localhost = INADDR_LOOPBACK;
  const IPv6Localhost = in6addr_loopback;

  record ipAddr {
    var _addressStorage:sys_sockaddr_t;
    var port:uint(16);
    var host:string;
  }

  proc ipAddr.init(inout host:string = "localhost", port:uint(16) = 8000, family:IPFamily = IPFamily.IPv4) {
    if(host == "localhost"){
      host = if family == IPFamily.IPv4 then "127.0.0.1" else "::1";
    }

    _addressStorage = new sys_sockaddr_t();

    try! {
      _addressStorage.initialize(host.c_str(), port, family:int);
    }
  }

  proc ipAddr.init(host:sys_in_addr_t, port: uint(16) = 8000) {
    this.host = "";
    this.port = port;
    _addressStorage = new sys_sockaddr_t();

    try! {
      _addressStorage.initialize(host,port);
    }
  }

  proc ipAddr.init(host:sys_in6_addr_t, port: uint(16) = 8000) {
    this.host = "";
    this.port = port;
    _addressStorage = new sys_sockaddr_t();

    try! {
      _addressStorage.initialize(host,port);
    }
  }

  proc ipAddr.init(ref address:sys_sockaddr_t) {
    var familySize = if address.family == IPFamily.IPv4 then INET_ADDRSTRLEN else INET6_ADDRSTRLEN;
    var buffer = c_calloc(c_char,familySize);

    var getport:uint(16);
    try! address.addr(buffer, getport);
    this.host = createStringWithOwnedBuffer(buffer,familySize,familySize);
    this.port = getport;
    this._addressStorage = new sys_sockaddr_t();
    try! {
      _addressStorage.initialize(this.host,this.port,client_addr.family);
    }
  }

  proc ipAddr.family {
    return _addressStorage.family;
  }

  private extern proc qio_get_fd(fl:qio_file_ptr_t, ref fd:c_int):syserr;

  type tcpConn = file;

  proc tcpConn.fd():int throws {
    var tempfd:c_int;
    var err:syserr = ENOERR;
    on this.home {
      err = qio_get_fd(this._file_internal, tempfd);
    }
    if err then try ioerror(err, "in file.fd()");
    return tempfd:int;
  }

  proc tcpConn.addr() throws {

  }

  // proc tcpConn.init=(x: (tcpConn, ipAddr)) {
  //   this.init(x(0));
  // }

  // proc ipAddr.init=(x: (tcpConn, ipAddr)) {
  //   this.init(x(1).host, x(0).port, x(0).family);
  // }

  private extern proc sizeof(e): size_t;

  record tcpListener {
    var socketFd:int(32);
    var host:ipAddr;

    proc init(socketFd:int(32), host:ipAddr) {
      this.socketFd = socketFd;
      this.host = host;
    }
  }

  proc tcpListener.accept():file throws {
    var client_addr:sys_sockaddr_t = new sys_sockaddr_t();
    var fdOut:int(32);
    var rset, allset: fd_set;
    var timeo:timeval = new timeval(10,0);

    sys_fd_zero(allset);
    sys_fd_set(this.socketFd,allset);
    rset = allset;
    var nready:int(32);

    var err = sys_select(socketFd+1,c_ptrTo(rset),nil,nil,c_ptrTo(timeo),nready);
    if(nready == 0){
      writeln("timed out");
    }

    // writeln("select complete ",nready);
    if(sys_fd_isset(socketFd,rset)){
      sleep(1);
      sys_accept(socketFd,client_addr,fdOut);
    }

    // writeln("file descriptor client is ", fdOut);
    var addr = new ipAddr(client_addr);
    // writeln("client socket family ", client_addr.family);
    writeln("Accepted new connection from ", addr.host, ":", addr.port);
    // writeln("client addr on port ", getport);


    // var flags:int(32);
    // sys_fcntl(fdOut,F_GETFL,flags);
    // writeln("Socket Flags are ",flags, " non blocking is set ",flags&O_NONBLOCK);
    var sockFile:tcpConn = openfd(fdOut);
    return sockFile;
  }

  proc tcpListener.close() throws {
    sys_close(socketFd);
  }

  proc tcpListener.addr {
    return host;
  }

  proc tcpListener.family {
    return ipAddr.family;
  }

  proc listen(ref host:ipAddr, reuseaddr=true, backlog=5) throws {
    var family = host.family;
    var socketFd: int(32);
    var err = sys_socket(family,SOCK_STREAM|SOCK_CLOEXEC,0,socketFd);
    if(err != 0){
      throw SystemError.fromSyserr(err,"Failed to create Socket");
    }

    var enable:int = if reuseaddr then 1 else 0;
    if enable {
      var ptrEnable:c_ptr(int) = c_ptrTo(enable);
      var voidPtrEnable:c_void_ptr = ptrEnable;
      sys_setsockopt(socketFd,SOL_SOCKET,SO_REUSEADDR,voidPtrEnable,sizeof(enable):int(32));
    }

    err = sys_bind(socketFd, host._addressStorage);
    if(err != 0){
      throw SystemError.fromSyserr(err,"Failed to bind Socket");
    }

    err = sys_listen(socketFd,backlog:int(32));
    if(err != 0){
      throw SystemError.fromSyserr(err,"Failed to listen on socket");
    }

    const tcpObject = new tcpListener(socketFd, host);
    return tcpObject;
  }
}
