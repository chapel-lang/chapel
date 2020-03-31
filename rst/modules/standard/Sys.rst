.. default-domain:: chpl

.. module:: Sys
   :synopsis: Support for low-level programming and system calls.

Sys
===
**Usage**

.. code-block:: chapel

   use Sys;


Support for low-level programming and system calls.

This module is for low-level programming. It provides Chapel versions of
many POSIX/Linux C library or system calls. For documentation on these
system calls and constants, please see your system's manual pages.

Each of the functions in this file provides the same functionality
as the corresponding function without the ``sys_`` prefix, except that
the ``sys_`` versions all return an error code (of type :type:`~SysBasic.err_t`)
and return any other values (such as how much was read) through an out
argument.

For example, you can find more about the ``mmap`` call with:

.. code-block:: sh

  man mmap
  
The call available here, :proc:`sys_mmap`, always returns an error
code (or 0 for no error). The pointer value normally returned by ``mmap``
will be returned through the final ``ret_out`` argument.



.. data:: const O_RDONLY: c_int

.. data:: const O_WRONLY: c_int

.. data:: const O_RDWR: c_int

.. data:: const O_APPEND: c_int

.. data:: const O_CREAT: c_int

.. data:: const O_DSYNC: c_int

.. data:: const O_EXCL: c_int

.. data:: const O_NOCTTY: c_int

.. data:: const O_NONBLOCK: c_int

.. data:: const O_RSYNC: c_int

.. data:: const O_SYNC: c_int

.. data:: const O_TRUNC: c_int

.. data:: const AF_UNIX: c_int

.. data:: const AF_LOCAL: c_int

.. data:: const AF_INET: c_int

.. data:: const AF_INET6: c_int

.. data:: const AF_IPX: c_int

.. data:: const AF_NETLINK: c_int

.. data:: const AF_X25: c_int

.. data:: const AF_AX25: c_int

.. data:: const AF_ATMPVC: c_int

.. data:: const AF_APPLETALK: c_int

.. data:: const AF_PACKET: c_int

.. data:: const SOCK_STREAM: c_int

.. data:: const SOCK_DGRAM: c_int

.. data:: const SOCK_SEQPACKET: c_int

.. data:: const SOCK_RAW: c_int

.. data:: const SOCK_RDM: c_int

.. data:: const SOCK_NONBLOCK: c_int

.. data:: const SOCK_CLOEXEC: c_int

.. data:: const MSG_CONFIRM: c_int

.. data:: const MSG_DONTROUTE: c_int

.. data:: const MSG_DONTWAIT: c_int

.. data:: const MSG_EOR: c_int

.. data:: const MSG_MORE: c_int

.. data:: const MSG_NOSIGNAL: c_int

.. data:: const MSG_OOB: c_int

.. data:: const MSG_CMSG_CLOEXEC: c_int

.. data:: const MSG_ERRQUEUE: c_int

.. data:: const MSG_PEEK: c_int

.. data:: const MSG_TRUNC: c_int

.. data:: const MSG_WAITALL: c_int

.. data:: const MSG_CTRUNC: c_int

.. data:: const SHUT_RD: c_int

.. data:: const SHUT_WR: c_int

.. data:: const SHUT_RDWR: c_int

.. data:: const IPPROTO_IP: c_int

.. data:: const IPPROTO_IPV6: c_int

.. data:: const IPPROTO_TCP: c_int

.. data:: const IPPROTO_UDP: c_int

.. data:: const IP_ADD_MEMBERSHIP: c_int

.. data:: const IP_DROP_MEMBERSHIP: c_int

.. data:: const IP_HDRINCL: c_int

.. data:: const IP_MTU: c_int

.. data:: const IP_MTU_DISCOVER: c_int

.. data:: const IP_MULTICAST_IF: c_int

.. data:: const IP_MULTICAST_LOOP: c_int

.. data:: const IP_MULTICAST_TTL: c_int

.. data:: const IP_OPTIONS: c_int

.. data:: const IP_PKTINFO: c_int

.. data:: const IP_RECVERR: c_int

