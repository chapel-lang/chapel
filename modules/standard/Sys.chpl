/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

/*
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

 */
module Sys {
  // get basic types from SysBasic
  use SysBasic;
  private use CTypes;
  private use SysError;


  // CONSTANTS

  // note that if a constant is not defined in the C environment,
  // the compile will fail when it is used (and not here!)
  // That means that we can just list all of the constants we know of,
  // for different OSs, and if one is missing errors will occur in
  // C code generation.
  // mferguson -- TODO -- is that still true? Even for LLVM?

  // fcntl command flags
  extern const F_GETFL:c_int;
  extern const F_SETFL:c_int;
  extern const F_GETFD:c_int;
  extern const F_SETFD:c_int;

  // basic file flags
  extern const O_RDONLY:c_int;
  extern const O_WRONLY:c_int;
  extern const O_RDWR:c_int;

  // POSIX flags
  extern const O_APPEND:c_int;
  extern const O_CREAT:c_int;
  extern const O_DSYNC:c_int;
  extern const O_EXCL:c_int;
  extern const O_NOCTTY:c_int;
  extern const O_NONBLOCK:c_int;
  extern const O_RSYNC:c_int;
  extern const O_SYNC:c_int;
  extern const O_TRUNC:c_int;

  // Linux flags (make them 0 if not on linux, please)
  //extern const O_ASYNC:c_int;
  //extern const O_CLOEXEC:c_int;
  //extern const O_DIRECT:c_int;
  //extern const O_DIRECTORY:c_int;
  //extern const O_LARGEFILE:c_int;
  //extern const O_NOATIME:c_int;
  //extern const O_NOFOLLOW:c_int;
  //extern const O_NDELAY:c_int;

  // open permission flags.
  // These are too complicated for the LLVM backend!
  //extern const S_IRWXU:c_int;
  //extern const S_IRUSR:c_int;
  //extern const S_IWUSR:c_int;
  //extern const S_IXUSR:c_int;
  //extern const S_IRWXG:c_int;
  //extern const S_IRGRP:c_int;
  //extern const S_IWGRP:c_int;
  //extern const S_IXGRP:c_int;
  //extern const S_IRWXO:c_int;
  //extern const S_IROTH:c_int;
  //extern const S_IWOTH:c_int;
  //extern const S_IXOTH:c_int;

  // socket domains
  extern const AF_UNIX:c_int;
  extern const AF_LOCAL:c_int;
  extern const AF_INET:c_int;
  extern const AF_INET6:c_int;
  extern const AF_IPX:c_int;
  extern const AF_NETLINK:c_int;
  extern const AF_X25:c_int;
  extern const AF_AX25:c_int;
  extern const AF_ATMPVC:c_int;
  extern const AF_APPLETALK:c_int;
  extern const AF_PACKET:c_int;

  // socket types
  extern const SOCK_STREAM:c_int;
  extern const SOCK_DGRAM:c_int;
  extern const SOCK_SEQPACKET:c_int;
  extern const SOCK_RAW:c_int;
  extern const SOCK_RDM:c_int;

  extern const SOCK_NONBLOCK:c_int;
  extern const SOCK_CLOEXEC:c_int;

  // sendmsg flags.
  extern const MSG_CONFIRM:c_int;
  extern const MSG_DONTROUTE:c_int;
  extern const MSG_DONTWAIT:c_int;
  extern const MSG_EOR:c_int;
  extern const MSG_MORE:c_int;
  extern const MSG_NOSIGNAL:c_int;
  extern const MSG_OOB:c_int;

  // recvmsg flags
  extern const MSG_CMSG_CLOEXEC:c_int;
  extern const MSG_ERRQUEUE:c_int;
  extern const MSG_PEEK:c_int;
  extern const MSG_TRUNC:c_int;
  extern const MSG_WAITALL:c_int;

  //extern const MSG_EOR:c_int; sendmsg flag
  extern const MSG_CTRUNC:c_int;

