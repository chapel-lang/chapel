---
layout: page
title: fi_pingpong(1)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}


# NAME

fi_pingpong  \- Quick and simple pingpong test for libfabric


# SYNOPSIS
```
 fi_pingpong [OPTIONS]						start server
 fi_pingpong [OPTIONS] <server address>		connect to server
```


# DESCRIPTION

fi_pingpong is a pingpong test for the core feature of the libfabric library:
transmitting data between two processes. fi_pingpong also displays aggregated
statistics after each test run, and can additionally verify data integrity upon
receipt.

By default, the datagram (FI_EP_DGRAM) endpoint is used for the test, unless
otherwise specified via `-e`.

# HOW TO RUN TESTS

Two copies of the program must be launched: first, one copy must be launched as
the server. Second, another copy is launched with the address of the server.

As a client-server test, each have the following usage model:

## Start the server
```
server$ fi_pingpong
```

## Start the client
```
client$ fi_pingpong <server address>
```


# OPTIONS

The server and client must be able to communicate properly for the fi_pingpong
utility to function. If any of the `-e`, `-I`, `-S`, or `-p` options are used,
then they must be specified on the invocation for both the server and the
client process. If the `-d` option is specified on the server, then the client
will select the appropriate domain if no hint is provided on the client side.
If the `-d` option is specified on the client, then it must also be specified
on the server. If both the server and client specify the `-d` option and the
given domains cannot communicate, then the application will fail.

## Control Messaging

*-B \<src_port\>*
: The non-default source port number of the control socket. If this is not
  provided then the server will bind to port 47592 by default and the client
  will allow the port to be selected automatically.

*-P \<dest_port\>*
: The non-default destination port number of the control socket. If this is not
  provided then the client will connect to 47592 by default. The server ignores
  this option.

## Fabric Filtering

*-p \<provider_name\>*
: The name of the underlying fabric provider (e.g., sockets, psm, usnic, etc.).
  If a provider is not specified via the -p switch, the test will pick one from
  the list of available providers (as returned by fi_getinfo(3)).

*-e \<endpoint\>*
: The type of endpoint to be used for data messaging between the two processes.
  Supported values are dgram, rdm, and msg. For more information on endpoint
  types, see fi_endpoint(3).

*-d \<domain\>*
: The name of the specific domain to be used.

## Test Options

*-I \<iter\>*
: The number of iterations of the test will run.

*-S \<msg_size\>*
: The specific size of the message in bytes the test will use or 'all' to run
  all the default sizes.

*-c*
: Activate data integrity checks at the receiver (note: this will degrade
  performance).

## Utility

*-v*
: Activate output debugging (warning: highly verbose)

*-h*
: Displays help output for the pingpong test.


# USAGE EXAMPLES

## A simple example

### Server: `fi_pingpong -p <provider_name>`
`server$ fi_pingpong -p sockets`

### Client: `fi_pingpong -p <provider_name> <server_addr>`
`client$ fi_pingpong -p sockets 192.168.0.123`

## An example with various options

### Server:
`server$ fi_pingpong -p usnic -I 1000 -S 1024`

### Client:
`client$ fi_pingpong -p usnic -I 1000 -S 1024 192.168.0.123`


Specifically, this will run a pingpong test with:

- usNIC provider
- 1000 iterations
- 1024 bytes message size
- server node as 192.168.0.123

## A longer test

### Server:
`server$ fi_pingpong -p usnic -I 10000 -S all`

### Client:
`client$ fi_pingpong -p usnic -I 10000 -S all 192.168.0.123`


# DEFAULTS

There is no default provider; if a provider is not specified via the `-p`
switch, the test will pick one from the list of available providers (as
returned by fi_getinfo(3)).

If no endpoint type is specified, 'dgram' is used.

The default tested sizes are:  64, 256, 1024, 4096, 65536, and 1048576. The
test will only test sizes that are within the selected endpoints maximum
message size boundary.

# OUTPUT

Each test generates data messages which are accounted for. Specifically, the
displayed statistics at the end are :

 - *bytes*          : number of bytes per message sent
 - *#sent*          : number of messages (ping) sent from the client to the
                      server
 - *#ack*           : number of replies (pong) of the server received by the
                      client
 - *total*          : amount of memory exchanged between the processes
 - *time*           : duration of this single test
 - *MB/sec*         : throughput computed from *total* and *time*
 - *usec/xfer*      : average time for transferring a message outbound (ping or
                      pong) in microseconds
 - *Mxfers/sec*     : average amount of transfers of message outbound per
                      second

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html)
[`fabric`(7)](fabric.7.html),
