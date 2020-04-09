//   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/sockutil.cpp $
// Description: Simple sock utils
// Copyright 1999, Dan Bonachea

#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <portable_inttypes.h>
#include "sockaddr.h" // for gasnet_tools/gasnet_config.h
#if HAVE_GETIFADDRS
  #if HAVE_IFADDRS_H
    #include <ifaddrs.h>
  #endif
  #include <net/if.h>
#endif

#include "sockutil.h"
#include "sig.h"

#ifndef if_pf /* for stand-alone mode without gasnet_tools */
#define if_pf(cond) if(cond)
#define if_pt(cond) if(cond)
#endif

bool endianconvert = false;

//-------------------------------------------------------------------------------------
static int isinit = 0;
static bool nh_cvt = false;
#define CHECKINIT() assert(isinit > 0)
bool socklibinit(){ 
  nh_cvt = isLittleEndian();
  isinit++; 
  return true; 
}
bool socklibend(){ isinit--; return true; }
//-------------------------------------------------------------------------------------
SOCKET listen_socket(unsigned short port, bool allowshared) {
  // create a socket to listen to a specific port
  return listen_socket(SockAddr((long unsigned int)INADDR_ANY, port), allowshared);
}
//-------------------------------------------------------------------------------------
SOCKET listen_socket(struct sockaddr* saddr, bool allowshared) {
  // create a socket to listen to a specific address
  SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s == INVALID_SOCKET) xsocket(s, "socket() failed while creating a listener socket");

  if (allowshared) {
    int allow = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&allow, sizeof(int)) == -1) {
      closesocket(s);
      xsocket(s, "setsockopt() failed to set reuseaddr while creating a listener socket");
    }
  }

  // bind
  if (bind(s, saddr, sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
    closesocket(s);
    xsocket(s, "bind() failed to bind a listener socket");
  }

  // listen
  if (listen(s, SOMAXCONN) == SOCKET_ERROR) {
    closesocket(s);
    xsocket(s, "listen() failed while creating a listener socket");
  }

  return s;
}
//------------------------------------------------------------------------------------

