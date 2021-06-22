module Socket {
  use Sys;
  use SysError;
  use CPtr;
  use Time;
  use SysCTypes;
  private extern proc sizeof(e): size_t;

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

}