  // shutdown how.
  extern const SHUT_RD:c_int;
  extern const SHUT_WR:c_int;
  extern const SHUT_RDWR:c_int;

  // socket option 'levels'
  extern const SOL_SOCKET:c_int;
  extern const IPPROTO_IP:c_int;
  extern const IPPROTO_IPV6:c_int;
  extern const IPPROTO_TCP:c_int;
  extern const IPPROTO_UDP:c_int;

  // socket options
  extern const SO_ACCEPTCONN:c_int;
  extern const SO_BROADCAST:c_int;
  extern const SO_DEBUG:c_int;
  extern const SO_ERROR:c_int;
  extern const SO_KEEPALIVE:c_int;
  extern const SO_LINGER:c_int;
  extern const SO_OOBINLINE:c_int;
  extern const SO_RCVBUF:c_int;
  extern const SO_RCVTIMEO:c_int;
  extern const SO_REUSEADDR:c_int;
  extern const SO_SNDBUF:c_int;
  extern const SO_SNDTIMEO:c_int;
  extern const SO_SECINFO:c_int;

  // IP socket options
  extern const IP_ADD_MEMBERSHIP:c_int;
  extern const IP_DROP_MEMBERSHIP:c_int;
  extern const IP_HDRINCL:c_int;
  extern const IP_MTU:c_int;
  extern const IP_MTU_DISCOVER:c_int;
  extern const IP_MULTICAST_IF:c_int;
  extern const IP_MULTICAST_LOOP:c_int;
  extern const IP_MULTICAST_TTL:c_int;
  extern const IP_OPTIONS:c_int;
  extern const IP_PKTINFO:c_int;
  extern const IP_RECVERR:c_int;
  extern const IP_RECVOPTS:c_int;
  extern const IP_RECVTOS:c_int;
  extern const IP_RECVTTL:c_int;
  extern const IP_RETOPTS:c_int;
  extern const IP_ROUTER_ALERT:c_int;
  extern const IP_TOS:c_int;
  extern const IP_TTL:c_int;

  // IP 6 socket options
  extern const IPV6_ADDRFORM:c_int;
  extern const IPV6_ADD_MEMBERSHIP:c_int;
  extern const IPV6_DROP_MEMBERSHIP:c_int;
  extern const IPV6_MTU:c_int;
  extern const IPV6_MTU_DISCOVER:c_int;
  extern const IPV6_MULTICAST_HOPS:c_int;
  extern const IPV6_MULTICAST_IF:c_int;
  extern const IPV6_MULTICAST_LOOP:c_int;
  extern const IPV6_PKTINFO:c_int;
  extern const IPV6_RTHDR:c_int;
  extern const IPV6_AUTHHDR:c_int;
  extern const IPV6_DSTOPTS:c_int;
  extern const IPV6_HOPOPTS:c_int;
  //extern const IPV6_FLOWINFO:c_int;
  extern const IPV6_HOPLIMIT:c_int;
  extern const IPV6_RECVERR:c_int;
  extern const IPV6_ROUTER_ALERT:c_int;
  extern const IPV6_UNICAST_HOPS:c_int;
  extern const IPV6_V6ONLY:c_int;

  // TCP socket options
  extern const TCP_CORK:c_int;
  extern const TCP_DEFER_ACCEPT:c_int;
  extern const TCP_INFO:c_int;
  extern const TCP_KEEPCNT:c_int;
  extern const TCP_KEEPIDLE:c_int;
  extern const TCP_KEEPINTVL:c_int;
  extern const TCP_LINGER2:c_int;
  extern const TCP_MAXSEG:c_int;
  extern const TCP_NODELAY:c_int;
  extern const TCP_QUICKACK:c_int;
  extern const TCP_SYNCNT:c_int;
  extern const TCP_WINDOW_CLAMP:c_int;

  // socket address sizes
  extern const INET_ADDRSTRLEN:c_int;
  extern const INET6_ADDRSTRLEN:c_int;
  extern const NI_MAXHOST:c_int;
  extern const NI_MAXSERV:c_int;