SOCKET accept_socket(SOCKET listener, struct sockaddr* calleraddr) {
  LENGTH_PARAM sz = (calleraddr?sizeof(struct sockaddr_in):0);
  while (1) {
    SOCKET newsock;
    if ((newsock = SOCK_accept(listener, calleraddr, &sz)) == INVALID_SOCKET) {
      if (errno == EINTR) continue; // ignore signal interruptions - keep blocking
      closesocket(listener);
      xsocket(listener, "accept() failed on listener socket");
    }

    disable_sigpipe(newsock);

    return newsock;
  }
}
//-------------------------------------------------------------------------------------
bool disable_sigpipe(SOCKET s) {
  #ifdef SO_NOSIGPIPE
    // Some OSs (eg BSD) allow SIGPIPE generation to be blocked at the socket level
    #ifndef SIGPIPE_BLOCKED
    #define SIGPIPE_BLOCKED 1
    #endif
    int set = 1;
    if (setsockopt(s, SOL_SOCKET, SO_NOSIGPIPE, (char *)&set, sizeof(set)) == -1) {
      closesocket(s);
      xsocket(s, "setsockopt() failed to set SO_NOSIGPIPE");
    }
    return true;
  #else // flag doesn't exist
    return false;
  #endif
} 
//-------------------------------------------------------------------------------------
SOCKET connect_socket(struct sockaddr* saddr) {
  SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s == INVALID_SOCKET) xsocket(s, "socket() failed while creating a connect socket");

  disable_sigpipe(s);

  if (connect(s, saddr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
    closesocket(s);
    xsocket(s, "connect() failed while creating a connect socket");
  }
  return s;
}
//-------------------------------------------------------------------------------------
SOCKET connect_socket(char* addr) {
  // create a socket and connect it to a remote host/port
  // interpret addr
  char* p = strchr(addr, ':');
  if (!p) throw xBase("Missing address in connect");
  *p = '\0';
  unsigned short portnum = atoi(++p);
  while(*p) { // check for stray crap
    if (!isdigit(*p)) throw xBase("Stray characters after address in connect");
    p++;
  }
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(portnum);
  for (int i=0; i < 8; i++) saddr.sin_zero[i] = '\0';
  if (isValidIP(addr)) saddr.sin_addr.s_addr = inet_addr(addr);
  else { // need to do DNS thing
    struct hostent* he = gethostbyname(addr);
    if (!he) xsocket(INVALID_SOCKET, "DNS failure in gethostbyname()");
    #if PLATFORM_OS_UNICOS
      assert(sizeof(u_long) == he->h_length);
      saddr.sin_addr.s_addr = *(u_long *)he->h_addr_list[0]; // s_addr is a bitfield on Unicos
    #else
      memcpy(&saddr.sin_addr.s_addr, he->h_addr_list[0], he->h_length);
    #endif
  }
  
  SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s == INVALID_SOCKET) xsocket(s, "socket() failed while creating a connect socket");

  disable_sigpipe(s);

  if (connect(s, (struct sockaddr*)&saddr, sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
    closesocket(s);
    xsocket(s, "connect() failed while creating a connect socket");
  }
  return s;
}
//-------------------------------------------------------------------------------------
void recvAll(SOCKET s, void* buffer, int numbytes) {
  // blocks until it can receive numbytes on s into buffer
  // (throws xSocket on close)
  char *buf = (char*)buffer;
  while (numbytes) {
    int retval;
    retval = recv(s, buf, numbytes, 0);
    if (retval == SOCKET_ERROR) {
      closesocket(s);
      xsocket(s, "error in recvAll() - connection closed");
    } else if (retval == 0) {
      closesocket(s);
      xsocket(s, "connection closed on recv() in recvAll()");
    }
    assert(retval <= numbytes); // can't send more than was in buffer...

    buf += retval;
    numbytes -= retval;
  }
}
//-------------------------------------------------------------------------------------
#ifdef MSG_NOSIGNAL
  // POSIX mandates a SIGPIPE on send() to a disconnected stream socket
  // but it's more efficient and less disruptive to clients to handle this via return code
  // Linux and a few others allow this to be disabled via send flag
  #define SEND_FLAGS MSG_NOSIGNAL 
  #ifndef SIGPIPE_BLOCKED
  #define SIGPIPE_BLOCKED 1
  #endif
#else
  #define SEND_FLAGS 0