.. data:: const IP_RECVOPTS: c_int

.. data:: const IP_RECVTOS: c_int

.. data:: const IP_RECVTTL: c_int

.. data:: const IP_RETOPTS: c_int

.. data:: const IP_ROUTER_ALERT: c_int

.. data:: const IP_TOS: c_int

.. data:: const IP_TTL: c_int

.. data:: const IPV6_ADDRFORM: c_int

.. data:: const IPV6_ADD_MEMBERSHIP: c_int

.. data:: const IPV6_DROP_MEMBERSHIP: c_int

.. data:: const IPV6_MTU: c_int

.. data:: const IPV6_MTU_DISCOVER: c_int

.. data:: const IPV6_MULTICAST_HOPS: c_int

.. data:: const IPV6_MULTICAST_IF: c_int

.. data:: const IPV6_MULTICAST_LOOP: c_int

.. data:: const IPV6_PKTINFO: c_int

.. data:: const IPV6_RTHDR: c_int

.. data:: const IPV6_AUTHHDR: c_int

.. data:: const IPV6_DSTOPTS: c_int

.. data:: const IPV6_HOPOPTS: c_int

.. data:: const IPV6_HOPLIMIT: c_int

.. data:: const IPV6_RECVERR: c_int

.. data:: const IPV6_ROUTER_ALERT: c_int

.. data:: const IPV6_UNICAST_HOPS: c_int

.. data:: const IPV6_V6ONLY: c_int

.. data:: const TCP_CORK: c_int

.. data:: const TCP_DEFER_ACCEPT: c_int

.. data:: const TCP_INFO: c_int

.. data:: const TCP_KEEPCNT: c_int

.. data:: const TCP_KEEPIDLE: c_int

.. data:: const TCP_KEEPINTVL: c_int

.. data:: const TCP_LINGER2: c_int

.. data:: const TCP_MAXSEG: c_int

.. data:: const TCP_NODELAY: c_int

.. data:: const TCP_QUICKACK: c_int

.. data:: const TCP_SYNCNT: c_int

.. data:: const TCP_WINDOW_CLAMP: c_int

.. type:: type sys_sockaddr_storage_t

   SOCKET STRUCTURE TYPES 

.. record:: sys_sockaddr_t

   .. attribute:: var addr: sys_sockaddr_storage_t

   .. attribute:: var len: socklen_t

   .. method:: proc init()

.. record:: sys_addrinfo_t

   .. attribute:: var ai_flags: c_int

   .. attribute:: var ai_family: c_int

   .. attribute:: var ai_socktype: c_int

   .. attribute:: var ai_protocol: c_int

.. type:: type sys_addrinfo_ptr_t

.. method:: proc sys_addrinfo_ptr_t.flags: c_int

.. method:: proc sys_addrinfo_ptr_t.family: c_int

.. method:: proc sys_addrinfo_ptr_t.socktype: c_int

.. method:: proc sys_addrinfo_ptr_t.socktype: c_int

.. method:: proc sys_addrinfo_ptr_t.addr: sys_sockaddr_t

.. method:: proc sys_addrinfo_ptr_t.next: sys_addrinfo_ptr_t

.. function:: proc sys_init_sys_sockaddr(ref addr: sys_sockaddr_t)

.. function:: proc sys_strerror(error: err_t, ref string_out: c_string): err_t

.. function:: proc sys_readlink(path: c_string, ref string_out: c_string): err_t

.. function:: proc sys_getenv(name: c_string, ref string_out: c_string): c_int

.. function:: proc sys_open(pathname: c_string, flags: c_int, mode: mode_t, ref fd_out: fd_t): err_t

.. function:: proc sys_close(fd: fd_t): err_t

.. function:: proc sys_mmap(addr: c_void_ptr, length: size_t, prot: c_int, flags: c_int, fd: fd_t, offset: off_t, ref ret_out: c_void_ptr): err_t

.. function:: proc sys_munmap(addr: c_void_ptr, length: size_t): err_t

