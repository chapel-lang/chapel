## Socket Module API

To those familiar with the Unix socket API, the method names will feel familiar, though their usage will be somewhat simpler than the raw Unix socket API. The Module will comprise of three classes which will abstract lower order functions to higher and more intutive methods for user:
- TCPConn : A TCP Socket with established connection
- TCPListener : A TCP Socket listening for connections
- UDPSocket : A connection less UDP Socket

## Connecting to remote sockets

`connect` function will create `TCPConn` instance which will take in the following parameters

**Method:**
```chapel
proc connect(addr:ipAddr,port:int) : TCPConn
```

**Parameters:**

```
addr: ipAddr - ipv4/ipv6 address default localhost
port: Integer - port number
```

A higher level construct for socket will take in addr as `string` which will we resolved and converted to `ipAddr` internally.

**Method:**
```chapel
proc connect(addr:string,port:int) : TCPConn
```

**Parameters:**
```
addr: String - host name either in dot-dash or domain form default localhost
port: Integer - port number
```

The `TCPConn` instance will have methods on it to call
- `read` ,
- `write` ,
- `close`,
- `closeWrite`,
- `closeRead`,
- `setKeepAlive`,
- `addr` ,
- `setReadBuffer`
- and `setWriteBuffer`

## Creating a Listener

`listen` function will create a `TCPListener` Instance which will take in following parameters

**Method:**
```chapel
proc listen(addr:ipAddr,port:int,backlog=5,reuseaddr=true) : TCPListener
```

**Parameters**
```
addr: ipAddr - ipv4/ipv6 address default localhost
port: Integer - port number
backlog: Integer - optional parameter, default value = 0
reuseaddr: Boolean - optional parameter, default value = true
```

The `TCPListener` instance will have methods on it to call
- `accept`,
  `accept` method will return a `TCPConn` instance of any new incoming connection otherwise will yield if no new connection is available at the instant. The communication can then proceed through the `TCPConn` instance returned
  ```python3
  proc TCPListener.accept() : TCPListener
  ```
- `close`,
  calling close on `TCPListener` will close the socket server and it won't accept anymore new connections.
  ```python3
  proc TCPListener.close()
  ```
- `addr` and
  addr will return an `ipAddr` instance providing details about the address and port at which socket is listening.
  ```python3
  proc TCPListener.addr() : ipAddr
  ```
- `setSocketOpt` to set various socket options.
  `setSocketOpt` will provide method to set arguments like `SO_KEEPALIVE`, `SO_BROADCAST`, etc. options on the socket instance. More options related to the method can be found at [man page](https://linux.die.net/man/3/setsockopt)

The additional support with `TCPConn` and `TCPListener` will be to enable or disable two of the prominent optimization algorithm for TCP Protocol that are:

- `nagle`
  This method will set/unset TCP_NODELAY for provided socket.
  ```python3
  proc nagle(socket:TCPConn, tcpNodeDelay:bool = 1)
  proc nagle(socket:TCPListener, tcpNodeDelay:bool = 1);
  proc nagle(socket:UDPSocket, tcpNodeDelay:bool = 1);
  ```
- `quickack`
  This method will set/unset TCP_QUICKACK for provided socket.
  ```python3
  proc quickack(socket:TCPConn, tcpQuickack:bool = 1)
  proc qucikack(socket:TCPListener, tcpQuickack:bool = 1);
  proc quickack(socket:UDPSocket, tcpQuickack:bool = 1);
  ```

## UDP Support

Support for UDP protocol will be through a separate constructor for `UDPSocket`
```chapel
const socketServ:UDPSocket = new UDPSocket()
```
A generic `bind` function will be used which will take in any of the socket three object and bind it to provided `host` , `port`.

**Method**

```
proc bind(socketServ,addr:ipAddr,port:int,reuseAddr=true)
```

**Parameters**

```
- socket : TCPListener|TCPConn|UDPSocket - socket object to be bound
- addr: ipAddr - ipv4/ipv6 address default localhost
- port: Integer - port number
- reuseaddr: Boolean - optional parameter, default value = true
```

## Data transfer on UDP

UDP Sockets will have method to call `recvFrom` on it this will return the `addr`, `port` of client socket and `data` being written to the socket.

**Method:**
```chapel
proc socketServ.recvFrom() : (addr:ipAddr, port:int, data:Generic)
```

**Return:**
```
- addr: ipAddr - ipv4/ipv6 address of sender socket
- port: int - related port number
- data: Generic - data to send
```


Another alternate method will be `recv` which will return just the `data` and no details abouts about client socket.

__Method:__
```chapel
proc socketServ.recv() : (data:Generic)
```

The `send` method on socket will take in `addr`, `port` and `data` to write to the socket at provided address

__Method:__
```chapel
proc socketServ.send(addr:ipAddr,port:int,data:Generic)
```

**Parameters:**
```
- addr: ipAddr - ipv4/ipv6 address of sender socket default localhost
- port: int - related port number
- data: Generic - data to send
```