#endif
void sendAll(SOCKET s, const void* buffer, int numbytes, bool dothrow) {
  // blocks until it can send numbytes on s from buffer
  // (throws xSocket on close by default)
  #if !SIGPIPE_BLOCKED
    // must use heavyweight method to block SIGPIPE errors
    LPSIGHANDLER oldsighandler = reghandler(SIGPIPE, (LPSIGHANDLER)SIG_IGN); 
  #endif
  char *buf = (char*)buffer;
  while (numbytes) {
    int retval;
    retval = send(s, buf, numbytes, SEND_FLAGS);
    if (retval == SOCKET_ERROR) {
      closesocket(s);
      #if !SIGPIPE_BLOCKED
        reghandler(SIGPIPE, oldsighandler); // restore handler
      #endif
      if (dothrow) xsocket(s, "error in sendAll() - connection closed");
      else break;
    }
    assert(retval <= numbytes); // can't send more than was in buffer...

    buf += retval;
    numbytes -= retval;
  }
  #if !SIGPIPE_BLOCKED
    reghandler(SIGPIPE, oldsighandler); // restore handler
  #endif
}
//-------------------------------------------------------------------------------------
void sendAll(SOCKET s, const char* buffer, int numbytes, bool dothrow) {
  if (numbytes == -1) numbytes = strlen(buffer);
  sendAll(s, (void *)buffer, numbytes, dothrow);
}
//-------------------------------------------------------------------------------------
void sendEOL(SOCKET s) {
  sendAll(s, "\r\n", 2);
}
//-------------------------------------------------------------------------------------
int recvLine(SOCKET s, char* buf, int bufsiz) {
  int readcount = 0;
  while(1) {
    int retval;
    retval = recv(s, buf+readcount, bufsiz-readcount, MSG_PEEK);
    if (retval == SOCKET_ERROR) xsocket(s, "error on recv() in recvLine()");
    else if (retval == 0) { // graceful close - nothing left to read
      if (readcount == 0) xsocket(s, "connection closed in recvLine()");
      else return readcount;
    }
    for(int i=0; i < retval-1; i++) {
      if (buf[readcount+i+1] == '\n') { // got eol
        bool bareeol = (buf[readcount+i] != '\r'); // also accept bare LF
        int retval = recv(s, buf+readcount, i+2, 0);
        if (retval == SOCKET_ERROR) xsocket(s, "error on recv() in recvLine()");
        else if (retval == 0) { // should never happen?
          if (readcount == 0) 
            xsocket(s, "connection closed in recv() in recvLine()"); // unexpected connection close
          else return readcount;
        }
        assert(retval == i+2); // make sure it gave us everything
        if (bareeol) {
          buf[readcount + i + 1] = '\0';
          return readcount + i + 1;
        } else {
          buf[readcount + i] = '\0';
          return readcount + i;
        }
      }
    }
    if (retval+readcount >= bufsiz) { // buffer too small to get entire line
      throw xBase("Buffer overrun in recvLine");
    } else { // get what we can
      int len = retval;
      retval = recv(s, buf+readcount, len, 0);
      if (retval == SOCKET_ERROR) xsocket(s, "error on recv() in recvLine()");
      else if (retval == 0) { // should never happen?
        if (readcount == 0) 
          xsocket(s, "connection closed on recv() in recvLine()"); // unexpected connection close
        else return readcount;
      }
      assert(retval == len); // make sure it gave us everything
      readcount += retval;
    }
  }
}
//------------------------------------------------------------------------------------
void getSockName(SOCKET s, sockaddr_in &addr) {
  GETSOCKNAME_LENGTH_T namelen = sizeof(sockaddr_in);    
  if (SOCK_getsockname(s, (sockaddr*)&addr, &namelen) == SOCKET_ERROR) {
    xsocket(s, "getsockname");
  }
}
//-------------------------------------------------------------------------------------
unsigned long getLocalAddress(SOCKET s) {
  sockaddr_in saddr;
  getSockName(s, saddr);
  return ntohl(saddr.sin_addr.s_addr);
}
//------------------------------------------------------------------------------------
int getLocalPort(SOCKET s) {
  sockaddr_in saddr;
  getSockName(s, saddr);
  return ntohs(saddr.sin_port);
}
//------------------------------------------------------------------------------------
void getSockPeer(SOCKET s, sockaddr_in &addr) {
  GETSOCKNAME_LENGTH_T namelen = sizeof(sockaddr_in);     
  if (SOCK_getpeername(s, (sockaddr*)&addr, &namelen) == SOCKET_ERROR) {
    xsocket(s, "getpeername");
  }
}
//------------------------------------------------------------------------------------
unsigned long getRemoteAddress(SOCKET s) {
  sockaddr_in saddr;
  getSockPeer(s, saddr);
  return ntohl(saddr.sin_addr.s_addr);
}
//------------------------------------------------------------------------------------
int getRemotePort(SOCKET s) {
  sockaddr_in saddr;
  getSockPeer(s, saddr);
  return ntohs(saddr.sin_port);
}
//------------------------------------------------------------------------------------
void byteSwap16(void *val) {
  uint8_t* p = (uint8_t *)val;
  uint8_t tmp;
  tmp = p[0];
  p[0] = p[1];
  p[1] = tmp;
}
//------------------------------------------------------------------------------------
void byteSwap32(void *val) {
  uint8_t* p = (uint8_t *)val;
  uint8_t tmp;
  tmp = p[0];
  p[0] = p[3];
  p[3] = tmp;
  tmp = p[1];
  p[1] = p[2];
  p[2] = tmp;
}
//------------------------------------------------------------------------------------
void byteSwap64(void *val) {
  uint8_t* p = (uint8_t *)val;
  uint8_t tmp;
  tmp = p[0];
  p[0] = p[7];
  p[7] = tmp;
  tmp = p[1];
  p[1] = p[6];
  p[6] = tmp;
  tmp = p[2];
  p[2] = p[5];
  p[5] = tmp;
  tmp = p[3];
  p[3] = p[4];
  p[4] = tmp;
}
//-------------------------------------------------------------------------------------
#define NTOH_DEF(width)                            \
  uint##width##_t ntoh##width(uint##width##_t v) { \
    CHECKINIT();                                   \
    if (nh_cvt) byteSwap##width(&v);               \
    return v;                                      \
  }                                                \
  void ntoh##width##a(void *pv) {                  \
    CHECKINIT();                                   \
    if (nh_cvt) byteSwap##width(pv);               \
  }                                                \
  uint##width##_t hton##width(uint##width##_t v) { \
    CHECKINIT();                                   \
    if (nh_cvt) byteSwap##width(&v);               \
    return v;                                      \
  }                                                \
  void hton##width##a(void *pv) {                  \
    CHECKINIT();                                   \
    if (nh_cvt) byteSwap##width(pv);               \
  }                                                
