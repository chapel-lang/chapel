/* system call wrapping */
module Sys {
  /* BASIC TYPES */
  use SysBasic;

 
  /* CONSTANTS */

  // note that if a constant is not defined in the C environment,
  // the compile will fail when it is used (and not here!)
  // That means that we can just list all of the constants we know of,
  // for different OSs, and if one is missing errors will occur in
  // C code generation.


  // basic file flags
  _extern const O_RDONLY:c_int;
  _extern const O_WRONLY:c_int;
  _extern const O_RDWR:c_int;

  // POSIX flags
  _extern const O_APPEND:c_int;
  _extern const O_CREAT:c_int;
  _extern const O_DSYNC:c_int;
  _extern const O_EXCL:c_int;
  _extern const O_NOCTTY:c_int;
  _extern const O_NONBLOCK:c_int;
  _extern const O_RSYNC:c_int;
  _extern const O_SYNC:c_int;
  _extern const O_TRUNC:c_int;

  // Linux flags (make them 0 if not on linux, please)
  _extern const O_ASYNC:c_int;
  _extern const O_CLOEXEC:c_int;
  _extern const O_DIRECT:c_int;
  _extern const O_DIRECTORY:c_int;
  _extern const O_LARGEFILE:c_int;
  _extern const O_NOATIME:c_int;
  _extern const O_NOFOLLOW:c_int;
  _extern const O_NDELAY:c_int;

  // open permission flags.
  _extern const S_IRWXU:c_int;
  _extern const S_IRUSR:c_int;
  _extern const S_IWUSR:c_int;
  _extern const S_IXUSR:c_int;
  _extern const S_IRWXG:c_int;
  _extern const S_IRGRP:c_int;
  _extern const S_IWGRP:c_int;
  _extern const S_IXGRP:c_int;
  _extern const S_IRWXO:c_int;
  _extern const S_IROTH:c_int;
  _extern const S_IWOTH:c_int;
  _extern const S_IXOTH:c_int;

  // socket domains
  _extern const AF_UNIX:c_int;
  _extern const AF_LOCAL:c_int;
  _extern const AF_INET:c_int;
  _extern const AF_INET6:c_int;
  _extern const AF_IPX:c_int;
  _extern const AF_NETLINK:c_int;
  _extern const AF_X25:c_int;
  _extern const AF_AX25:c_int;
  _extern const AF_ATMPVC:c_int;
  _extern const AF_APPLETALK:c_int;
  _extern const AF_PACKET:c_int;

  // socket types
  _extern const SOCK_STREAM:c_int;
  _extern const SOCK_DGRAM:c_int;
  _extern const SOCK_SEQPACKET:c_int;
  _extern const SOCK_RAW:c_int;
  _extern const SOCK_RDM:c_int;

  _extern const SOCK_NONBLOCK:c_int;
  _extern const SOCK_CLOEXEC:c_int;

  // sendmsg flags.
  _extern const MSG_CONFIRM:c_int;
  _extern const MSG_DONTROUTE:c_int;
  _extern const MSG_DONTWAIT:c_int;
  _extern const MSG_EOR:c_int;
  _extern const MSG_MORE:c_int;
  _extern const MSG_NOSIGNAL:c_int;
  _extern const MSG_OOB:c_int;

  // recvmsg flags
  _extern const MSG_CMSG_CLOEXEC:c_int;
  _extern const MSG_ERRQUEUE:c_int;
  _extern const MSG_PEEK:c_int;
  _extern const MSG_TRUNC:c_int;
  _extern const MSG_WAITALL:c_int;

  //_extern const MSG_EOR:c_int; sendmsg flag
  _extern const MSG_CTRUNC:c_int;

  // shutdown how.
  _extern const SHUT_RD:c_int;
  _extern const SHUT_WR:c_int;
  _extern const SHUT_RDWR:c_int;

  // socket option 'levels'
  _extern const IPPROTO_IP:c_int;
  _extern const IPPROTO_IPV6:c_int;
  _extern const IPPROTO_TCP:c_int;
  _extern const IPPROTO_UDP:c_int;

