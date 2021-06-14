## Socket Module API

To those familiar with the Unix socket API, the method names will feel familiar, though their usage will be somewhat simpler than the raw Unix socket API.

## Connecting to remote sockets

`connect` function will create `TCPConn` instance which will take in the following parameters

**Method:**
```chapel
connect(addr:IPAddr,port:int) -> TCPConn
```

**Parameters:**

```
addr: IPAddr - ipv4/ipv6 address
port: Integer - port number
```

A higher level construct for socket will take in addr as `string` which will we resolved and converted to `IPAddr` internally.

**Method:**
```chapel
connect(addr:string,port:int) -> TCPConn
```

**Parameters:**
```
addr: String - host name either in dot-dash or domain form
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

The additional support with `TCPConn` and `TCPListener` will be to enable or disable two of the prominent optimization algorithm for TCP Protocol that are:

- `nagle`
- `quickack`

The support will be in form of separate utility function attached with socket module that can set/unset the optimization based on `boolean` argument

## Creating a Listener

`listen` function will create a `TCPListener` Instance which will take in following parameters

**Method:**
```chapel
listen(addr:IPAddr,port:int,backlog=5,reuseaddr=true) -> TCPListener
```

**Parameters**
```
addr: IPAddr - ipv4/ipv6 address
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

## UDP Support

Support for UDP protocol will be through a separate constructor for `UDPSocket`
```chapel
const socketServ:UDPSocket = new UDPSocket()
```
A generic `bind` function will be used which will take in any of the socket three object and bind it to provided `host` , `port`.

**Method**

```
bind(socketServ,addr:IPAddr,port:int,reuseAddr=true) -> void
```

**Parameters**

```
- socket : TCPListener|TCPConn|UDPSocket - socket object to be binded
- addr: IPAddr - ipv4/ipv6 address
- port: Integer - port number
- reuseaddr: Boolean - optional parameter, default value = true
```

## Data transfer on UDP

UDP Sockets will have method to call `recvFrom` on it this will return the `host`, `port` of client socket and `data` being written to the socket.

**Method:**
```chapel
socketServ.recvFrom() -> (host, port, data)
```

**Return:**
```
- host: IPAddr - ipv4/ipv6 address of sender socket
- port: int - related port number
- data: Generic - data to send
```


Another alternate method will be `recv` which will return just the `data` and no details abouts about client socket.

__Method:__
```chapel
socketServ.recv() -> data
```

The `write` method on socket will take in `host`, `port` and `data` to write to the socket at provided address

__Method:__
```chapel
socketServ.send(host:IPAddr,port:int,data)
```

**Parameters:**
```
- host: IPAddr - ipv4/ipv6 address of sender socket
- port: int - related port number
- data: Generic - data to send
```
