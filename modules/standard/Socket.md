# Socket Module API

To those familiar with the Unix socket API, the method names will feel familiar, though their usage will be somewhat simpler than the raw Unix socket API. The Module will comprise of four records which will abstract lower order functions to higher and more intutive methods for user:
- ipAddr : Address information storage structure.
- tcpConn : A TCP Socket with established connection
- tcpListener : A TCP Socket listening for connections
- udpSocket : A connection less UDP Socket

## Address Information

`ipAddr` serves the purpose of storing address information in a single record irrespective of the protocol used which varies the buffer required to store address.

To initialize an `ipAddr` there are several available overloads and defaults.

**Method**
```python
proc ipAddr.init(host:string = "localhost", port:uint(16) = 8000, family:IPFamily = IPFamily.IPv4)
```
**Parameters**
```
host: string - address string in dot or color notation depending on family. "127.0.0.1" or "::1"
port: uint(16) - port number for socket
family: IPFamily - Internet Protocol version which can be IPFamily.IPv4 or IPFamily.IPv6
```

There are two more initialzers which can take in standard IP Addresses like loopback and broadcast as `host` which are defined as constants inside the `Sys` module.

**Method**
```python
proc ipAddr.init(host:sys_in_addr_t, port: uint(16) = 8000)
proc ipAddr.init(host:sys_in6_addr_t, port: uint(16) = 8000)
```
**Parameter**
```
host: string - standard ip addresses
port: uint(16) - port number for socket
```

Family is determined based on type of standard address used `sys_in_addr_t` for ipv4 and `sys_in6_addr_t` for ipv6. Details about available options can be found [here](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netinet_in.h.html)


`ipAddr` instance created will then have getters for all the three arguments i.e. `host`, `port` and `family` which dont take up any space inside the record and are fetched from [sockaddr_storage](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html) field stored in `ipAddr`.

## Connecting to remote sockets

`connect` function will create `tcpConn` instance which will take in the following parameters

**Method:**
```python
proc connect(in address:ipAddr, in timeout = new timeval(-1,0)) : tcpConn
```

**Parameters:**

```
address: ipAddr - address information of remote socket
timeout: timeval - defines the time in `(seconds, microseconds)` function should wait before
                  throwing `TimeoutError`. `(-1, 0)` implies indefinite blocking.
```

A higher level construct for socket will take in `host` as `string` which can perform DNS resolution and convert to `ipAddr` internally.

**Method:**
```python
proc connect(host:string, port, family:IPFamily = IPFamily.IPv4, in timeout = new timeval(-1,0)) : tcpConn
```

**Parameters:**
```
host: String - host name either in dot-dash or domain form default "localhost"
port: Integer - port number
family: IPFamily - Internet Protocol version which can be IPFamily.IPv4 or IPFamily.IPv6
timeout: timeval - defines the time in `(seconds, microseconds)` function should wait before
                  throwing `TimeoutError`. `(-1, 0)` implies indefinite blocking.
```