  // IP socket options
  _extern const IP_ADD_MEMBERSHIP:c_int;
  _extern const IP_DROP_MEMBERSHIP:c_int;
  _extern const IP_HDRINCL:c_int;
  _extern const IP_MTU:c_int;
  _extern const IP_MTU_DISCOVER:c_int;
  _extern const IP_MULTICAST_IF:c_int;
  _extern const IP_MULTICAST_LOOP:c_int;
  _extern const IP_MULTICAST_TTL:c_int;
  _extern const IP_OPTIONS:c_int;
  _extern const IP_PKTINFO:c_int;
  _extern const IP_RECVERR:c_int;
  _extern const IP_RECVOPTS:c_int;
  _extern const IP_RECVTOS:c_int;
  _extern const IP_RECVTTL:c_int;
  _extern const IP_RETOPTS:c_int;
  _extern const IP_ROUTER_ALERT:c_int;
  _extern const IP_TOS:c_int;
  _extern const IP_TTL:c_int;

  // IP 6 socket options
  _extern const IPV6_ADDRFORM:c_int;
  _extern const IPV6_ADD_MEMBERSHIP:c_int;
  _extern const IPV6_DROP_MEMBERSHIP:c_int;
  _extern const IPV6_MTU:c_int;
  _extern const IPV6_MTU_DISCOVER:c_int;
  _extern const IPV6_MULTICAST_HOPS:c_int;
  _extern const IPV6_MULTICAST_IF:c_int;
  _extern const IPV6_MULTICAST_LOOP:c_int;
  _extern const IPV6_PKTINFO:c_int;
  _extern const IPV6_RTHDR:c_int;
  _extern const IPV6_AUTHHDR:c_int;
  _extern const IPV6_DSTOPS:c_int;
  _extern const IPV6_HOPOPTS:c_int;
  _extern const IPV6_FLOWINFO:c_int;
  _extern const IPV6_HOPLIMIT:c_int;
  _extern const IPV6_RECVERR:c_int;
  _extern const IPV6_ROUTER_ALERT:c_int;
  _extern const IPV6_UNICAST_HOPS:c_int;
  _extern const IPV6_V6ONLY:c_int;

  // TCP socket options
  _extern const TCP_CORK:c_int;
  _extern const TCP_DEFER_ACCEPT:c_int;
  _extern const TCP_INFO:c_int;
  _extern const TCP_KEEPCNT:c_int;
  _extern const TCP_KEEPIDLE:c_int;
  _extern const TCP_KEEPINTVL:c_int;
  _extern const TCP_LINGER2:c_int;
  _extern const TCP_MAXSEG:c_int;
  _extern const TCP_NODELAY:c_int;
  _extern const TCY_QUICKACK:c_int;
  _extern const SYNCNT:c_int;
  _extern const TCP_WINDOW_CLAMP:c_int;

  // UDP socket options
  _extern const UDP_CORK:c_int;


  /* SOCKET STRUCTURE TYPES */

  _extern type sys_sockaddr_storage_t;
  _extern record sys_sockaddr_t {
    var addr:sys_sockaddr_storage_t;
    var len:socklen_t;
    proc sys_sockaddr_t() {
      sys_init_sys_sockaddr_t(this);
    }
  }

  _extern record sys_addrinfo_t {
    var ai_flags: c_int;
    var ai_family: c_int;
    var ai_socktype: c_int;
    var ai_protocol: c_int;
  }
  _extern type sys_addrinfo_ptr_t; // opaque


  proc sys_addrinfo_ptr_t.flags:c_int { return sys_getaddrinfo_flags(this); }
  proc sys_addrinfo_ptr_t.family:c_int { return sys_getaddrinfo_family(this); }
  proc sys_addrinfo_ptr_t.socktype:c_int { return sys_getaddrinfo_socktype(this); }
  proc sys_addrinfo_ptr_t.socktype:c_int { return sys_getaddrinfo_socktype(this); }
  proc sys_addrinfo_ptr_t.addr:sys_sockaddr_t { return sys_getaddrinfo_addr(this); }
  proc sys_addrinfo_ptr_t.canonname:string { return sys_getaddrinfo_cannonname(this); }
  proc sys_addrinfo_ptr_t.next:sys_addrinfo_ptr_t { return sys_getaddrinfo_next(this); }

