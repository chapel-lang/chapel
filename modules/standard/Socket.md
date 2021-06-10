## Socket Module API

To those familiar with the Unix socket API, the method names will feel familiar, though their usage will be somewhat simpler than the raw Unix socket API.

## Creating a Listener

`socket.listen` function will create a `TCPListener` Instance which will take in following parameters

```chapel
socket.listen([addr:IPAddr,]port:int,backlog=5,reuseaddr=true)
```

The `TCPListener` instance will have methods on it to call
- `accept`,
- `close`,
- `addr` and
- `setSocketOpt` to set various socket options.

`accept` method will return a `TCPConn` instance and associated `addr` of the connection.

## Connecting to remote sockets

`socket.connect` function will create `TCPConn` instance which will take in the following parameters

```chapel
const client = socket.connect(addr:IPAddr,port:int)
```

A higher level construct for socket will take in addr as `string` which will we resolved and converted to `IPAddr` internally.

```chapel
const client = socket.connect(addr:string,port:int)
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

## UDP Support

Support for UDP protocol will be through a separate constructor for `UDPSocket`

A generic `bind` function will be used which will take in any of the socket three object and bind it to provided `host` , `port`.

```chapel
const socketServ = UDPSocket()
bind(socketServ,addr:IPAddr,port:int,reuseAddr=true)
```

## Data transfer on UDP

UDP Sockets will have method to call `recvFrom` on it this will return the `host`, `port` of client socket and `data` being written to the socket.

```chapel
socketServ.recvFrom()
```

Another alternate method will be `recv` which will return just the `data` and no details abouts about client socket.

```chapel
socketServ.recv()
```

The `write` method on socket will take in `host`, `port` and `data` to write to the socket at provided address

```chapel
socketServ.write(host:IPAddr,port:int,data:string)
```