NTOH_DEF(16)
NTOH_DEF(32)
NTOH_DEF(64)
//-------------------------------------------------------------------------------------
uint32_t recv32(SOCKET s) { // get 32-bit integer
  uint32_t temp;
  recvAll(s, &temp, 4);
  if (endianconvert) byteSwap32(&temp);
  return temp;
}
//-------------------------------------------------------------------------------------
void send32(SOCKET s, uint32_t value) { // send 32-bit integer
  if (endianconvert) byteSwap32(&value);
  sendAll(s, &value, 4);
}
//-------------------------------------------------------------------------------------
char recvch(SOCKET s) { // get one character
  char temp;
  recvAll(s, &temp, 1);
  return temp;
}
//-------------------------------------------------------------------------------------
SockAddr getsockname(SOCKET s) {
  GETSOCKNAME_LENGTH_T sz = sizeof(struct sockaddr);
  SockAddr saddr;
  if (SOCK_getsockname(s, (struct sockaddr *)saddr, &sz) == SOCKET_ERROR) 
    xsocket(s, "getsockname");
  return saddr;
}
//-------------------------------------------------------------------------------------
SockAddr getpeername(SOCKET s) {
  GETSOCKNAME_LENGTH_T sz = sizeof(struct sockaddr);
  SockAddr saddr;
  if (SOCK_getpeername(s, (struct sockaddr *)saddr, &sz) == SOCKET_ERROR) 
    xsocket(s, "getpeername");
  return saddr;
}
//-------------------------------------------------------------------------------------
char const *getMyHostName() {
  static char hostname[1024]; // assume never changes during program run
  static bool firsttime = true;
  if (firsttime) {
    if (gethostname(hostname, 1024) == SOCKET_ERROR) 
      xsocket(INVALID_SOCKET, "gethostname");
    firsttime = false;
  }
  return hostname;
}
//-------------------------------------------------------------------------------------
SockAddr DNSLookup(const char *hostnameOrIPStr) {
  const char *hostname = hostnameOrIPStr;
  if (isValidIP(hostnameOrIPStr)) { // numeric IP
    return SockAddr(hostnameOrIPStr, 0);
  } else {
    hostent *he = gethostbyname(hostname);
    if (!he) xsocket(INVALID_SOCKET, "gethostbyname"); 
    if (he->h_length != 4) xsocket(INVALID_SOCKET, "gethostbyname returned wrong h_length"); 
    if (he->h_addr_list[0] == NULL) xsocket(INVALID_SOCKET, "gethostbyname returned no entries"); 
    return SockAddr((unsigned long)ntohl(*((uint32_t *)he->h_addr_list[0])), (unsigned short)0);
  }
}
//-------------------------------------------------------------------------------------
bool inputWaiting(SOCKET s, bool dothrow) { // returns true if input or close conn is waiting
  fd_set sockset;
  timeval tm = {0, 0};
  FD_ZERO(&sockset);
  FD_SET(s, &sockset);
  int retval = select(s+1, &sockset, NULL, NULL, &tm);
  if_pf (retval == SOCKET_ERROR) {
    if (dothrow) xsocket(s, "select");
    else return true; // error waiting
  } else if (retval > 0) return true; // new input or closed conn
  
  return false;
}
//-------------------------------------------------------------------------------------
int numBytesWaiting(SOCKET s) { // returns number of bytes waiting to be received
  IOCTL_FIONREAD_ARG_T arg = 0;
  if (SOCK_ioctlsocket(s, _FIONREAD, &arg) == SOCKET_ERROR) 
    xsocket(s, "numBytesWaiting");
  return (int)arg;
}
//-------------------------------------------------------------------------------------
bool isValidIP(const char* buf) {
  int a = atoi(buf);
  if (a > 255 || a < 0) return false;
  while (isdigit(*buf)) buf++;
  if (*buf != '.') return false;
  buf++;

  int b = atoi(buf);
  if (b > 255 || b < 0) return false;
  while (isdigit(*buf)) buf++;
  if (*buf != '.') return false;
  buf++;

  int c = atoi(buf);
  if (c > 255 || c < 0) return false;
  while (isdigit(*buf)) buf++;
  if (*buf != '.') return false;
  buf++;

  int d = atoi(buf);
  if (d > 255 || d < 0) return false;
  while (isdigit(*buf)) buf++;
  while (isspace(*buf)) buf++;
  if (*buf != '\0') return false;

  return true;
}
//-------------------------------------------------------------------------------------
bool isLittleEndian() {
  union {
    int i;                  // machine word
    unsigned char b[sizeof(int)];    // b[0] overlaid with first byte of i
  } x;
  x.i = 0xFF;    // set lsb, zero all others
  return x.b[0] == 0xFF;
}
//-------------------------------------------------------------------------------------
bool isBigEndian() {
  return !isLittleEndian();
}
//-------------------------------------------------------------------------------------
void close_socket(SOCKET s) {
  if (closesocket(s) == SOCKET_ERROR) {
    xsocket(s, "closesocket");
  }
}
//-------------------------------------------------------------------------------------
void closeGracefully(SOCKET s){
  // close a socket gracefully, blocking until everything is sent
  char temp;
  shutdown(s, SD_SEND); // initiate graceful close
  recv(s, &temp, 1, 0); // wait for other side to acknowledge or reset
  closesocket(s); // deallocate socket
}
//-------------------------------------------------------------------------------------
void waitForClose(SOCKET s) {
  char temp;
  int retval = recv(s, &temp, 1, MSG_PEEK); // block for activity
  if (retval == 0) return;
  else if (retval == SOCKET_ERROR) xsocket(s, "waitForClose()");
  else {
    xsocket(s, "waitForClose() got data when close expected");
  }
}
//-------------------------------------------------------------------------------------
bool waitForActivity(SOCKET s, struct timeval* tv) {
  fd_set set;
  FD_ZERO(&set);
  FD_SET(s, &set);
  int retval = select(s+1, &set, NULL, NULL, tv);
  if (retval < 0) {
    xsocket(s, "waitForActivity()");
  }
  else if (retval == 1) return true;
  else if (retval == 0) return false;
  else { perror("select"); abort(); }

  return false; /* make compiler happy */
}
//-------------------------------------------------------------------------------------
bool isClosed(SOCKET s) {
  // first, test to see if recv will block
  fd_set set;
  FD_ZERO(&set);
  FD_SET(s, &set);
  struct timeval tv = {0,0};
  if (!select(s+1, &set, NULL, NULL, &tv)) {
    // isn't readable, therefore can't be closed
    // (or at least our OS can't have detected it)
    return false;
  }

  char c;
  int len = recv(s, &c, 1, MSG_PEEK); // something is waiting -> never blocks
  if (len == 0) return true; // socket closed
  else if (len == SOCKET_ERROR) {
    int err = errno;
    if (err == ECONNRESET ||
        err == ENOTCONN ||
        err == ENETRESET ||
        err == ECONNABORTED ||
        err == ESHUTDOWN ||
        err == EPIPE ||
        err == ENOTSOCK ||
        err == EBADF ||
        err == ETIMEDOUT) return true;
    else xsocket(s, "recv(MSG_PEEK) within isClosed()"); // some error
  }

  // not closed
  return false;
}
//-----------------------------------------------------------------------------------
bool hasOOBdata(SOCKET s) {
  FD_SET set;
  FD_ZERO(&set);
  FD_SET(s, &set);
  struct timeval tv = {0,0};
  if (select(s+1, NULL, NULL, &set, &tv)) {
    // see what it is for debugging
    char buffer[10];
    int retval = recv(s, buffer, 10, MSG_OOB);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------------

#undef select
extern int myselect(int  n,  fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
            struct timeval *timeout) {
  #ifdef FD_SETSIZE
    assert((unsigned int)n <= (unsigned int)FD_SETSIZE);
  #endif

  /* a select that ignores UNIX's ridiculously inconvenient interrupt signals */
  int retval;
  do {
      retval = select(n, readfds, writefds, exceptfds, timeout);
  } while (retval == SOCKET_ERROR && errno == EINTR);
  return retval;
}
/* ------------------------------------------------------------------------------------ */
extern int myrecvfrom(SOCKET s, char * buf, int len, int flags,                  
                      struct sockaddr *from, int *fromlen) {
  while (1) {
    LENGTH_PARAM sz;
    LENGTH_PARAM *psz = &sz;
    if (fromlen) sz = *fromlen;
    else psz = NULL;
    int retval = SOCK_recvfrom(s, buf, len, flags, from, psz);
    if (fromlen) *fromlen = (int)sz;

    if (retval == SOCKET_ERROR && errno == EINTR) continue;

    return retval;
  }
}
/* ------------------------------------------------------------------------------------ */
#if HAVE_GETIFADDRS
bool getIfaceAddr(SockAddr ipnet_sa, SockAddr &ret, char *subnets, size_t subnetsz) {
  struct sockaddr_in *net = (sockaddr_in*)ipnet_sa;
  struct sockaddr_in *result = (sockaddr_in*)ret;
  bool found = false;
  struct ifaddrs *ifas;
  if (subnets) subnets[0] = '\0';

  if (getifaddrs(&ifas) != -1) {
    for (struct ifaddrs *ifa = ifas; ifa != NULL; ifa = ifa->ifa_next) {
      if (!ifa->ifa_addr 
          || (ifa->ifa_addr->sa_family != AF_INET)  
          || !(ifa->ifa_flags & IFF_RUNNING) // ignore disabled interfaces
       ) continue;
    
      struct sockaddr_in *if_addr = (sockaddr_in*)ifa->ifa_addr;
      struct sockaddr_in *if_mask = (sockaddr_in*)ifa->ifa_netmask;
      unsigned long subnet = if_addr->sin_addr.s_addr & if_mask->sin_addr.s_addr;

      if (if_addr->sin_addr.s_addr == INADDR_ANY) continue; // can't use a wildcard interface (0.0.0.0)

      if (subnets && subnetsz > 0) {
        snprintf(subnets, subnetsz, "%s ",SockAddr((unsigned long)ntohl(subnet),0).IPStr());
        size_t len = strlen(subnets);
        subnets += len;
        subnetsz -= len;
      }

      if (subnet == net->sin_addr.s_addr) {
        memcpy(result, if_addr, sizeof(struct sockaddr_in));
        result->sin_port = 0;
        found = true;
        break;
      }
    }
    freeifaddrs(ifas);
  }

  return found;
}
#endif // HAVE_GETIFADDRS
/* ------------------------------------------------------------------------------------ */