.. function:: proc sys_fcntl(fd: fd_t, cmd: c_int, ref ret_out: c_int): err_t

.. function:: proc sys_fcntl_long(fd: fd_t, cmd: c_int, arg: c_long, ref ret_out: c_int): err_t

.. function:: proc sys_fcntl_ptr(fd: fd_t, cmd: c_int, arg: c_void_ptr, ref ret_out: c_int): err_t

.. function:: proc sys_dup(oldfd: fd_t, ref fd_out: fd_t): err_t

.. function:: proc sys_dup2(oldfd: fd_t, newfd: fd_t, ref fd_out: fd_t): err_t

.. function:: proc sys_pipe(ref read_fd_out: fd_t, ref write_fd_out: fd_t): err_t

.. function:: proc sys_accept(sockfd: fd_t, ref add_out: sys_sockaddr_t, ref fd_out: fd_t): err_t

.. function:: proc sys_bind(sockfd: fd_t, ref addr: sys_sockaddr_t): err_t

.. function:: proc sys_connect(sockfd: fd_t, ref addr: sys_sockaddr_t): err_t

.. function:: proc sys_getaddrinfo(node: c_string, service: c_string, ref hints: sys_addrinfo_t, ref res_out: sys_addrinfo_ptr_t): err_t

.. function:: proc sys_getaddrinfo_flags(res: sys_addrinfo_ptr_t): c_int

.. function:: proc sys_getaddrinfo_family(res: sys_addrinfo_ptr_t): c_int

.. function:: proc sys_getaddrinfo_socktype(res: sys_addrinfo_ptr_t): c_int

.. function:: proc sys_getaddrinfo_protocol(res: sys_addrinfo_ptr_t): c_int

.. function:: proc sys_getaddrinfo_addr(res: sys_addrinfo_ptr_t): sys_sockaddr_t

.. function:: proc sys_getaddrinfo_next(res: sys_addrinfo_ptr_t): sys_addrinfo_ptr_t

.. function:: proc sys_freeaddrinfo(res: sys_addrinfo_ptr_t)

.. function:: proc sys_getnameinfo(ref addr: sys_sockaddr_t, ref host_out: c_string, ref serv_outc_: c_string, flags: c_int): err_t

.. function:: proc sys_getpeername(sockfd: fd_t, ref addr: sys_sockaddr_t): err_t

.. function:: proc sys_getsockname(sockfd: fd_t, ref addr: sys_sockaddr_t): err_t

.. function:: proc sys_getsockopt(sockfd: fd_t, level: c_int, optname: c_int, optval: c_void_ptr, ref optlen: socklen_t): err_t

.. function:: proc sys_setsockopt(sockfd: fd_t, level: c_int, optname: c_int, optval: c_void_ptr, optlen: socklen_t): err_t

.. function:: proc sys_listen(sockfd: fd_t, backlog: c_int): err_t

.. function:: proc sys_shutdown(sockfd: fd_t, how: c_int): err_t

.. function:: proc sys_socket(_domain: c_int, _type: c_int, protocol: c_int, ref sockfd_out: fd_t): err_t

.. function:: proc sys_socketpair(_domain: c_int, _type: c_int, protocol: c_int, ref sockfd_out_a: fd_t, ref sockfd_out_b: fd_t): err_t

.. type:: type fd_set

.. type:: type time_t = c_long

.. type:: type suseconds_t = c_long

.. record:: timeval

   .. attribute:: var tv_sec: time_t

   .. attribute:: var tv_usec: suseconds_t

.. function:: proc sys_select(nfds: c_int, readfds: c_ptr(fd_set), writefds: c_ptr(fd_set), exceptfds: c_ptr(fd_set), timeout: c_ptr(timeval), ref nset: c_int): err_t

.. function:: proc sys_fd_clr(fd: c_int, ref set: fd_set)

.. function:: proc sys_fd_isset(fd: c_int, ref set: fd_set): c_int

.. function:: proc sys_fd_set(fd: c_int, ref set: fd_set)

.. function:: proc sys_fd_zero(ref set: fd_set)