  _extern proc sys_init_sys_sockaddr(inout addr:sys_sockaddr_t);
  _extern proc sys_strerror(error:err_t, inout string_out:string):err_t;
  _extern proc sys_strerror_str(error:err_t, inout err_in_strerror:err_t):string;

  _extern proc sys_readlink(path:string, inout string_out):err_t;
  _extern proc sys_readlink(path:string, inout string_out):err_t;
  _extern proc sys_open(pathname:string, flags:c_int, mode:mode_t, inout fd_out:fd_t):err_t;
  _extern proc sys_close(fd:fd_t):err_t;

  _extern proc sys_mmap(addr:c_ptr, length:size_t, prot:c_int, flags:c_int, fd:fd_t, offset:off_t, inout ret_out:c_ptr):err_t;
  _extern proc sys_munmap(addr:c_ptr, length:size_t):err_t;

  // readv, writev, preadv, pwritev -- can't (yet) pass array.
  
  _extern proc sys_fcntl(fd:fd_t, cmd:c_int, inout ret_out:c_int):err_t;
  _extern proc sys_fcntl_long(fd:fd_t, cmd:c_int, arg:c_long, inout ret_out:c_int):err_t;
  _extern proc sys_fcntl_ptr(fd:fd_t, cmd:c_int, arg:c_ptr, inout ret_out:c_int):err_t;
  _extern proc sys_dup(oldfd:fd_t, inout fd_out:fd_t):err_t;
  _extern proc sys_dup2(oldfd:fd_t, newfd:fd_t, inout fd_out:fd_t):err_t;
  _extern proc sys_pipe(inout read_fd_out:fd_t, inout write_fd_out:fd_t):err_t;
  _extern proc sys_accept(sockfd:fd_t, inout add_out:sys_sockaddr_t, inout fd_out:fd_t):err_t;
  _extern proc sys_bind(sockfd:fd_t, inout addr:sys_sockaddr_t):err_t;
  _extern proc sys_connect(sockfd:fd_t, inout addr:sys_sockaddr_t):err_t;
  _extern proc sys_getaddrinfo(node:string, service:string, inout hints:sys_addrinfo_t, inout res_out:sys_addrinfo_ptr_t):err_t;
  _extern proc sys_getaddrinfo_flags(res:sys_addrinfo_ptr_t):c_int;
  _extern proc sys_getaddrinfo_family(res:sys_addrinfo_ptr_t):c_int;
  _extern proc sys_getaddrinfo_socktype(res:sys_addrinfo_ptr_t):c_int;
  _extern proc sys_getaddrinfo_protocol(res:sys_addrinfo_ptr_t):c_int;
  _extern proc sys_getaddrinfo_addr(res:sys_addrinfo_ptr_t):sys_sockaddr_t;
  _extern proc sys_getaddrinfo_next(res:sys_addrinfo_ptr_t):sys_addrinfo_ptr_t;
  _extern proc sys_freeaddrinfo(res:sys_addrinfo_ptr_t);

  _extern proc sys_getnameinfo(inout addr:sys_sockaddr_t, inout host_out:string, inout serv_out:string, flags:c_int):err_t;
  _extern proc sys_getpeername(sockfd:fd_t, inout addr:sys_sockaddr_t):err_t;
  _extern proc sys_getsockname(sockfd:fd_t, inout addr:sys_sockaddr_t):err_t;

  // TODO -- these should be generic, assuming caller knows what they
  // are doing.
  _extern proc sys_getsockopt(sockfd:fd_t, level:c_int, optname:c_int, optval:c_ptr, inout optlen:socklen_t):err_t;
  _extern proc sys_setsockopt(sockfd:fd_t, level:c_int, optname:c_int, optval:c_ptr, optlen:socklen_t):err_t;

  _extern proc sys_listen(sockfd:fd_t, backlog:c_int):err_t;
  _extern proc sys_shutdown(sockfd:fd_t, how:c_int):err_t;
  _extern proc sys_socket(_domain:c_int, _type:c_int, protocol:c_int, inout sockfd_out:fd_t):err_t;
  _extern proc sys_socketpair(_domain:c_int, _type:c_int, protocol:c_int, inout sockfd_out_a:fd_t, inout sockfd_out_b:fd_t):err_t;

  // recv, recvfrom, recvmsg, send, sendto, sendmsg are in io
}