  // standard ipv4 addresses
  extern const INADDR_ANY:sys_in_addr_t;
  extern const INADDR_BROADCAST:sys_in_addr_t;
  extern const INADDR_LOOPBACK:sys_in_addr_t;

  // standard ipv6 addresses
  extern const in6addr_any:sys_in6_addr_t;
  extern const in6addr_loopback:sys_in6_addr_t;

  // UDP socket options
  //extern const UDP_CORK:c_int;


  /* SOCKET STRUCTURE TYPES */

  extern type sys_in_addr_t;
  extern type sys_in6_addr_t;

  extern type sys_sockaddr_storage_t;
  /* The type corresponding to C's socklen_t */
  extern type socklen_t = int(32);
  extern record sys_sockaddr_t {
    var addr:sys_sockaddr_storage_t;
    var len:socklen_t;

    proc init() {
      this.complete();
      sys_init_sys_sockaddr_t(this);
    }

    /*
    Initialize sys_sockaddr_t with provided `family`, `host` and
    `port`. `host` should be provided in standard notation as per
    family. Note : `host` isn't resolved using DNS Lookup.

    :arg host: hostname address in ipv4 or ipv6 string notation
    :type host: `string`

    :arg port: port number
    :type port: `c_uint`

    :arg family: domain of socket
    :type family: `c_int`

    :throws IllegalArgumentError: Upon failure to provide a compatible
                                  `host` and `family`.
    */
    pragma "no doc"
    proc set(host: c_string, port: c_uint, family: c_int) throws {
      var err_out = sys_set_sys_sockaddr_t(this, host, port, family);
      if err_out != 1 {
        throw new IllegalArgumentError("Incompatible Address and Family");
      }
    }

    /*
    Initialize sys_sockaddr_t with provided `host` and
    `port`. `host` should be one of the standard ipv4
    addresses. family for socket address is assumed to be
    `AF_INET` based on `host` address being ipv4.

    :arg host: standard hostname address ipv6
    :type host: `sys_in_addr_t`

    :arg port: port number
    :type port: `c_uint`
    */
    pragma "no doc"
    proc set(host: sys_in_addr_t, port: c_uint) {
      sys_set_sys_sockaddr_in_t(this, host, port);
    }

    /*
    Initialize sys_sockaddr_t with provided `host` and
    `port`. `host` should be one of the standard ipv6
    addresses. family for socket address is assumed to be
    `AF_INET6` based on `host` address being ipv6.

    :arg host: standard hostname address ipv6
    :type host: `sys_in6_addr_t`

    :arg port: port number
    :type port: `c_uint`
    */
    pragma "no doc"
    proc set(host: sys_in6_addr_t, port: c_uint) {
      sys_set_sys_sockaddr_in6_t(this, host, port);
    }

    /*
    Returns the `host` address stored in record.

    :return: Returns numeric host string.
    :rtype: `string`

    :throws Error: If record was uninitialized and has no information
                   about `host` or `port`.
    */
    pragma "no doc"
    proc const ref numericHost() throws {

      var buffer = c_calloc(c_char, NI_MAXHOST);
      var length:c_int;

      var err_out = sys_host_sys_sockaddr_t(this, buffer, NI_MAXHOST, length);
      if err_out != 0 {
        throw SystemError.fromSyserr(err_out);
      }

      return createStringWithOwnedBuffer(buffer, length, NI_MAXHOST);
    }

    /*
    Returns the `port` stored in record.

    :return: Returns numeric port.
    :rtype: `c_uint`

    :throws Error: If record was uninitialized and has no information
                   about `host` or `port`.
    */
    pragma "no doc"
    proc const ref port() throws {
      var port:c_uint;

      var err_out = sys_port_sys_sockaddr_t(this, port);
      if err_out != 0 {
        throw SystemError.fromSyserr(err_out);
      }

      return port;
    }
  }

