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
  use Sys;
  use SysError;
  use CPtr;
  use Time;
  use SysCTypes;
  private extern proc sizeof(e): size_t;

  extern { #include "qthread/qt_syscalls.h" }
  extern { #include "qthread/io.h" }

  extern "qt_begin_blocking_action" proc beginBlocking();
  extern "qt_end_blocking_action" proc endBlocking();

  enum IPFamily {
    IPv4 = 2,
    IPv6 = 10
  }

  const IPv4Localhost = INADDR_LOOPBACK;
  const IPv6Localhost = in6addr_loopback;

  record InetAddr {
    var port;
    var address:string;
    var _addressStorage:sys_sockaddr_t;

    proc init(inout address:string = "localhost", port:uint(16), family:IPFamily = IPFamily.IPv4) {
      this.port = port;
      this.address = address;
      _addressStorage = new sys_sockaddr_t();
      if(address == "localhost"){
        address = if family == AF_INET then "127.0.0.1" else "::1";
      }

      _addressStorage.fill(address, port, family);
    }

    proc init(address, port: uint(16)) where address.type == sys_in_addr_t || sys_in6_addr_t {
      this.port = port;
      this.address = address;
      _addressStorage = new sys_sockaddr_t();

      _addressStorage.fill(address,port);
    }
  }

  proc listen(in address:string,port:uint(16),reuseaddr=true) throws {
    var family = AF_INET;
    var socketFD: int(32);
    var err = sys_socket(family,SOCK_STREAM|SOCK_CLOEXEC,0,socketFD);
    if(err != 0){
      throw SystemError.fromSyserr(err,"Failed to create Socket");
    }
    writeln("Socket File Descriptor is:",socketFD);


    var flags:int(32);
    sys_fcntl(socketFD,F_GETFL,flags);
    writeln("Socket Flags are ",flags, " non blocking is set ",flags&O_NONBLOCK);


    var enable:int = if reuseaddr then 1 else 0;
    var ptrEnable:c_ptr(int) = c_ptrTo(enable);
    var voidPtrEnable:c_void_ptr = ptrEnable;
    sys_setsockopt(socketFD,SOL_SOCKET,SO_REUSEADDR,voidPtrEnable,sizeof(enable):int(32));
    writeln("Reuse address ",reuseaddr);

    var socket_addr:sys_sockaddr_t = new sys_sockaddr_t();
    if(address == "localhost"){
      address = if family == AF_INET then "127.0.0.1" else "::1";
    }
    err = socket_addr.fill(address.c_str(), port, family);
    if(err != 1){
      throw new Error("Invalid/Incompatible Address");
    }
    writeln("socket fill complete ");


    var familySize = if socket_addr.family == AF_INET then INET_ADDRSTRLEN else INET6_ADDRSTRLEN;
    var buffer = c_calloc(c_char,familySize);
    var getport:uint(16);
    sys_extract_sys_sockaddr_t(socket_addr,buffer,getport);
    var getAddr = createStringWithOwnedBuffer(buffer,familySize,familySize);
    writeln("socket family ", socket_addr.family);
    writeln("socket addr on address ", getAddr);
    writeln("socket addr on port ", getport);


    err = sys_bind(socketFD,socket_addr);
    if(err != 0){
      throw SystemError.fromSyserr(err,"Failed to bind Socket");
    }
    writeln("socket binded on port ", port);


    err = sys_listen(socketFD,300);
    if(err != 0){
      throw SystemError.fromSyserr(err,"Failed to listen on socket");
    }
    writeln("socket listening");

    err = sys_close(socketFD);
    if(err != 0){
      throw SystemError.fromSyserr(err,"Failed to close Socket");
    }
    writeln("Socket Closed");
  }

  proc accept(socketFD:int(32)) throws {
    // var rset, allset: fd_set;
    // var timeo:timeval = new timeval(10,0);

    // sys_fd_zero(allset);
    // sys_fd_set(socketFD,allset);
    // rset = allset;
    // var nready:int(32);

    // writeln("going to select now, selecting on ", sys_fd_isset(socketFD,rset));

    // var err = sys_select(socketFD+1,c_ptrTo(rset),nil,nil,c_ptrTo(timeo),nready);
    // if(err == 0){
    //   writeln("timed out");
    // }

    // writeln("select complete ",nready);
    // if(sys_fd_isset(socketFD,rset)){
    //   sleep(1);
    //   var client_addr:sys_sockaddr_t = new sys_sockaddr_t();
    //   var fdOut:int(32);
    //   sys_accept(socketFD,client_addr,fdOut);
    //   writeln(fdOut);
    // }
    writeln("blocking socket now");
    var client_addr:sys_sockaddr_t = new sys_sockaddr_t();
    var fdOut:int(32);
    beginBlocking();
    sys_accept(socketFD,client_addr,fdOut);
    endBlocking();

    writeln("file descriptor client is ", fdOut);

    var familySize = if client_addr.family == AF_INET then INET_ADDRSTRLEN else INET6_ADDRSTRLEN;
    var buffer = c_calloc(c_char,familySize);
    var getport:uint(16);
    sys_extract_sys_sockaddr_t(client_addr,buffer,getport);
    var getAddr = createStringWithOwnedBuffer(buffer,familySize,familySize);
    writeln("client socket family ", client_addr.family);
    writeln("client addr on address ", getAddr);
    writeln("client addr on port ", getport);

    sys_close(fdOut);
    writeln("client closed");
    sleep(1);
  }
}
