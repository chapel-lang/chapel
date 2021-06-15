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
- `close`,
- `addr` and
- `setSocketOpt` to set various socket options.

`accept` method will return a `TCPConn` instance and associated `addr` of the connection.

The additional support with `TCPConn` and `TCPListener` will be to enable or disable two of the prominent optimization algorithm for TCP Protocol that are:

- `nagle`
- `quickack`

The support will be in form of separate utility function attached with socket module that can set/unset the optimization based on `boolean` argument

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