  /*
    Returns socket family.

    :returns: a socket family
    :rtype: `c_int`
  */
  proc const ref sys_sockaddr_t.family:c_int { return sys_getsockaddr_family(this); }

  extern "struct addrinfo" record sys_addrinfo_t {
    var ai_flags: c_int;
    var ai_family: c_int;
    var ai_socktype: c_int;
    var ai_protocol: c_int;
    var ai_addrlen: socklen_t;
    var ai_next: c_ptr(sys_addrinfo_t);
  }
  type sys_addrinfo_ptr_t = c_ptr(sys_addrinfo_t);

  proc sys_addrinfo_ptr_t.flags:c_int { return sys_getaddrinfo_flags(this); }
  proc sys_addrinfo_ptr_t.family:c_int { return sys_getaddrinfo_family(this); }
  proc sys_addrinfo_ptr_t.socktype:c_int { return sys_getaddrinfo_socktype(this); }
  proc sys_addrinfo_ptr_t.addr:sys_sockaddr_t { return sys_getaddrinfo_addr(this); }
  // Not supported yet
  // proc sys_addrinfo_ptr_t.canonname:c_string { return sys_getaddrinfo_canonname(this); }
  proc sys_addrinfo_ptr_t.next:sys_addrinfo_ptr_t { return sys_getaddrinfo_next(this); }

  extern proc sys_init_sys_sockaddr_t(ref addr:sys_sockaddr_t);
  extern proc sys_getsockaddr_family(const ref addr: sys_sockaddr_t):c_int;
  extern proc sys_set_sys_sockaddr_t(ref addr: sys_sockaddr_t, host: c_string, port: c_uint, family: c_int):c_int;
  extern proc sys_set_sys_sockaddr_in_t(ref addr: sys_sockaddr_t, host:sys_in_addr_t, port:c_uint);
  extern proc sys_set_sys_sockaddr_in6_t(ref addr: sys_sockaddr_t, host:sys_in6_addr_t, port:c_uint);
  extern proc sys_host_sys_sockaddr_t(const ref addr: sys_sockaddr_t, host: c_ptr(c_char), hostlen: socklen_t, ref length: c_int) : c_int;
  extern proc sys_port_sys_sockaddr_t(const ref addr: sys_sockaddr_t, ref port: c_uint) : c_int;
  extern proc sys_strerror(error:err_t, ref string_out:c_string):err_t;

  extern proc sys_readlink(path:c_string, ref string_out:c_string):err_t;

  /*Check whether or not the environment variable ``name`` is defined.
    If ``name`` is defined then return 1 and update ``string_out``
    to store the value of the environment variable
    otherwise the function returns 0.

    :arg name: name of the environment variable
    :type name: `c_string`

    :arg string_out: store the value of ``name`` environment variable if defined
    :type string_out: `c_string`

    :returns: 1 if ``name`` is defined and 0 if not
    :rtype: `c_int`
   */
  extern proc sys_getenv(name:c_string, ref string_out:c_string):c_int;
  /* The type corresponding to C's mode_t */
  extern type mode_t = uint(32);
  extern proc sys_open(pathname:c_string, flags:c_int, mode:mode_t, ref fd_out:fd_t):err_t;
  extern proc sys_close(fd:fd_t):err_t;

  /* The type corresponding to C's off_t */
  extern type off_t = int(64);

  extern proc sys_mmap(addr:c_void_ptr, length:c_size_t, prot:c_int, flags:c_int, fd:fd_t, offset:off_t, ref ret_out:c_void_ptr):err_t;
  extern proc sys_munmap(addr:c_void_ptr, length:c_size_t):err_t;

  // readv, writev, preadv, pwritev -- can't (yet) pass array.