The purpose of family is important for address resolution and converting addr string to standard notation as well as validation. How the family can affect address resolution depends on [getaddrinfo](https://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html) function.

The `tcpConn` instance is basically a chapel `file` instance with added methods aligned to its nature as a socket. All the usual `file` methods are available on it:
- `reader`
  will return a reader channel for associated socket
  ```python
  proc tcpConn.writer() : channel
  ```
- `writer`
  will return a writer channel for associated socket
  ```python
  proc tcpConn.writer() : channel
  ```
- `close` calling this on `tcpConn` will close the socket connection and associated `file` too.
  ```python
  proc tcpConn.close()
  ```
- `addr` this function will return a `ipAddr` object which will contain the `host` and `port` where socket is bound.

## Creating a Listener

`listen` function will create a `tcpListener` Instance which will take in following parameters

**Method:**
```python
proc listen(in address:ipAddr, reuseAddr = true, backlog = 5) : tcpListener
```

**Parameters**
```
address: ipAddr - contains family info of socket to create and where to bind it.
reuseaddr: Boolean - optional parameter, default value = true
backlog: int - optional parameter, default value = 0
```

- The `backlog` argument defines the maximum length to which specifies the queue length for completely established sockets waiting to be accepted. If a connection request arrives when the queue is full, the client may receive an error with an indication of `ECONNREFUSED` or, if the underlying protocol supports retransmission, the request may be ignored so that a later reattempt at connection succeeds.
- Calling in `listen` without `reuseaddr` can return an error that some other socket is already bound to the specified address. This can also be a result of re-running the program after only a short waiting period where the `bind` may find address as already in use.

The `tcpListener` instance will have methods on it to call
- `accept`: method will return a `tcpConn` instance of any new incoming connection otherwise will yield if no new connection is available at the instant. The communication can then proceed through the `tcpConn` instance returned
  ```python
  proc tcpListener.accept(in timeout:timeval = new timeval(-1,0)) : tcpConn
  ```
  `accept` will throw`TimeoutError` on not being able to accept any new connection
  before `timeout`.
- `close`,
  calling close on `tcpListener` will close the socket server and it won't accept anymore new connections.
  ```python
  proc tcpListener.close()
  ```
- `family` returns the family of socket `AF_INET` or `AF_INET6`
- `addr` will return an `ipAddr` instance providing details about the address and port at which socket is listening.
  ```python
  proc tcpListener.addr : ipAddr
  ```

The additional support with `tcpConn` and `tcpListener` will be to enable or disable two of the prominent optimization algorithm for TCP Protocol that are:

- `nagle`
  This method will set/unset TCP_NODELAY for provided socket.
  ```python
  proc nagle(socket:tcpConn, tcpNodeDelay:bool = 1)
  proc nagle(socket:tcpListener, tcpNodeDelay:bool = 1);
  proc nagle(socket:udpSocket, tcpNodeDelay:bool = 1);
  ```
- `quickAck`
  This method will set/unset TCP_QUICKACK for provided socket.
  ```python
  proc quickAck(socket:tcpConn, tcpQuickAck:bool = 1)
  proc quickAck(socket:tcpListener, tcpQuickAck:bool = 1);
  proc quickAck(socket:udpSocket, tcpQuickAck:bool = 1);
  ```

## UDP Support

Support for UDP protocol will be through a separate constructor for `udpSocket`. They need not be binded or connected as they are connectionless socket.
```python
const socketServ:udpSocket = new udpSocket(family=IPFamily.IPv4)
```

A generic `bind` function will be used which will take in any of the socket three object and bind it to provided `host` , `port`.

**Method**

```
proc bind(socket, in address:ipAddr, reuseAddr = true)
```

**Parameters**

```
socket : tcpListener|tcpConn|udpSocket - socket object to be bound
address: contains address info about where to bind the socket.
reuseaddr: Boolean - optional parameter, default value = true
```

Bind is responsible for binding any socket to a specified address and port. Here the socket can be anything either a UDP or a TCP Socket. listen under the hood will can in bind to bind `tcpListener` to address and port. With either tcpConn or udpSocket that isn't a requirement as the system assigns any free port and localhost address to them by default.

While `tcpConn` is in a connected state when they are created `udpSocket` are connectionless they are just waiting for the user to use them for sending and receiving purpose only. But for sending and receiving purpose user might require a socket to have a known address that is when bind will be useful for `udpSocket`.

## Data transfer on UDP

>UDP I/O can't work the same as TCP because it's stateless there is no persistent connection between the sockets. So because of the absence of a `file` user has to handle I/O by sending in bytes and specify the amount of bytes they are willing to retrieve.

UDP Sockets will have method to call `recvFrom` on it this will return the `addr`, `port` of client socket and `data` being written to the socket.

**Method:**
```python
proc socketServ.recvFrom(buffer_len:int, in timeout = new timeval(-1,0)) : (address:ipAddr, data:bytes)
```

**Parameters**

```
buffer_len: int - number of bytes to read
timeout: timeval - defines the time in `(seconds, microseconds)` function should wait till `recvfrom` fails with `TimeoutError`. `(-1, 0)` implies indefinite blocking.
reuseaddr: Boolean - optional parameter, default value = true
```

**Return:**
```
address: ipAddr - contains family info of socket that sent data to our UDP Socket.
data: bytes - data received
```

Another alternate method will be `recv` which will return just the `data` and no details abouts about client socket.

__Method:__
```python
proc socketServ.recv(buffer_len: int, in timeout = new timeval(-1,0)) : bytes
```

**Parameters**

```
buffer_len: int - number of bytes to read
timeout: timeval - defines the time in `(seconds, microseconds)` function should wait till `recv` fails with `TimeoutError`. `(-1, 0)` implies indefinite blocking.
reuseaddr: Boolean - optional parameter, default value = true
```

The `send` method on socket will take in `addr`, `timeout`, `flags` and `data` to write to the socket at provided `address`.

__Method:__
```python
proc socketServ.send(data: bytes, in address: ipAddr, in timeout = new timeval(-1,0))
```

**Parameters:**
```
data: bytes - data to send
address: ipAddr - address info for where to send the data.
timeout: timeval - defines the time in `(seconds, microseconds)` function should wait till `send` fails with `TimeoutError`. `(-1, 0)` implies indefinite blocking.
```

## Options on Socket

For providing more granular control over socket options `setSocketOpt` and `getSocketOpt` will be provided which can be used to manipulate socket options. For more information about them [man page](https://man7.org/linux/man-pages/man2/getsockopt.2.html) can be referred as to what options we can set on sockets.

The functions will use Chapel implementation for them defined in the [Sys module](https://chapel-lang.org/docs/modules/standard/Sys.html#Sys.sys_getsockopt) but user won't need to provide in file descriptor manually then can just pass in `tcpConn|tcpListener|udpSocket` rest arguments will remain same.