  extern proc sys_fcntl(fd:fd_t, cmd:c_int, ref ret_out:c_int):err_t;
  extern proc sys_fcntl_long(fd:fd_t, cmd:c_int, arg:c_long, ref ret_out:c_int):err_t;
  extern proc sys_fcntl_ptr(fd:fd_t, cmd:c_int, arg:c_void_ptr, ref ret_out:c_int):err_t;
  extern proc sys_dup(oldfd:fd_t, ref fd_out:fd_t):err_t;
  extern proc sys_dup2(oldfd:fd_t, newfd:fd_t, ref fd_out:fd_t):err_t;
  extern proc sys_pipe(ref read_fd_out:fd_t, ref write_fd_out:fd_t):err_t;
  extern proc sys_accept(sockfd:fd_t, ref add_out:sys_sockaddr_t, ref fd_out:fd_t):err_t;
  extern proc sys_bind(sockfd:fd_t, const ref addr:sys_sockaddr_t):err_t;
  extern proc sys_connect(sockfd:fd_t, const ref addr:sys_sockaddr_t):err_t;
  extern proc getaddrinfo(node:c_string, service:c_string, ref hints:sys_addrinfo_t, ref res_out:sys_addrinfo_ptr_t):err_t;
  extern proc sys_getaddrinfo_flags(res:sys_addrinfo_ptr_t):c_int;
  extern proc sys_getaddrinfo_family(res:sys_addrinfo_ptr_t):c_int;
  extern proc sys_getaddrinfo_socktype(res:sys_addrinfo_ptr_t):c_int;
  extern proc sys_getaddrinfo_protocol(res:sys_addrinfo_ptr_t):c_int;
  extern proc sys_getaddrinfo_addrlen(res:sys_addrinfo_ptr_t):socklen_t;
  extern proc sys_getaddrinfo_addr(res:sys_addrinfo_ptr_t):sys_sockaddr_t;
  extern proc sys_getaddrinfo_next(res:sys_addrinfo_ptr_t):sys_addrinfo_ptr_t;
  extern proc sys_freeaddrinfo(res:sys_addrinfo_ptr_t);

  extern proc sys_getnameinfo(ref addr:sys_sockaddr_t, ref host_out:c_string, ref serv_outc_:c_string, flags:c_int):err_t;
  extern proc sys_getpeername(sockfd:fd_t, ref addr:sys_sockaddr_t):err_t;
  extern proc sys_getsockname(sockfd:fd_t, ref addr:sys_sockaddr_t):err_t;

  // TODO -- these should be generic, assuming caller knows what they
  // are doing.
  extern proc sys_getsockopt(sockfd:fd_t, level:c_int, optname:c_int, optval:c_void_ptr, ref optlen:socklen_t):err_t;
  extern proc sys_setsockopt(sockfd:fd_t, level:c_int, optname:c_int, optval:c_void_ptr, optlen:socklen_t):err_t;

  extern proc sys_listen(sockfd:fd_t, backlog:c_int):err_t;
  extern proc sys_shutdown(sockfd:fd_t, how:c_int):err_t;
  extern proc sys_socket(_domain:c_int, _type:c_int, protocol:c_int, ref sockfd_out:fd_t):err_t;
  extern proc sys_socketpair(_domain:c_int, _type:c_int, protocol:c_int, ref sockfd_out_a:fd_t, ref sockfd_out_b:fd_t):err_t;

  extern type fd_set;
  extern type time_t = c_long;
  extern type suseconds_t = c_long;
  extern "struct timeval" record timeval {
     var tv_sec:time_t; // seconds
     var tv_usec:suseconds_t; // microseconds
  }
  extern proc sys_select(nfds:c_int, readfds:c_ptr(fd_set), writefds:c_ptr(fd_set), exceptfds:c_ptr(fd_set), timeout:c_ptr(timeval), ref nset:c_int):err_t;
  extern proc sys_fd_clr(fd:c_int, ref set:fd_set);
  extern proc sys_fd_isset(fd:c_int, ref set:fd_set):c_int;
  extern proc sys_fd_set(fd:c_int, ref set:fd_set);
  extern proc sys_fd_zero(ref set:fd_set);

  // recv, recvfrom, recvmsg, send, sendto, sendmsg are in io
}
